#pragma once
// Duo Vocal Comp - complete signal chain, framework independent (no JUCE).
// The plugin processor is a thin wrapper around this class, so the DSP test suite
// (tests/) measures exactly the code that ships.
//
// Signal flow (per channel):
//   in -> sanitize -> [dry delay line: exactly the reported latency] -------------------+
//      -> upsample 1x/2x/4x -> FET / Opto (order switchable, each crossfaded on/off)    |
//      -> downsample -> auto makeup -> mix with delayed dry (base rate) -> output gain <-+
//      -> SC-listen crossfade -> OS-switch fade -> bypass crossfade -> out
//
// Real-time safety: prepare() allocates everything; process() never allocates, locks or
// throws. Oversampling changes are RT-safe (all filters for every factor are precomputed)
// and are hidden behind a 3 ms fade-out / fade-in.

#include "Dsp.h"
#include "Oversampling.h"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <vector>

#if defined(__SSE__) || defined(_M_X64) || defined(__x86_64__)
 #include <xmmintrin.h>
 #define DUO_HAS_SSE 1
#endif

namespace duo
{
struct Params
{
    // v1 parameters (IDs frozen in the plugin)
    bool  fetOn = true;
    float fetInDb = 6.0f, fetOutDb = 0.0f, fetAtkMs = 0.2f, fetRelMs = 150.0f;
    int   fetRatio = 0;                         // 0..4 = 4, 8, 12, 20, All
    bool  optoOn = true;
    float optoPR = 30.0f, optoGainDb = 0.0f;
    int   optoMode = 0;                         // 0 Compress, 1 Limit
    int   order = 0;                            // 0 FET > Opto, 1 Opto > FET
    float scHpfHz = 80.0f, colorPct = 30.0f, mixPct = 100.0f, outDb = 0.0f;
    // v2 parameters
    float linkPct = 100.0f, emphasisPct = 0.0f;
    int   osLog2 = 1;                           // 0 = 1x, 1 = 2x, 2 = 4x
    bool  scListen = false, autoMakeup = false;
    bool  bypass = false;                       // host bypass (latency-compensated)
};

inline FetStage::Settings fetSettings (const Params& p) noexcept
{
    FetStage::Settings s;
    s.inDb = p.fetInDb;
    s.outDb = p.fetOutDb;
    s.attackMs = p.fetAtkMs;
    s.releaseMs = p.fetRelMs;
    s.ratio = p.fetRatio;
    s.scHz = p.scHpfHz;
    s.color = p.colorPct * 0.01f;
    s.link = p.linkPct * 0.01f;
    return s;
}

inline OptoStage::Settings optoSettings (const Params& p) noexcept
{
    OptoStage::Settings s;
    s.peakReduction = p.optoPR;
    s.gainDb = p.optoGainDb;
    s.limit = p.optoMode == 1;
    s.scHz = p.scHpfHz;
    s.color = p.colorPct * 0.01f;
    s.link = p.linkPct * 0.01f;
    s.emphasis = p.emphasisPct * 0.01f;
    return s;
}

struct Meters
{
    float inPeak[2] {}, outPeak[2] {};
    float fetGrDb = 0.0f, optoGrDb = 0.0f, makeupDb = 0.0f;
    void clear() noexcept { *this = Meters(); }
};

// test hook: records the per-oversampled-sample gain reduction (max over channels)
struct Probe
{
    float* fetGr = nullptr;
    float* optoGr = nullptr;
    int capacity = 0;
    int count = 0;
};

// flush-to-zero / denormals-are-zero for the duration of a process call (restores the host's mode)
struct ScopedFlushDenormals
{
#if DUO_HAS_SSE
    ScopedFlushDenormals() noexcept : saved (_mm_getcsr()) { _mm_setcsr (saved | 0x8040u); }
    ~ScopedFlushDenormals() { _mm_setcsr (saved); }
    unsigned int saved;
#endif
};

class Engine
{
public:
    static constexpr int kChunk = 256;

