# Engineering decisions

Each entry: what was decided, the measurement that justified it, and any spec deviation.
All numbers come from `DuoDspTests` (see `tests/report.md`).

## D1. The whole signal chain lives in JUCE-free headers
`Source/Engine.h` (+ `Dsp.h`, `Oversampling.h`, `Presets.h`) contains oversampling, both stages,
mix, smoothing, metering and presets. `PluginProcessor` only maps parameters and calls `Engine::process`.
**Why:** the test suite (plain g++; `Source/Dsp.h` stays JUCE-free by design) then measures exactly the code that ships,
including oversampling and mix. The draft kept those in the JUCE processor, where they couldn't be tested.

## D2. Linear-phase FIR oversampling instead of JUCE's IIR halfbands
Polyphase Kaiser halfbands, 1x/2x/4x, integer latency (44.1/48 kHz: 71 samples at 2x, 79 at 4x;
>= 88.2 kHz: 25 / 33). Every filter's stopband is **measured** at design time (the Kaiser length formula
alone gave -97.5 dB for a "100 dB" design) and verified by tests: >= 100 dB stopband, <= 0.0002 dB
passband ripple, wet path flat to 0.0003 dB from 20 Hz to 20 kHz.
**Why:** a linear-phase chain is a pure delay, so the dry path can be delayed by exactly the reported
latency (see D3), and parallel mixing has no phase smear.

## D3. Mix happens at the base rate against an exactly delayed dry signal
The spec suggested keeping the mix in the oversampled domain, or relaxing the null test to -60 dB.
Instead, the dry signal goes through an integer delay line equal to the reported latency and never
passes the filters. **Result:** Mix 0 % is bit-exact with the delayed input (residual 0, i.e. below
-100 dBFS by an unlimited margin). The draft measured -22 dBFS. The Mix 50 % residual against the
delayed dry with the stages off is <= -116 dBFS (<18 kHz), so parallel blends don't comb-filter.
No spec target was relaxed.

## D4. FET detector: max(2 ms peak-hold of |x|, analytic magnitude of the >1 kHz band)
- A plain sample-peak detector (draft) reads a different peak every half cycle when there are few samples
  per cycle (up to -1.3 dB at 15 kHz / 88.2 kHz). The release then makes GR "beat", producing
  non-harmonic sidebands (estimated about -51 dBFS). That fails the 4x aliasing target (-85 dBFS) no matter how
  good the filters are.
- The magnitude of the analytic signal (IIR 90-degree allpass pair, elliptic closed-form design with exact
  elliptic nome) doesn't depend on sample phase. A full-band version (15 Hz edge) was tried first and
  measured: about 1.3 ms of envelope lag at 1 kHz and +6 % overshoot/ringing after a level step. That
  made the 800 us attack measure 1150 us (+44 %).
- Final design: the |x| peak path with a 2 ms hold (instant, gap-free from the first waveform peak), plus the
  analytic magnitude computed only on a 4th-order 1 kHz high-passed copy (12 coefficients, 500 Hz edge,
  phase error <= 0.0005 degrees at 88.2 kHz). That path runs at the base rate and is interpolated (D7).
- **Measured:** attack 20 us -> 20.3-20.6 us, 800 us -> 802-804 us; release 50 ms -> 55.6 ms,
  1100 ms -> 1176 ms (both within +-20 %; the excess is the 2 ms hold plus memory build-up during the
  0.3 s hold). Aliasing (15 kHz, -3 dBFS, 20:1, Color 100) at 44.1 kHz: -84.5 dBFS at 2x
  (target < -70), -165 dBFS at 4x (target < -85).

## D5. Ballistics time convention in the tests
A stage output sample `j` frames after a step represents the state at the end of that sample period,
i.e. time `(j+1)/fs`. This is the zero-order-hold discretisation of a continuous RC, under which a pure
one-pole with time constant tau measures exactly tau. Ballistics are measured on the stage at its 2x
internal rate with a sample-exact step, because a step that has passed the 44.1 kHz input band limit cannot
rise faster than ~23 us, which would make a 20 us attack unmeasurable. Everything else is measured through
the complete engine.

## D6. FET colour = GR-scaled 2nd + 3rd harmonic polynomial (no tanh)
A cubic polynomial can't create harmonics above the 3rd, so the colour itself doesn't feed the aliasing
measured in D4. The even term is DC-blocked (5 Hz) on its own, so the main path stays linear-phase. The amount
rises with GR (0.08 + GR/10) and Color; All-buttons is 2.5x. The amount is capped so the transfer is
provably monotonic. **Measured:** THD at 0 dB GR / Color 0 = 0.000006 %; 10 dB GR / Color 50 = 0.66 %
(target 0.3-1.5 %), 99.8 % of the distortion power is in H2+H3; All mode = 1.11 % (1.68x the 4:1 value).

## D7. Opto detector + ballistics run at the base rate; gain interpolated per oversampled sample
Nothing in an opto moves faster than ~8 ms. Running the detector at the base rate halves the stage's CPU
at 2x, and the opto behaves identically at 1x/2x/4x. The GR (dB, for meters) and the linear gain are
interpolated linearly between control points. **Measured:** release smoothness residual 0.006
(target < 0.01). No stepping (the draft updated the release every 32 samples).

