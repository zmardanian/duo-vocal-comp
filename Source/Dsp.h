#pragma once
// Duo Vocal Comp - DSP core (framework independent, no JUCE: unit-testable with plain g++)
// Stage 1: FET (1176-style) peak compressor
// Stage 2: Opto (LA-2A-style) leveling amplifier with two-stage, program-dependent release
//
// Both stages run at the oversampled rate. Each stage can be prepared for 1x/2x/4x and
// switched between them without allocating (coefficients for every factor are precomputed).

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace duo
{
constexpr int kMaxCh = 2;
constexpr double kPi = 3.141592653589793;
constexpr int kNumOsFactors = 3; // 1x, 2x, 4x

//==============================================================================
// Fast, deterministic dB conversions (called per oversampled sample).
// fastLog2: |error| < 1e-6 (=> < 1e-5 dB). fastExp2: relative error < 2e-7, exp2(0) == 1 exactly.
inline float fastLog2 (float x) noexcept
{
    uint32_t bits;
    std::memcpy (&bits, &x, sizeof bits);
    int e = (int) ((bits >> 23) & 0xffu) - 127;
    bits = (bits & 0x007fffffu) | 0x3f800000u;
    float m;
    std::memcpy (&m, &bits, sizeof m);
    if (m > 1.41421356f)
    {
        m *= 0.5f;
        ++e;
    }
    const float t = (m - 1.0f) / (m + 1.0f);
    const float t2 = t * t;
    const float s = t * (1.0f + t2 * (0.333333333f + t2 * (0.2f + t2 * 0.142857143f)));
    return (float) e + 2.88539008f * s;
}

inline float fastExp2 (float x) noexcept
{
    x = std::clamp (x, -126.0f, 126.0f);
    const float r = std::floor (x + 0.5f);
    const float f = (x - r) * 0.693147181f; // |f| <= 0.3466
    const float p = 1.0f + f * (1.0f + f * (0.5f + f * (0.166666667f + f * (0.0416666667f + f * (0.00833333333f + f * 0.00138888889f)))));
    const uint32_t bits = (uint32_t) ((int) r + 127) << 23;
    float scale;
    std::memcpy (&scale, &bits, sizeof scale);
    return p * scale;
}

inline float dbToGain (float db) noexcept { return fastExp2 (db * 0.166096405f); }
inline float gainToDb (float g) noexcept { return 6.02059991f * fastLog2 (std::max (g, 1.0e-9f)); }
inline float powerToDb (float p) noexcept { return 3.01029996f * fastLog2 (std::max (p, 1.0e-18f)); }

// one-pole smoothing coefficient for a time constant in seconds (computed at block rate)
inline float onePole (float seconds, double fs) noexcept
{
    return seconds <= 0.0f ? 0.0f : (float) std::exp (-1.0 / ((double) seconds * fs));
}

// 1 - exp(-x) for the small per-sample rates of slow envelopes (x < 0.01): avoids a
// transcendental per sample and lets release times change smoothly every sample.
inline float oneMinusExpSmall (float x) noexcept { return x * (1.0f - x * (0.5f - x * 0.166666667f)); }

// static soft-knee curve, returns gain reduction in dB (positive number)
inline float gainReductionDb (float levelDb, float thresholdDb, float ratio, float kneeDb) noexcept
{
    const float over  = levelDb - thresholdDb;
    const float slope = 1.0f - 1.0f / ratio;

    if (kneeDb > 0.0f)
    {
        if (2.0f * over < -kneeDb)
            return 0.0f;
        if (2.0f * std::abs (over) <= kneeDb)
        {
            const float t = over + kneeDb * 0.5f;
            return slope * t * t / (2.0f * kneeDb);
        }
    }
    else if (over <= 0.0f)
    {
        return 0.0f;
    }
    return slope * over;
}

//==============================================================================
// RBJ biquad (high-pass / high-shelf), double precision state, up to kMaxCh channels.
class Biquad
{
public:
    void setHighPass (double fs, double freq, double q = 0.7071067811865476)
    {
        if (std::abs (freq - lastFreq) < 1.0e-9 && std::abs (fs - lastFs) < 1.0e-9 && lastType == 1)
            return;
        lastFreq = freq;
        lastFs = fs;
        lastType = 1;
        const double w0 = 2.0 * kPi * std::min (freq, fs * 0.45) / fs;
        const double cosw = std::cos (w0), alpha = std::sin (w0) / (2.0 * q), a0 = 1.0 + alpha;
        b0 = (1.0 + cosw) * 0.5 / a0;
        b1 = -(1.0 + cosw) / a0;
        b2 = b0;
        a1 = -2.0 * cosw / a0;
        a2 = (1.0 - alpha) / a0;
    }

    void setHighShelf (double fs, double freq, double gainDb)
    {
        if (std::abs (freq - lastFreq) < 1.0e-9 && std::abs (fs - lastFs) < 1.0e-9 && lastType == 2
            && std::abs (gainDb - lastGain) < 1.0e-9)
            return;
        lastFreq = freq;
        lastFs = fs;
        lastType = 2;
        lastGain = gainDb;
        const double A = std::pow (10.0, gainDb / 40.0);
        const double w0 = 2.0 * kPi * std::min (freq, fs * 0.45) / fs;
        const double cosw = std::cos (w0), sinw = std::sin (w0);
        const double alpha = sinw / 2.0 * std::sqrt (2.0); // shelf slope S = 1
        const double sq = 2.0 * std::sqrt (A) * alpha;
        const double a0 = (A + 1.0) - (A - 1.0) * cosw + sq;
        b0 = A * ((A + 1.0) + (A - 1.0) * cosw + sq) / a0;
        b1 = -2.0 * A * ((A - 1.0) + (A + 1.0) * cosw) / a0;
        b2 = A * ((A + 1.0) + (A - 1.0) * cosw - sq) / a0;
        a1 = 2.0 * ((A - 1.0) - (A + 1.0) * cosw) / a0;
        a2 = ((A + 1.0) - (A - 1.0) * cosw - sq) / a0;
    }

    double process (double x, int channel) noexcept
    {
        const auto ch = (size_t) channel;
        const double y = b0 * x + z1[ch];
        z1[ch] = b1 * x - a1 * y + z2[ch];
        z2[ch] = b2 * x - a2 * y;
        return y;
    }

    void resetState() noexcept
    {
        z1.fill (0.0);
        z2.fill (0.0);
    }

    void invalidate() noexcept { lastType = 0; }

private:
    double b0 = 1.0, b1 = 0.0, b2 = 0.0, a1 = 0.0, a2 = 0.0;
    std::array<double, kMaxCh> z1 {}, z2 {};
    double lastFreq = -1.0, lastFs = -1.0, lastGain = 0.0;
    int lastType = 0;
};

//==============================================================================
// 90-degree phase-difference allpass pair ("IIR Hilbert transformer").
// |a + j b| is the instantaneous amplitude of the analytic signal: for a sine it is constant
// and independent of where the samples fall on the waveform. A plain sample-peak detector
// reads a different peak every half cycle, which creates non-harmonic GR modulation at high
// frequencies (that is what the 15 kHz aliasing test catches).
// Coefficients: elliptic halfband polyphase design (closed form, exact elliptic nome),
// frequency-shifted by fs/4. Sections (c - z^-2) / (1 - c z^-2); even coefficients in chain A,
// odd in chain B, chain B delayed by one sample.
class HilbertPair
{
public:
    static constexpr int kMaxCoefs = 20;

    // accurate band [fLow, fs/2 - fLow]. Error with 20 coefficients and fLow = 15 Hz:
    // 0.0012 deg at 96 kHz, 0.0136 deg at 768 kHz (192 kHz x 4).
    static void design (double fs, double fLow, int numCoefs, double* coefs)
    {
        const double t = std::clamp (2.0 * fLow / fs, 1.0e-7, 0.45);
        double k = std::tan ((1.0 - 2.0 * t) * kPi / 4.0);
        k *= k;
        auto ellipK = [] (double m)
        {
            double a = 1.0, b = std::sqrt (std::max (1.0e-300, 1.0 - m));
            for (int i = 0; i < 64 && std::abs (a - b) > 1.0e-15 * a; ++i)
            {
                const double an = 0.5 * (a + b);
                b = std::sqrt (a * b);
                a = an;
            }
            return kPi / (2.0 * a);
        };
        const double q = std::exp (-kPi * ellipK (1.0 - k * k) / ellipK (k * k));
        const int order = 2 * numCoefs + 1;
        for (int idx = 0; idx < numCoefs; ++idx)
        {
            const int c = idx + 1;
            double num = 0.0, den = 0.0;
            for (int i = 0, j = 1; i < 200; ++i, j = -j)
            {
                const double term = std::pow (q, (double) (i * (i + 1))) * std::sin ((i * 2 + 1) * c * kPi / order) * j;
                num += term;
                if (std::abs (term) < 1.0e-30)
                    break;
            }
            for (int i = 1, j = -1; i < 200; ++i, j = -j)
            {
                const double term = std::pow (q, (double) (i * i)) * std::cos (i * 2 * c * kPi / order) * j;
                den += term;
                if (std::abs (term) < 1.0e-30)
                    break;
            }
            const double ww = num * std::pow (q, 0.25) / (den + 0.5);
            const double w2 = ww * ww;
            const double x = std::sqrt ((1.0 - w2 * k) * (1.0 - w2 / k)) / (1.0 + w2);
            coefs[idx] = (1.0 - x) / (1.0 + x);
        }
    }

    void setCoefficients (const double* c, int n) noexcept
    {
        numCoefs = std::clamp (n, 2, kMaxCoefs);
        nA = (numCoefs + 1) / 2;
        nB = numCoefs / 2;
        for (int i = 0; i < numCoefs; ++i)
            (i % 2 == 0 ? cA[i / 2] : cB[i / 2]) = c[i];
        resetState();
    }

    void resetState() noexcept
    {
        for (auto& s : st)
            s = Chan();
    }

    // returns |analytic signal|^2
    double power (double x, int ch) noexcept
    {
        Chan& s = st[(size_t) ch];
        double a = x, b = x;
        for (int i = 0; i < nA; ++i)
        {
            const double y = cA[i] * (a + s.ay2[i]) - s.ax2[i];
            s.ax2[i] = s.ax1[i];
            s.ax1[i] = a;
            s.ay2[i] = s.ay1[i];
            s.ay1[i] = y;
            a = y;
        }
        for (int i = 0; i < nB; ++i)
        {
            const double y = cB[i] * (b + s.by2[i]) - s.bx2[i];
            s.bx2[i] = s.bx1[i];
            s.bx1[i] = b;
            s.by2[i] = s.by1[i];
            s.by1[i] = y;
            b = y;
        }
        const double bd = s.bDelay;
        s.bDelay = b;
        return a * a + bd * bd;
    }

private:
    struct Chan
    {
        double ax1[kMaxCoefs / 2] {}, ax2[kMaxCoefs / 2] {}, ay1[kMaxCoefs / 2] {}, ay2[kMaxCoefs / 2] {};
        double bx1[kMaxCoefs / 2] {}, bx2[kMaxCoefs / 2] {}, by1[kMaxCoefs / 2] {}, by2[kMaxCoefs / 2] {};
        double bDelay = 0.0;
    };
    double cA[kMaxCoefs / 2] {}, cB[kMaxCoefs / 2] {};
    int numCoefs = 2, nA = 1, nB = 1;
    std::array<Chan, kMaxCh> st {};
};

// 5 Hz one-pole DC blocker applied only to generated even-order distortion, so the
// main signal path itself stays perfectly linear-phase (dry/wet alignment is untouched).
class DcBlocker
{
public:
    void prepare (double fs) noexcept
    {
        r = 1.0 - 2.0 * kPi * 5.0 / fs;
        resetState();
    }
    void resetState() noexcept
    {
        x1.fill (0.0);
        y1.fill (0.0);
    }
    double process (double x, int channel) noexcept
    {
        const auto ch = (size_t) channel;
        const double y = x - x1[ch] + r * y1[ch];
        x1[ch] = x;
        y1[ch] = y;
        return y;
    }

private:
    double r = 0.999;
    std::array<double, kMaxCh> x1 {}, y1 {};
};

//==============================================================================
// FET stage: fixed threshold, INPUT drives into it (like a real 1176),
// attack 20-800 us, release 50-1100 ms, 4/8/12/20/All ratios.
class FetStage
{
public:
    struct Settings
    {
        float inDb = 6.0f, outDb = 0.0f, attackMs = 0.2f, releaseMs = 150.0f;
        int ratio = 0;            // 0..4 = 4, 8, 12, 20, All
        float scHz = 80.0f, color = 0.3f, link = 1.0f;
    };

    static constexpr float kThresholdDb = -12.0f;

    void prepare (double baseRate)
    {
        baseFs = baseRate;
        // the HF analytic path runs at the base rate (it is an envelope; interpolated per sample)
        double coefs[HilbertPair::kMaxCoefs] {};
        HilbertPair::design (baseFs, kHilbertLowHz, kHilbertCoefs, coefs);
        hil.setCoefficients (coefs, kHilbertCoefs);
        hf1.setHighPass (baseFs, 1000.0, 0.5411961); // 4th-order Butterworth high-pass at 1 kHz
        hf2.setHighPass (baseFs, 1000.0, 1.3065630);
        setOversampling (1);
        resetState();
    }

    // RT-safe: keeps the envelope state (GR, memory); no allocation
    void setOversampling (int log2Factor) noexcept
    {
        osIndex = std::clamp (log2Factor, 0, kNumOsFactors - 1);
        fs = baseFs * (1 << osIndex);
        decim = 1 << osIndex;
        ctlCount = 0;
        sc.invalidate();
        sc.resetState();
        holdLen = std::max (1, (int) std::lround (0.002 * fs));
        dc.prepare (fs);
        smoothK = 1.0f - onePole (0.02f, fs);
        memRiseK = 1.0f - onePole (2.0f, fs);
        memFallK = 1.0f - onePole (4.0f, fs);
        setParams (settings);
    }

    void setParams (const Settings& s) noexcept
    {
        settings = s;
        static constexpr float ratios[] = { 4.0f, 8.0f, 12.0f, 20.0f, 20.0f };
        // knee gets harder as the ratio rises (spec 2.1)
        static constexpr float knees[]  = { 6.0f, 4.0f, 3.0f, 2.0f, 2.0f };

        const int idx = std::clamp (s.ratio, 0, 4);
        allButtons = (idx == 4);
        ratio = ratios[idx];
        knee  = knees[idx];
        inTarget  = dbToGain (s.inDb);
        outTarget = dbToGain (s.outDb);
        // All-buttons: attack 0.6x and release 0.7x of the knob (faster, pumping; documented in DECISIONS.md)
        const float atkSec = std::clamp (s.attackMs, 0.02f, 0.8f) * 0.001f * (allButtons ? 0.6f : 1.0f);
        const float relSec = std::clamp (s.releaseMs, 50.0f, 1100.0f) * 0.001f * (allButtons ? 0.7f : 1.0f);
        atkK = 1.0f - onePole (atkSec, fs);
        relRate = (float) (1.0 / (relSec * fs));
        sc.setHighPass (fs, std::clamp (s.scHz, 20.0f, 300.0f));
        colorTarget = std::clamp (s.color, 0.0f, 1.0f);
        link = std::clamp (s.link, 0.0f, 1.0f);
    }

    void snap() noexcept
    {
        inGain = inTarget;
        outGain = outTarget;
        color = colorTarget;
    }

    void resetState() noexcept
    {
        hil.resetState();
        sc.resetState();
        hf1.resetState();
        hf2.resetState();
        dc.resetState();
        gr.fill (0.0f);
        memory.fill (0.0f);
        held.fill (0.0f);
        holdCount.fill (0);
        hCur.fill (0.0f);
        hStep.fill (0.0f);
        ctlCount = 0;
    }

    // processes one multichannel frame in place, returns the largest channel GR in dB
    float processFrame (float* x, int numCh) noexcept
    {
        return numCh >= 2 ? frame<2> (x) : frame<1> (x);
    }

    float grDb (int ch) const noexcept { return gr[(size_t) ch]; }
    double sampleRate() const noexcept { return fs; }

private:
    // channel count as a compile-time constant: loops unroll and the two channels'
    // dependency chains interleave (measured ~10 % less CPU for the stereo stage)
    template <int numCh>
    float frame (float* x) noexcept
    {
        inGain  += (inTarget - inGain) * smoothK;
        outGain += (outTarget - outGain) * smoothK;
        color   += (colorTarget - color) * smoothK;
        if (std::abs (color - colorTarget) < 1.0e-6f)
            color = colorTarget;

        // Detector = max(2 ms peak-hold of |x|, |analytic signal of the >1 kHz band|).
        // - |x| makes transients instant (20 us attack is real).
        // - the 2 ms hold bridges the gaps between waveform peaks, so the level is steady from
        //   the first peak on: attack/release behave like the knob says on tones >= 250 Hz.
        // - with only a few samples per cycle the sampled peak depends on where the samples
        //   fall (up to -1.3 dB at 15 kHz / 88.2 kHz) and the GR would "beat" -> non-harmonic
        //   sidebands. The analytic magnitude is sample-phase independent; it is only needed
        //   (and only used) above ~1 kHz, where the Hilbert pair can be short and fast.
        float env[kMaxCh] {};
        double sv[kMaxCh] {};
        float envMax = 0.0f;
        for (int c = 0; c < numCh; ++c)
        {
            x[c] *= inGain;
            sv[c] = sc.process (x[c], c);
        }
        if (--ctlCount <= 0)
        {
            ctlCount = decim;
            for (int c = 0; c < numCh; ++c)
            {
                const float h = (float) std::sqrt (hil.power (hf2.process (hf1.process (sv[c], c), c), c));
                hStep[(size_t) c] = (h - hCur[(size_t) c]) / (float) decim;
            }
        }
        for (int c = 0; c < numCh; ++c)
        {
            const double s = sv[c];
            const float a = (float) std::abs (s);
            float& hv = held[(size_t) c];
            int& hc = holdCount[(size_t) c];
            if (a >= hv || hc <= 0)
            {
                hv = a;
                hc = holdLen;
            }
            else
            {
                --hc;
            }
            hCur[(size_t) c] += hStep[(size_t) c];
            env[c] = std::max (hv, hCur[(size_t) c]);
            envMax = std::max (envMax, env[c]);
        }

        float grMax = 0.0f;
        for (int c = 0; c < numCh; ++c)
        {
            const float e = envMax + (env[c] - envMax) * (1.0f - link);
            // far below the knee nothing happens: skip the log/curve (quiet passages, pauses)
            const float target = e < kQuietLevel ? 0.0f : curve (gainToDb (e));

            // program-dependent release: memory builds up over ~2 s of heavy (>2 dB, full at
            // 10 dB) GR and slows the release by up to +40 % (spec 2.1); decays over ~4 s.
            float& g = gr[(size_t) c];
            float& m = memory[(size_t) c];
            if (target > g)
                g += (target - g) * atkK;
            else
                g += (target - g) * oneMinusExpSmall (relRate / (1.0f + 0.4f * m));
            // memory follows the compression demand (target), so it stops charging the moment
            // the signal drops - otherwise a long release would keep slowing itself down
            const float w = std::clamp ((target - 2.0f) * 0.125f, 0.0f, 1.0f);
            m += (w - m) * (w > m ? memRiseK : memFallK);

            float y = g > 0.0f ? x[c] * dbToGain (-g) : x[c];

            if (color > 0.0f)
                y += distortion (y, g, c);
            else
                (void) dc.process (0.0, c); // keep the blocker state decaying

            x[c] = y * outGain;
            grMax = std::max (grMax, g);
        }
        return grMax;
    }

    float curve (float levelDb) const noexcept
    {
        float g = gainReductionDb (levelDb, kThresholdDb, ratio, knee);
        if (allButtons)
        {
            // over-compression: above threshold + 10 dB the output level falls by 0.1 dB per dB
            // (soft 6 dB knee, capped at 6 dB extra) - the "nuke" bump of all-buttons-in.
            g += std::min (6.0f, gainReductionDb (levelDb, kThresholdDb + 10.0f, 1.0f / (1.0f - 0.15f), 6.0f));
        }
        return g;
    }

    // FET gain-element distortion: 2nd + 3rd harmonic only (a cubic polynomial cannot create
    // the higher harmonics that would fold back as aliasing). Amount rises with GR like the FET's
    // channel non-linearity, scaled by Color; All-buttons 2.5x. The polynomial input is bounded
    // (s / sqrt(1 + s^2/16)) and the amount capped so the transfer stays monotonic.
    float distortion (float y, float grDb, int c) noexcept
    {
        constexpr float T = 0.251188643f;          // threshold amplitude (-12 dBFS)
        constexpr float k2 = 0.0129f, k3 = 0.0208f; // calibrated: ~0.45 % H2 + ~0.5 % H3 at 10 dB GR, Color 50
        const float amount = std::min (4.0f, color * (0.08f + std::min (grDb, 20.0f) * 0.1f) * (allButtons ? 2.5f : 1.0f));
        const float s = y * (1.0f / T);
        const float sb = s / std::sqrt (1.0f + s * s * (1.0f / 16.0f));
        const float even = (float) dc.process ((double) (amount * k2 * sb * sb), c);
        const float odd = -amount * k3 * sb * sb * sb;
        return T * (even + odd);
    }

    // 500 Hz lower edge, 12 coefficients: <= 0.0001 deg at 44.1 kHz, <= 0.003 deg at 192 kHz
    static constexpr int kHilbertCoefs = 12;
    static constexpr double kHilbertLowHz = 500.0;
    // -12 dBFS threshold - 4 dB (half the widest knee + margin): curve() is exactly 0 below this
    static constexpr float kQuietLevel = 0.158489319f;
    Settings settings;
    double baseFs = 44100.0, fs = 88200.0;
    int osIndex = 1, decim = 2, ctlCount = 0;
    std::array<float, kMaxCh> hCur {}, hStep {};
    Biquad sc, hf1, hf2;
    HilbertPair hil;
    DcBlocker dc;
    int holdLen = 176;
    std::array<float, kMaxCh> held {};
    std::array<int, kMaxCh> holdCount {};
    float ratio = 4.0f, knee = 6.0f, atkK = 1.0f, relRate = 0.0f, link = 1.0f;
    float memRiseK = 0.0f, memFallK = 0.0f;
    float inGain = 1.0f, inTarget = 1.0f, outGain = 1.0f, outTarget = 1.0f;
    float color = 0.0f, colorTarget = 0.0f, smoothK = 0.001f;
    std::array<float, kMaxCh> gr {}, memory {};
    bool allButtons = false;
};

//==============================================================================
// Opto stage (LA-2A-style): power detector (analytic signal, ~2 ms), ~10 ms attack,
// two-stage release: 60 % of the GR follows a fast 40 ms release, 40 % a slow "light cell"
// release of 0.5-5 s whose length grows with how long and how hard it has been compressing.
//
// The detector and ballistics run at the BASE rate (every 1/2/4 oversampled frames) and the
// gain reduction is interpolated linearly in dB per oversampled sample. Nothing in an opto
// moves faster than ~8 ms, so this halves the stage's CPU at 2x and makes the opto behave
// identically at 1x, 2x and 4x. The make-up/tube stage runs at the oversampled rate.
class OptoStage
{
public:
    struct Settings
    {
        float peakReduction = 30.0f, gainDb = 0.0f;
        bool limit = false;
        float scHz = 80.0f, color = 0.3f, link = 1.0f, emphasis = 0.0f;
    };

    static constexpr float kThresholdDb = -10.0f;

    void prepare (double baseRate)
    {
        baseFs = baseRate;
        double coefs[HilbertPair::kMaxCoefs] {};
        HilbertPair::design (baseFs, 15.0, kHilbertCoefs, coefs);
        hil.setCoefficients (coefs, kHilbertCoefs);
        detK     = 1.0f - onePole (kDetectorSec, baseFs);
        atkK     = 1.0f - onePole (kAttackSec, baseFs);
        relFastK = 1.0f - onePole (kFastReleaseSec, baseFs);
        memRiseK = 1.0f - onePole (4.0f, baseFs);
        memFallK = 1.0f - onePole (8.0f, baseFs);
        invFs    = (float) (1.0 / baseFs);
        setOversampling (1);
        resetState();
    }

    void setOversampling (int log2Factor) noexcept
    {
        osIndex = std::clamp (log2Factor, 0, kNumOsFactors - 1);
        fs = baseFs * (1 << osIndex);
        decim = 1 << osIndex;
        ctlCount = 0;
        dc.prepare (fs);
        smoothK = 1.0f - onePole (0.02f, fs);
        setParams (settings);
    }

    void setParams (const Settings& s) noexcept
    {
        settings = s;
        prGainDb     = std::clamp (s.peakReduction, 0.0f, 100.0f) * 0.4f; // 0..40 dB into the cell
        ratio        = s.limit ? 10.0f : 3.0f;
        knee         = s.limit ? 6.0f : 12.0f;
        makeupTarget = dbToGain (s.gainDb);
        sc.setHighPass (baseFs, std::clamp (s.scHz, 20.0f, 300.0f));
        // R37-style HF emphasis: up to +12 dB high shelf at 3 kHz in the detector only
        const double emphDb = 12.0 * std::clamp (s.emphasis, 0.0f, 1.0f);
        if (emphDb > 0.0 && ! useShelf)
            shelf.resetState();
        useShelf = emphDb > 0.0;
        shelf.setHighShelf (baseFs, 3000.0, emphDb);
        colorTarget = std::clamp (s.color, 0.0f, 1.0f);
        link = std::clamp (s.link, 0.0f, 1.0f);
    }

    void snap() noexcept
    {
        makeup = makeupTarget;
        color = colorTarget;
    }

    void resetState() noexcept
    {
        hil.resetState();
        sc.resetState();
        shelf.resetState();
        dc.resetState();
        ms.fill (0.0f);
        grFast.fill (0.0f);
        grSlow.fill (0.0f);
        memory.fill (0.0f);
        grCur.fill (0.0f);
        grStep.fill (0.0f);
        gainCur.fill (1.0f);
        gainStep.fill (0.0f);
        ctlCount = 0;
    }

    float processFrame (float* x, int numCh) noexcept
    {
        return numCh >= 2 ? frame<2> (x) : frame<1> (x);
    }

    double sampleRate() const noexcept { return fs; }

private:
    template <int numCh>
    float frame (float* x) noexcept
    {
        makeup += (makeupTarget - makeup) * smoothK;
        color  += (colorTarget - color) * smoothK;
        if (std::abs (color - colorTarget) < 1.0e-6f)
            color = colorTarget;

        if (--ctlCount <= 0)
        {
            ctlCount = decim;
            control (x, numCh);
        }

        // GR (dB, for meters/tests) and the linear gain are both interpolated linearly
        // between base-rate control points; no transcendental per oversampled sample
        float grMax = 0.0f;
        for (int c = 0; c < numCh; ++c)
        {
            float& g = grCur[(size_t) c];
            g += grStep[(size_t) c];
            gainCur[(size_t) c] += gainStep[(size_t) c];
            float y = x[c] * gainCur[(size_t) c] * makeup;
            if (color > 0.0f)
                y += tube (y, c);
            else
                (void) dc.process (0.0, c);
            x[c] = y;
            grMax = std::max (grMax, g);
        }
        return grMax;
    }

    // one base-rate step of detector + ballistics; sets the per-frame GR interpolation
    void control (const float* x, int numCh) noexcept
    {
        float msMax = 0.0f;
        for (int c = 0; c < numCh; ++c)
        {
            double v = sc.process (x[c], c);
            if (useShelf)
                v = shelf.process (v, c);
            const float p = (float) hil.power (v, c);
            ms[(size_t) c] += (p - ms[(size_t) c]) * detK;
            msMax = std::max (msMax, ms[(size_t) c]);
        }

        for (int c = 0; c < numCh; ++c)
        {
            const float p = msMax + (ms[(size_t) c] - msMax) * (1.0f - link);
            const float target = gainReductionDb (powerToDb (p) + prGainDb, kThresholdDb, ratio, knee);

            float& f = grFast[(size_t) c];
            float& sl = grSlow[(size_t) c];
            float& m = memory[(size_t) c];
            // slow tail 0.5 s .. 5 s, set per step from the light-cell memory (no stepping)
            const float tauSlow = 0.5f + 4.5f * m;
            f  += (target - f)  * (target > f  ? atkK : relFastK);
            sl += (target - sl) * (target > sl ? atkK : oneMinusExpSmall (invFs / tauSlow));
            const float g = kFastShare * f + (1.0f - kFastShare) * sl;
            // memory follows the compression demand: full build-up at 10 dB
            const float w = std::clamp (target * 0.1f, 0.0f, 1.0f);
            m += (w - m) * (w > m ? memRiseK : memFallK);
            const float inv = 1.0f / (float) decim;
            grStep[(size_t) c] = (g - grCur[(size_t) c]) * inv;
            gainStep[(size_t) c] = (dbToGain (-g) - gainCur[(size_t) c]) * inv;
        }
    }

    // Tube make-up colour: mostly 2nd harmonic (plus a little 3rd), level dependent, DC-free.
    // Referenced to -6 dBFS; Color 50 gives ~0.3 % H2 at -12 dBFS out, ~1 % near 0 dBFS.
    float tube (float y, int c) noexcept
    {
        constexpr float R = 0.5f;
        const float s = y * (1.0f / R);
        const float sb = s / std::sqrt (1.0f + s * s * (1.0f / 9.0f));
        const float a = color * 0.048f;
        const float even = (float) dc.process ((double) (a * sb * sb), c);
        const float odd = -a * 0.15f * sb * sb * sb;
        return R * (even + odd);
    }

    static constexpr float kDetectorSec = 0.002f;
    static constexpr float kAttackSec = 0.0085f;
    static constexpr float kFastReleaseSec = 0.040f;
    static constexpr float kFastShare = 0.6f;
    // power is averaged (2 ms + 8.5 ms attack): 8 coefficients (<= ~1 deg) is ample here
    static constexpr int kHilbertCoefs = 8;

    Settings settings;
    double baseFs = 44100.0, fs = 88200.0;
    int osIndex = 1, decim = 2, ctlCount = 0;
    bool useShelf = false;
    Biquad sc, shelf;
    HilbertPair hil;
    DcBlocker dc;
    float prGainDb = 12.0f, ratio = 3.0f, knee = 12.0f, link = 1.0f;
    float makeup = 1.0f, makeupTarget = 1.0f, color = 0.0f, colorTarget = 0.0f, smoothK = 0.001f;
    float detK = 0.0f, atkK = 0.0f, relFastK = 0.0f, memRiseK = 0.0f, memFallK = 0.0f, invFs = 0.0f;
    std::array<float, kMaxCh> ms {}, grFast {}, grSlow {}, memory {}, grCur {}, grStep {};
    std::array<float, kMaxCh> gainCur { 1.0f, 1.0f }, gainStep {};
};
} // namespace duo