    void prepare (double sampleRate, int numChannels)
    {
        fs = sampleRate;
        numCh = std::clamp (numChannels, 1, kMaxCh);
        os.prepare (kChunk, fs);
        fet.prepare (fs);
        opto.prepare (fs);

        int ring = 1;
        while (ring < std::max (os.latencyFor (1), os.latencyFor (2)) + kChunk + 1)
            ring <<= 1;
        ringMask = ring - 1;
        for (int c = 0; c < kMaxCh; ++c)
        {
            dryRing[c].assign ((size_t) ring, 0.0f);
            wet[c].assign ((size_t) kChunk, 0.0f);
        }
        grBase.assign ((size_t) kChunk, 0.0f);

        smoothBase  = 1.0f - onePole (0.02f, fs);
        switchK     = 1.0f - onePole (0.01f, fs);
        gateRelease = onePole (0.1f, fs);
        avgK        = 1.0f - onePole (kAutoMakeupSec, fs);
        makeupK     = 1.0f - onePole (0.3f, fs);
        fadeStep    = (float) (1.0 / std::max (1.0, 0.003 * fs));

        curOs.store (std::clamp (params.osLog2, 0, 2));
        applyOversampling (curOs.load());
        setParams (params);
        reset();
    }

    // clears all signal state and snaps every smoother to its target
    void reset() noexcept
    {
        os.reset();
        fet.resetState();
        opto.resetState();
        for (auto& r : dryRing)
            std::fill (r.begin(), r.end(), 0.0f);
        writePos = 0;
        if (requestedOs != curOs.load())
            applyOversampling (requestedOs);
        activeOrder = params.order;
        fadeGain = 1.0f;
        fadeState = Fade::none;
        avgGrDb = makeupDb = 0.0f;
        gateEnv = 0.0f;
        listenHpf.resetState();
        listenShelf.resetState();
        fet.snap();
        opto.snap();
        fetAmt = fetTarget;
        optoAmt = optoTarget;
        mixCur = mixTarget;
        outCur = outTarget;
        listenAmt = listenTarget;
        bypassAmt = bypassTarget;
        meters.clear();
    }

    void setParams (const Params& p) noexcept
    {
        params = p;
        fet.setParams (fetSettings (p));
        opto.setParams (optoSettings (p));
        fetTarget = p.fetOn ? 1.0f : 0.0f;
        optoTarget = p.optoOn ? 1.0f : 0.0f;
        mixTarget = std::clamp (p.mixPct, 0.0f, 100.0f) * 0.01f;
        outTarget = dbToGain (std::clamp (p.outDb, -24.0f, 24.0f));
        listenTarget = p.scListen ? 1.0f : 0.0f;
        bypassTarget = p.bypass ? 1.0f : 0.0f;
        requestedOs = std::clamp (p.osLog2, 0, 2);
        listenHpf.setHighPass (fs, std::clamp (p.scHpfHz, 20.0f, 300.0f));
        listenShelf.setHighShelf (fs, 3000.0, 12.0 * std::clamp (p.emphasisPct, 0.0f, 100.0f) * 0.01);
    }

    // latency of the currently active oversampling factor (thread-safe read)
    int getLatencySamples() const noexcept { return os.latencyFor (curOs.load (std::memory_order_relaxed)); }
    int latencyForOversampling (int log2) const noexcept { return os.latencyFor (log2); }
    int getOversamplingLog2() const noexcept { return curOs.load (std::memory_order_relaxed); }
    double getOversampledRate() const noexcept { return fs * (1 << curOs.load (std::memory_order_relaxed)); }
    const Oversampler& oversampler() const noexcept { return os; }

    const Meters& getMeters() const noexcept { return meters; }
    void clearMeters() noexcept { meters.clear(); }
    void setProbe (Probe* p) noexcept { probe = p; }

    void process (float* const* io, int numChannels, int numSamples) noexcept
    {
        ScopedFlushDenormals ftz;
        const int nCh = std::clamp (numChannels, 0, numCh);
        if (nCh == 0)
            return;
        for (int offset = 0; offset < numSamples; offset += kChunk)
        {
            const int n = std::min (kChunk, numSamples - offset);
            float* ptr[kMaxCh] {};
            for (int c = 0; c < nCh; ++c)
                ptr[c] = io[c] + offset;
            processChunk (ptr, nCh, n);
        }
    }

private:
    enum class Fade { none, out, in };
    static constexpr float kAutoMakeupSec = 2.0f;   // spec: time constant >= 1 s
    static constexpr float kGateLevel = 0.00316f;   // -50 dBFS: hold the GR average in pauses
    static constexpr float kAutoMakeupMaxDb = 24.0f; // never add more than +24 dB automatically