## D8. Opto release model
60 % of the GR follows a 40 ms release and 40 % a light-cell release of 0.5-5 s. The light-cell time grows
with a memory that charges (4 s) from the compression *demand* and discharges over 8 s. A first version
charged from the current GR, so the memory kept charging during the release and lengthened it. The FET
program-dependent release had the same flaw, which measured +20 % at 1100 ms. Both were fixed.
**Measured:** attack 9.85 ms (~10 ms); 50 % recovery after 2 s of 10 dB GR = 71 ms (40-80);
tail time constant 1.9 s (0.5-5); 90 % recovery after 10 s vs 1 s hold = 2.5x (>= 1.5).

## D9. FET All-buttons curve
Ratio 20 with a 2 dB knee, plus an over-compression term above threshold + 10 dB (soft 6 dB knee):
output falls 0.1 dB per dB of further input, capped at 6 dB extra. Attack is 0.6x and release 0.7x the knob
setting (faster, more pumping), and the colour is 2.5x. **Measured:** the curve peaks 8-12 dB above threshold
and drops 0.6-0.7 dB to 0 dBFS input; attack is 0.60x of 20:1 at the same knob.

## D10. Program-dependent FET release
Memory charges over 2 s from the demand (0 at 2 dB, full at 10 dB GR) and slows release by up to +40 %.
**Measured:** 1.29x after 6 s at 16.5 dB GR versus after 0.3 s (target 1.15-1.40).

## D11. Base rates >= 64 kHz use a decimator whose stopband starts at Nyquist
With halfband decimators, anything in the transition band around Nyquist aliases. At 88.2 kHz the 3rd
harmonic of a 15 kHz tone (45 kHz) sits exactly there: measured -65.8 dBFS alias at 43.2 kHz, the only
failure in the extended test matrix (the spec condition, 44.1 kHz, passed). At >= 64 kHz the passband
doesn't need to reach near Nyquist, so the final decimator is a full low-pass with passband 0.30 fs
(26 kHz at 88.2 kHz) and stopband from 0.5 fs (>= 100 dB). **Measured:** 88.2 kHz alias < -150 dBFS.
At 44.1/48 kHz the halfband stays (20 kHz passband is needed there). Bonus: lower latency (25 samples)
and lower CPU at high rates.

## D12. CPU
Initial new-engine build: 2.59 % (target < 2 %). Steps, each measured: control-rate opto (D7) and FET HF
path; channel count as a compile-time constant; no exp for the opto gain; skip log/curve below the knee;
8-coefficient opto Hilbert; SC-listen filters only while listening. **Final: 1.69 %** of one core
(48 kHz / 128 / stereo / 2x, tested on an 8-core Zen 3 laptop), silence costs 0.82x of program (denormal-safe).

## D13. Oversampling switching
All filters for all factors are prepared up front, so switching is RT-safe. The engine fades out over 3 ms,
switches at a chunk boundary, then fades back in. The new latency is reported to the host from a 20 Hz
message-thread timer: `setLatencySamples` never runs on the audio thread. Tested: random switching gives
no NaN/Inf, peak output stays bounded, and after switching the impulse lands exactly on the reported latency.

## D14. Factory preset gain staging
Presets are tuned with the engine on a vocal-like test signal (-18 dBFS RMS, peaks -4 dBFS).
INPUT sets the FET peak GR (lead ~7 dB, double ~10, parallel smash ~18, gentle ~3, spoken ~5).
FET OUTPUT recovers it the way it's used on a 1176. PEAK REDUCTION sets the average opto GR (3-4.5 dB)
and Opto GAIN makes that up. **Measured:** output within +-0.6 dB of input loudness for every preset at
every rate, so switching presets compares sound, not level.

## D15. Program changes from the audio thread
VST3 hosts can send program changes on the audio thread. `setCurrentProgram` therefore only
touches atomics and parameter values there. The preset *name* (a `juce::String`, which allocates)
is resolved on the message thread by the processor timer. All other access to the name is under
a lock that the audio thread never takes. Tested: a program change from a separate thread applies
its values immediately, and the name follows within one timer tick.

## D16. State restore re-applies every parameter explicitly
pluginval (strictness 10) intermittently reported "FET On not restored" (expected 1, got 0.64).
Cause: `APVTS::replaceState` skips a parameter whose snapped value already equals the stored value,
so a raw host value (0.64 on a bool) survives the restore. Every restore path (session, user preset,
A/B) now re-applies each stored value. Regression test: 50 random rounds of raw-value save/scramble/restore.
It fails without the fix and passes with it. pluginval then passed 3/3 runs with different random seeds.

## D17. Stage-order switch is faded like the oversampling switch
The two orders produce different gain trajectories, so an instant swap can click. A change now
fades out over 3 ms, swaps at a chunk boundary and fades back in. **Measured:** the largest sample
step across the switch is 1.03x the steady tone's own largest step.

## D18. Auto makeup is capped at +24 dB
With extreme settings (~60 dB of GR) uncapped makeup would add 60 dB. **Measured cap:** 23.99 dB.