    void applyOversampling (int log2) noexcept
    {
        curOs.store (log2, std::memory_order_relaxed);
        os.reset();
        fet.setOversampling (log2);
        opto.setOversampling (log2);
        stageK = 1.0f - onePole (0.01f, fs * (1 << log2));
    }

    void processChunk (float* const* io, int nCh, int n) noexcept
    {
        // 0. sanitize input, meter, feed the dry delay line
        for (int c = 0; c < nCh; ++c)
        {
            float* d = dryRing[c].data();
            for (int i = 0; i < n; ++i)
            {
                float v = io[c][i];
                v = std::isfinite (v) ? std::clamp (v, -1000.0f, 1000.0f) : 0.0f;
                io[c][i] = v;
                meters.inPeak[c] = std::max (meters.inPeak[c], std::abs (v));
                d[(writePos + i) & ringMask] = v;
            }
        }

        // 1. structural changes (oversampling factor, stage order) can't be crossfaded sample by
        //    sample: fade out (3 ms), switch at a chunk boundary, fade back in - no clicks
        const bool structural = requestedOs != curOs.load (std::memory_order_relaxed) || params.order != activeOrder;
        if (fadeState == Fade::none && structural)
            fadeState = Fade::out;
        if (fadeState == Fade::out && fadeGain <= 0.0f)
        {
            if (requestedOs != curOs.load (std::memory_order_relaxed))
                applyOversampling (requestedOs);
            activeOrder = params.order;
            fadeState = Fade::in;
        }

        // 2. oversampled processing
        const int log2 = curOs.load (std::memory_order_relaxed);
        const int factor = 1 << log2;
        const int nS = n * factor;
        float* hi[kMaxCh] {};
        for (int c = 0; c < nCh; ++c)
            hi[c] = os.up (io[c], n, c, log2);

        float fetPeak = 0.0f, optoPeak = 0.0f;
        float x[kMaxCh] {}, pre[kMaxCh] {};
        for (int s = 0; s < nS; ++s)
        {
            for (int c = 0; c < nCh; ++c)
                x[c] = hi[c][s];

            float fg = 0.0f, og = 0.0f;
            for (int pass = 0; pass < 2; ++pass)
            {
                const bool isFet = (pass == 0) == (activeOrder == 0);
                if (isFet)
                    fg = runStage (fet, fetAmt, fetTarget, x, pre, nCh);
                else
                    og = runStage (opto, optoAmt, optoTarget, x, pre, nCh);
            }
            fetPeak = std::max (fetPeak, fg);
            optoPeak = std::max (optoPeak, og);
            if (probe != nullptr && probe->count < probe->capacity)
            {
                if (probe->fetGr != nullptr)  probe->fetGr[probe->count] = fg;
                if (probe->optoGr != nullptr) probe->optoGr[probe->count] = og;
                ++probe->count;
            }
            if ((s & (factor - 1)) == factor - 1)
                grBase[(size_t) (s >> log2)] = fg + og;

            for (int c = 0; c < nCh; ++c)
                hi[c][s] = x[c];
        }
        for (int c = 0; c < nCh; ++c)
            os.down (wet[c].data(), n, c, log2);
        meters.fetGrDb = std::max (meters.fetGrDb, fetPeak);
        meters.optoGrDb = std::max (meters.optoGrDb, optoPeak);

        // 3. base rate: auto makeup, latency-aligned dry/wet mix, output, listen, fades
        const int L = os.latencyFor (log2);
        for (int i = 0; i < n; ++i)
        {
            float inAbs = 0.0f;
            for (int c = 0; c < nCh; ++c)
                inAbs = std::max (inAbs, std::abs (io[c][i]));
            gateEnv = std::max (inAbs, gateEnv * gateRelease);
            if (gateEnv > kGateLevel)
                avgGrDb += (grBase[(size_t) i] - avgGrDb) * avgK;
            makeupDb += ((params.autoMakeup ? std::min (avgGrDb, kAutoMakeupMaxDb) : 0.0f) - makeupDb) * makeupK;
            const float mk = std::abs (makeupDb) > 0.0f ? dbToGain (makeupDb) : 1.0f;

            mixCur    += (mixTarget - mixCur) * smoothBase;
            outCur    += (outTarget - outCur) * smoothBase;
            if (listenAmt <= 0.0f && listenTarget > 0.0f)
            {
                listenHpf.resetState();
                listenShelf.resetState();
            }
            listenAmt += (listenTarget - listenAmt) * switchK;
            bypassAmt += (bypassTarget - bypassAmt) * switchK;
            snapIfClose (mixCur, mixTarget);
            snapIfClose (outCur, outTarget);
            snapIfClose (listenAmt, listenTarget);
            snapIfClose (bypassAmt, bypassTarget);

            if (fadeState == Fade::out)
                fadeGain = std::max (0.0f, fadeGain - fadeStep);
            else if (fadeState == Fade::in && (fadeGain = std::min (1.0f, fadeGain + fadeStep)) >= 1.0f)
                fadeState = Fade::none;

            for (int c = 0; c < nCh; ++c)
            {
                const float dry = dryRing[c][(size_t) ((writePos + i - L) & ringMask)];
                float y = (dry + (wet[c][(size_t) i] * mk - dry) * mixCur) * outCur;
                if (listenAmt > 0.0f)
                {
                    const float lis = (float) listenShelf.process (listenHpf.process (dry, c), c);
                    y += (lis * outCur - y) * listenAmt;
                }
                y += (dry - y) * bypassAmt;
                y *= fadeGain;
                io[c][i] = y;
                meters.outPeak[c] = std::max (meters.outPeak[c], std::abs (y));
            }
        }
        writePos = (writePos + n) & ringMask;
        meters.makeupDb = makeupDb;
    }

    // runs a stage with a 10 ms on/off crossfade; a fully-off stage is skipped (and its
    // envelope reset so it starts clean when switched back on)
    template <typename Stage>
    float runStage (Stage& st, float& amt, float target, float* x, float* pre, int nCh) noexcept
    {
        if (amt <= 0.0f && target <= 0.0f)
            return 0.0f;
        if (amt <= 0.0f)
            st.resetState();
        amt += (target - amt) * stageK;
        snapIfClose (amt, target);
        if (amt >= 1.0f)
            return st.processFrame (x, nCh);
        for (int c = 0; c < nCh; ++c)
            pre[c] = x[c];
        const float g = st.processFrame (x, nCh);
        for (int c = 0; c < nCh; ++c)
            x[c] = pre[c] + (x[c] - pre[c]) * amt;
        return g * amt;
    }

    static void snapIfClose (float& v, float target) noexcept
    {
        if (std::abs (v - target) < 1.0e-6f)
            v = target;
    }

    double fs = 44100.0;
    int numCh = 2;
    Params params;
    Oversampler os;
    FetStage fet;
    OptoStage opto;
    std::atomic<int> curOs { 1 };
    int requestedOs = 1, activeOrder = 0;

    std::vector<float> dryRing[kMaxCh], wet[kMaxCh], grBase;
    int ringMask = 0, writePos = 0;

    float fetAmt = 1.0f, fetTarget = 1.0f, optoAmt = 1.0f, optoTarget = 1.0f;
    float mixTarget = 1.0f, mixCur = 1.0f, outTarget = 1.0f, outCur = 1.0f;
    float listenTarget = 0.0f, listenAmt = 0.0f, bypassTarget = 0.0f, bypassAmt = 0.0f;
    float smoothBase = 0.001f, switchK = 0.002f, stageK = 0.001f;
    float gateEnv = 0.0f, gateRelease = 0.999f, avgGrDb = 0.0f, avgK = 0.0f, makeupDb = 0.0f, makeupK = 0.0f;
    float fadeGain = 1.0f, fadeStep = 0.01f;
    Fade fadeState = Fade::none;
    Biquad listenHpf, listenShelf;
    Meters meters;
    Probe* probe = nullptr;
};
} // namespace duo
