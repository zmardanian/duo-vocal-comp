# Duo Vocal Comp - DSP test report

Generated 2026-10-13 13:43 by `DuoDspTests` in 46.544 s. Stage ballistics are measured on the stage at the default 2x internal rate with a sample-exact step; everything else runs through the complete engine (oversampling, mix, output). Build: Release.

**Result: ALL PASS** - 1299 checks passed, 0 failed (every check runs once per sample rate / channel configuration).

## Summary (worst case over all configurations)

| Section | Check | Target | Worst measured | at | Pass |
|---|---|---|---|---|---|
| 1 Static curve | FET 4:1: slope below threshold (-60..-44 dBFS) | 0.9800 .. 1.020 dB/dB | 1.0000 dB/dB | 96 k mono | PASS 10/10 |
| 1 Static curve | FET 4:1: ratio above threshold (-12..0 dBFS in) | 3.600 .. 4.400 :1 | 4.013 :1 | 96 k mono | PASS 10/10 |
| 1 Static curve | FET 8:1: slope below threshold (-60..-44 dBFS) | 0.9800 .. 1.020 dB/dB | 1.0000 dB/dB | 96 k mono | PASS 10/10 |
| 1 Static curve | FET 8:1: ratio above threshold (-12..0 dBFS in) | 7.200 .. 8.800 :1 | 8.134 :1 | 96 k mono | PASS 10/10 |
| 1 Static curve | FET 12:1: slope below threshold (-60..-44 dBFS) | 0.9800 .. 1.020 dB/dB | 1.0000 dB/dB | 96 k mono | PASS 10/10 |
| 1 Static curve | FET 12:1: ratio above threshold (-12..0 dBFS in) | 10.800 .. 13.200 :1 | 12.274 :1 | 96 k mono | PASS 10/10 |
| 1 Static curve | FET 20:1: slope below threshold (-60..-44 dBFS) | 0.9800 .. 1.020 dB/dB | 1.0000 dB/dB | 96 k mono | PASS 10/10 |
| 1 Static curve | FET 20:1: ratio above threshold (-12..0 dBFS in) | 18.000 .. 22.000 :1 | 19.124 :1 | 192 k mono | PASS 10/10 |
| 1 Static curve | FET All: slope below threshold (-60..-44 dBFS) | 0.9800 .. 1.020 dB/dB | 1.0000 dB/dB | 96 k mono | PASS 10/10 |
| 1 Static curve | FET All: over-compression, drop from curve peak to 0 dBFS in | > 0.3000 dB | 0.7314 dB | 96 k mono | PASS 10/10 |
| 1 Static curve | FET All: curve peak position above threshold | 4.000 .. 16.000 dB | 8.000 dB | 44.1 k mono | PASS 10/10 |
| 1 Static curve | FET All: drop stays 'slight' (peak to 0 dBFS in) | < 4.000 dB | 0.7479 dB | 192 k mono | PASS 10/10 |
| 1 Static curve | Opto Compress: slope below threshold (-60..-44 dBFS) | 0.9800 .. 1.020 dB/dB | 1.0000 dB/dB | 48 k mono | PASS 10/10 |
| 1 Static curve | Opto Compress: ratio above threshold (-12..0 dBFS in) | 2.700 .. 3.300 :1 | 2.999 :1 | 192 k mono | PASS 10/10 |
| 1 Static curve | Opto Limit: slope below threshold (-60..-44 dBFS) | 0.9800 .. 1.020 dB/dB | 1.0000 dB/dB | 48 k mono | PASS 10/10 |
| 1 Static curve | Opto Limit: ratio above threshold (-12..0 dBFS in) | 9.000 .. 11.000 :1 | 10.000 :1 | 96 k mono | PASS 10/10 |
| 2 FET attack/release | attack 20.000 us: time to 63 % GR | 16.000 .. 24.000 us | 20.642 us | 44.1 k mono | PASS 10/10 |
| 2 FET attack/release | attack 800.0 us: time to 63 % GR | 640.0 .. 960.0 us | 803.8 us | 48 k mono | PASS 10/10 |
| 2 FET attack/release | release 50.000 ms: time to 63 % recovery | 40.000 .. 60.000 ms | 55.574 ms | 88.2 k mono | PASS 10/10 |
| 2 FET attack/release | release 1100.0 ms: time to 63 % recovery | 880.0 .. 1320.0 ms | 1173.4 ms | 192 k mono | PASS 10/10 |
| 2 FET attack/release | program-dependent: GR during hold | (info) | 16.507 .. 16.532 dB | all | - |
| 2 FET attack/release | program-dependent: release after 6 s vs 0.3 s of >10 dB GR | 1.150 .. 1.400 x | 1.295 x | 48 k mono | PASS 10/10 |
| 2 FET attack/release | All mode: attack time relative to 20:1 (800 us knob) | < 0.8000 x | 0.6015 x | 192 k mono | PASS 10/10 |
| 3 Opto attack/release | attack: time to 63 % of 10 dB GR | 8.000 .. 12.000 ms | 9.771 ms | 192 k mono | PASS 10/10 |
| 3 Opto attack/release | release after 2 s of 10 dB GR: 50 % recovery | 40.000 .. 80.000 ms | 71.260 ms | 96 k mono | PASS 10/10 |
| 3 Opto attack/release | release tail time constant after 2 s (70->90 % recovery) | 0.5000 .. 5.000 s | 1.926 s | 48 k mono | PASS 10/10 |
| 3 Opto attack/release | 90 % recovery after 1 s of 10 dB GR | (info) | 1.918 .. 1.931 s | all | - |
| 3 Opto attack/release | 90 % recovery after 10 s of 10 dB GR | (info) | 4.864 .. 4.874 s | all | - |
| 3 Opto attack/release | light-cell memory: 90 % recovery 10 s hold / 1 s hold | > 1.500 x | 2.523 x | 88.2 k mono | PASS 10/10 |
| 3 Opto attack/release | release smoothness (slope residual vs slope, 10 s hold) | < 0.0100 ratio | 0.0055 ratio | 192 k mono | PASS 10/10 |
| 4 THD (1 kHz) | FET 0 dB GR, Color 0 | < 0.1000 % | 5.1e-06 % | 48 k mono | PASS 10/10 |
| 4 THD (1 kHz) | FET 10 dB GR, Color 50 (4:1) | 0.3000 .. 1.500 % | 0.6619 % | 192 k mono | PASS 10/10 |
| 4 THD (1 kHz) | FET 10 dB GR, Color 50: share of 2nd+3rd in distortion power | > 80.000 % | 99.830 % | 44.1 k mono | PASS 10/10 |
| 4 THD (1 kHz) | FET All 10 dB GR, Color 50 | (info) | 1.107 .. 1.110 % | all | - |
| 4 THD (1 kHz) | FET All vs 4:1 THD at 10 dB GR, Color 50 | > 1.250 x | 1.673 x | 96 k mono | PASS 10/10 |
| 4 THD (1 kHz) | Opto 10 dB GR, Color 0 (clean) | < 0.1000 % | 4.67e-05 % | 96 k mono | PASS 10/10 |
| 4 THD (1 kHz) | Opto 10 dB GR, Color 50 | (info) | 0.1348 .. 0.1348 % | all | - |
| 4 THD (1 kHz) | Opto Color 50: H2 / H3 (predominantly 2nd) | > 2.000 x | 118.6 x | 48 k mono | PASS 10/10 |
| 4 THD (1 kHz) | Opto Color 50: THD rises with level (0 dB vs -20 dB rel.) | > 1.300 x | 3.169 x | 96 k mono | PASS 10/10 |
| 4 THD (1 kHz) | Opto Color 100: DC offset | < -100.0 dBFS | -176.7 dBFS | 192 k mono | PASS 10/10 |
| 5 Aliasing (15 kHz, -3 dBFS, FET 20:1, Color 100) | FET only, 1x (no oversampling) | (info) | -117.4 .. -51.385 dBFS | all | - |
| 5 Aliasing (15 kHz, -3 dBFS, FET 20:1, Color 100) | FET only, 2x | < -70.000 dBFS | -84.383 dBFS | 48 k mono | PASS 10/10 |
| 5 Aliasing (15 kHz, -3 dBFS, FET 20:1, Color 100) | FET only, 4x | < -85.000 dBFS | -154.2 dBFS | 48 k mono | PASS 10/10 |
| 5 Aliasing (15 kHz, -3 dBFS, FET 20:1, Color 100) | FET + Opto (default), 1x (no oversampling) | (info) | -124.4 .. -54.586 dBFS | all | - |
| 5 Aliasing (15 kHz, -3 dBFS, FET 20:1, Color 100) | FET + Opto (default), 2x | < -70.000 dBFS | -91.428 dBFS | 48 k mono | PASS 10/10 |
| 5 Aliasing (15 kHz, -3 dBFS, FET 20:1, Color 100) | FET + Opto (default), 4x | < -85.000 dBFS | -135.6 dBFS | 44.1 k mono | PASS 10/10 |
| 6 Null / latency | Mix 0 %: residual vs input delayed by reported latency, 1x | < -100.0 dBFS | -600.0 dBFS | 44.1 k mono | PASS 10/10 |
| 6 Null / latency | reported latency matches factor, 1x | = 0.0000 samples | 0.0000 samples | 44.1 k mono | PASS 10/10 |
| 6 Null / latency | wet impulse peak offset - reported latency, 1x | = 0.0000 samples | 0.0000 samples | 44.1 k mono | PASS 10/10 |
| 6 Null / latency | Mix 50 %, stages off: residual vs delayed dry (<18 kHz), 1x | < -80.000 dBFS | -600.0 dBFS | 44.1 k mono | PASS 10/10 |
| 6 Null / latency | Mix 0 %: residual vs input delayed by reported latency, 2x | < -100.0 dBFS | -600.0 dBFS | 44.1 k mono | PASS 10/10 |
| 6 Null / latency | reported latency matches factor, 2x | = 0.0000 samples | 0.0000 samples | 44.1 k mono | PASS 10/10 |
| 6 Null / latency | wet impulse peak offset - reported latency, 2x | = 0.0000 samples | 0.0000 samples | 44.1 k mono | PASS 10/10 |
| 6 Null / latency | Mix 50 %, stages off: residual vs delayed dry (<18 kHz), 2x | < -80.000 dBFS | -126.7 dBFS | 96 k mono | PASS 10/10 |
| 6 Null / latency | Mix 0 %: residual vs input delayed by reported latency, 4x | < -100.0 dBFS | -600.0 dBFS | 44.1 k mono | PASS 10/10 |
| 6 Null / latency | reported latency matches factor, 4x | = 0.0000 samples | 0.0000 samples | 44.1 k mono | PASS 10/10 |
| 6 Null / latency | wet impulse peak offset - reported latency, 4x | = 0.0000 samples | 0.0000 samples | 44.1 k mono | PASS 10/10 |
| 6 Null / latency | Mix 50 %, stages off: residual vs delayed dry (<18 kHz), 4x | < -80.000 dBFS | -121.3 dBFS | 44.1 k mono | PASS 10/10 |
| 7 Silence / stability | zeros after +24 dBFS noise: max |out| after 1 s of silence | < 1e-07  | 4.01e-17  | 44.1 k mono | PASS 10/10 |
| 7 Silence / stability | zeros from fresh instance: max |out| | < 1e-07  | 0.0000  | 44.1 k mono | PASS 10/10 |
| 7 Silence / stability | random automation every block, +24 dBFS: all samples finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| 7 Silence / stability | random automation: peak output (sanity bound) | < 80.000 dBFS | 43.570 dBFS | 44.1 k stereo | PASS 10/10 |
| 7 Silence / stability | random automation then silence: max |out| in last 0.5 s | < 1e-07  | 2.8e-23  | 96 k mono | PASS 10/10 |
| 8 Block-size invariance | max |diff| blocks 1/17/64/2048/4096 vs 512, 1x | < 1e-05  | 0.0000  | 44.1 k mono | PASS 10/10 |
| 8 Block-size invariance | max |diff| blocks 1/17/64/2048/4096 vs 512, 2x | < 1e-05  | 0.0000  | 44.1 k mono | PASS 10/10 |
| 8 Block-size invariance | max |diff| blocks 1/17/64/2048/4096 vs 512, 4x | < 1e-05  | 0.0000  | 44.1 k mono | PASS 10/10 |
| F Features (spec section 2) | FET SC HPF: GR reduction on 60 Hz, HPF 20 -> 300 Hz | > 6.000 dB | 8.908 dB | 44.1 k mono | PASS 10/10 |
| F Features (spec section 2) | Opto SC HPF: GR reduction on 60 Hz, HPF 20 -> 300 Hz | > 6.000 dB | 17.625 dB | 44.1 k mono | PASS 10/10 |
| F Features (spec section 2) | SC Listen (HPF 300 Hz): 50 Hz relative to 3 kHz in output | < -20.000 dB | -31.129 dB | 192 k mono | PASS 10/10 |
| F Features (spec section 2) | Emphasis 100 %: extra opto GR on 8 kHz | > 4.000 dB | 7.726 dB | 192 k mono | PASS 10/10 |
| F Features (spec section 2) | Emphasis 100 %: GR change on 200 Hz (should be ~0) | < 0.5000 dB | 0.000188 dB | 192 k mono | PASS 10/10 |
| F Features (spec section 2) | Auto makeup: final makeup vs average GR (|diff|) | < 1.500 dB | 0.4186 dB | 192 k mono | PASS 10/10 |
| F Features (spec section 2) | Auto makeup: time to 63 % of final makeup (>= 1 s) | 1.000 .. 6.000 s | 2.235 s | 192 k mono | PASS 10/10 |
| F Features (spec section 2) | Auto makeup: cap under extreme GR | < 24.001 dB | 23.987 dB | 44.1 k mono | PASS 10/10 |
| F Features (spec section 2) | OS switching every 37 blocks: all samples finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| F Features (spec section 2) | OS switching: peak output vs peak input | < 12.000 dB | -7.057 dB | 44.1 k mono | PASS 10/10 |
| F Features (spec section 2) | OS switched to 4x: reported latency - latency of 4x | = 0.0000 samples | 0.0000 samples | 44.1 k mono | PASS 10/10 |
| F Features (spec section 2) | OS switched to 4x: impulse peak - reported latency | = 0.0000 samples | 0.0000 samples | 44.1 k mono | PASS 10/10 |
| F Features (spec section 2) | Order switch while playing: max sample step vs steady tone | < 1.500 x | 1.030 x | 96 k mono | PASS 10/10 |
| F Features (spec section 2) | Output +12 dB step: 10-90 % transition | > 5.000 ms | 44.000 ms | 44.1 k mono | PASS 10/10 |
| F Features (spec section 2) | FET Output -20 dB step: 10-90 % transition | > 5.000 ms | 44.000 ms | 44.1 k mono | PASS 10/10 |
| F Features (spec section 2) | Opto Gain +12 dB step: 10-90 % transition | > 5.000 ms | 44.000 ms | 44.1 k mono | PASS 10/10 |
| F Features (spec section 2) | FET Input -18 dB step: 10-90 % transition | > 5.000 ms | 44.000 ms | 44.1 k mono | PASS 10/10 |
| F Features (spec section 2) | Mix 100 -> 0 % step (wet 20 dB lower): 10-90 % transition | > 5.000 ms | 44.000 ms | 44.1 k mono | PASS 10/10 |
| O Oversampling filters | 2x stage (up/down): stopband peak | < -100.0 dB | -100.3 dB | 88.2 k mono | PASS 10/10 |
| O Oversampling filters | 2x stage (up/down): passband ripple | < 0.0010 dB | 8.43e-05 dB | 88.2 k mono | PASS 10/10 |
| O Oversampling filters | 4x stage: stopband peak | < -100.0 dB | -100.4 dB | 44.1 k mono | PASS 10/10 |
| O Oversampling filters | 4x stage: passband ripple | < 0.0010 dB | 8.34e-05 dB | 44.1 k mono | PASS 10/10 |
| O Oversampling filters | latency 1x | (info) | 0.0000 samples | all | - |
| O Oversampling filters | wet path (stages off) flatness 20 Hz-20 kHz, 1x | < 0.0100 dB | 0.0000 dB | 44.1 k mono | PASS 10/10 |
| O Oversampling filters | latency 2x | (info) | 25.000 .. 71.000 samples | all | - |
| O Oversampling filters | wet path (stages off) flatness 20 Hz-20 kHz, 2x | < 0.0100 dB | 0.000157 dB | 44.1 k mono | PASS 10/10 |
| O Oversampling filters | latency 4x | (info) | 33.000 .. 79.000 samples | all | - |
| O Oversampling filters | wet path (stages off) flatness 20 Hz-20 kHz, 4x | < 0.0100 dB | 0.000205 dB | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Rap Lead: output vs input loudness | -1.500 .. 1.500 dB | 0.4610 dB | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Rap Lead: output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Rap Lead: FET peak GR | (info) | 6.680 .. 8.116 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Rap Lead: opto average GR | (info) | 2.737 .. 3.061 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Boom Bap Double: output vs input loudness | -1.500 .. 1.500 dB | -0.5382 dB | 192 k stereo | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Boom Bap Double: output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Boom Bap Double: FET peak GR | (info) | 9.534 .. 10.787 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Boom Bap Double: opto average GR | (info) | 4.342 .. 4.569 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Aggressive Parallel (All): output vs input loudness | -1.500 .. 1.500 dB | -0.2183 dB | 192 k stereo | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Aggressive Parallel (All): output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Aggressive Parallel (All): FET peak GR | (info) | 17.108 .. 19.009 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Aggressive Parallel (All): opto average GR | (info) | 1.148 .. 1.661 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Gentle Leveling: output vs input loudness | -1.500 .. 1.500 dB | 0.2353 dB | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Gentle Leveling: output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Gentle Leveling: FET peak GR | (info) | 2.961 .. 4.352 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Gentle Leveling: opto average GR | (info) | 3.620 .. 4.055 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Spoken Word: output vs input loudness | -1.500 .. 1.500 dB | -0.3418 dB | 192 k stereo | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Spoken Word: output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Spoken Word: FET peak GR | (info) | 4.854 .. 6.262 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Spoken Word: opto average GR | (info) | 3.666 .. 4.057 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Classic Serial (FET > Opto): output vs input loudness | -1.500 .. 1.500 dB | -0.2304 dB | 96 k stereo | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Classic Serial (FET > Opto): output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Classic Serial (FET > Opto): FET peak GR | (info) | 4.863 .. 6.269 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Classic Serial (FET > Opto): opto average GR | (info) | 2.666 .. 3.111 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Opto First, FET Catch: output vs input loudness | -1.500 .. 1.500 dB | 0.6020 dB | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Opto First, FET Catch: output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Opto First, FET Catch: FET peak GR | (info) | 3.836 .. 4.721 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Opto First, FET Catch: opto average GR | (info) | 3.740 .. 3.973 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: New York Parallel: output vs input loudness | -1.500 .. 1.500 dB | -0.1718 dB | 192 k stereo | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: New York Parallel: output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: New York Parallel: FET peak GR | (info) | 13.602 .. 15.354 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: New York Parallel: opto average GR | (info) | 1.144 .. 1.581 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Upfront Hip-Hop: output vs input loudness | -1.500 .. 1.500 dB | -0.3752 dB | 192 k stereo | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Upfront Hip-Hop: output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Upfront Hip-Hop: FET peak GR | (info) | 7.379 .. 8.813 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Upfront Hip-Hop: opto average GR | (info) | 2.763 .. 3.057 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Ad-libs & Hype: output vs input loudness | -1.500 .. 1.500 dB | -0.7582 dB | 192 k stereo | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Ad-libs & Hype: output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Ad-libs & Hype: FET peak GR | (info) | 10.679 .. 12.031 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Ad-libs & Hype: opto average GR | (info) | 3.909 .. 4.135 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Sibilance-Aware Leveling: output vs input loudness | -1.500 .. 1.500 dB | -1.252 dB | 192 k stereo | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Sibilance-Aware Leveling: output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Sibilance-Aware Leveling: FET peak GR | (info) | 2.589 .. 3.997 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Sibilance-Aware Leveling: opto average GR | (info) | 3.926 .. 4.568 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Transparent Peak Control: output vs input loudness | -1.500 .. 1.500 dB | 0.5901 dB | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Transparent Peak Control: output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Transparent Peak Control: FET peak GR | (info) | 5.475 .. 6.911 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Transparent Peak Control: opto average GR | (info) | 0.0000 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Sung Hook: output vs input loudness | -1.500 .. 1.500 dB | 0.4391 dB | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Sung Hook: output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Sung Hook: FET peak GR | (info) | 3.711 .. 5.122 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Sung Hook: opto average GR | (info) | 4.716 .. 5.103 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Lo-fi Grit (All): output vs input loudness | -1.500 .. 1.500 dB | -0.4527 dB | 192 k stereo | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Lo-fi Grit (All): output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Lo-fi Grit (All): FET peak GR | (info) | 14.792 .. 16.506 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Lo-fi Grit (All): opto average GR | (info) | 1.532 .. 2.114 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Broadcast Voice: output vs input loudness | -1.500 .. 1.500 dB | -0.2272 dB | 96 k stereo | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Broadcast Voice: output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Broadcast Voice: FET peak GR | (info) | 5.604 .. 7.280 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Vocal: Broadcast Voice: opto average GR | (info) | 4.581 .. 5.185 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Mix Bus: Glue: output vs input loudness | -1.500 .. 1.500 dB | 0.1715 dB | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Mix Bus: Glue: output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Mix Bus: Glue: FET peak GR | (info) | 1.808 .. 2.361 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Mix Bus: Glue: opto average GR | (info) | 1.436 .. 1.509 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Mix Bus: Punch: output vs input loudness | -1.500 .. 1.500 dB | -0.1636 dB | 192 k stereo | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Mix Bus: Punch: output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Mix Bus: Punch: FET peak GR | (info) | 3.674 .. 4.179 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Mix Bus: Punch: opto average GR | (info) | 0.0000 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Master: Transparent Leveling: output vs input loudness | -1.500 .. 1.500 dB | 0.2189 dB | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Master: Transparent Leveling: output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Master: Transparent Leveling: FET peak GR | (info) | 0.0000 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Master: Transparent Leveling: opto average GR | (info) | 1.972 .. 2.009 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Master: Peak Catch: output vs input loudness | -1.500 .. 1.500 dB | -0.0913 dB | 192 k stereo | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Master: Peak Catch: output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Master: Peak Catch: FET peak GR | (info) | 1.621 .. 2.288 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Master: Peak Catch: opto average GR | (info) | 0.0000 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Master: Parallel Density: output vs input loudness | -1.500 .. 1.500 dB | -0.2627 dB | 192 k stereo | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Master: Parallel Density: output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Master: Parallel Density: FET peak GR | (info) | 7.268 .. 7.952 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Master: Parallel Density: opto average GR | (info) | 0.0000 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Drum Bus: Boom Bap Smash: output vs input loudness | -1.500 .. 1.500 dB | -0.2987 dB | 192 k stereo | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Drum Bus: Boom Bap Smash: output finite | no NaN/Inf | 1.000  | 44.1 k mono | PASS 10/10 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Drum Bus: Boom Bap Smash: FET peak GR | (info) | 14.671 .. 15.450 dB | all | - |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS) | Drum Bus: Boom Bap Smash: opto average GR | (info) | 0.0000 dB | all | - |
| 9 Stereo identity | identical L/R in: differing output samples (defaults) | = 0.0000 samples | 0.0000 samples | 44.1 k stereo | PASS 5/5 |
| 9 Stereo identity | identical L/R in: differing samples (4x, link 0, All, auto makeup) | = 0.0000 samples | 0.0000 samples | 44.1 k stereo | PASS 5/5 |
| 9 Stereo identity | mono instance vs dual-mono stereo: max |diff| | < 1e-06  | 0.0000  | 44.1 k stereo | PASS 5/5 |
| F Features (spec section 2) | Link 100 %: quiet side follows loud side's GR (|GR_L - GR_R|) | < 0.5000 dB | 2.29e-07 dB | 192 k stereo | PASS 5/5 |
| F Features (spec section 2) | Link 0 %: quiet side GR (independent detection) | < 0.2000 dB | 7.36e-06 dB | 88.2 k stereo | PASS 5/5 |
| O Oversampling filters | 2x decimator (stopband from base Nyquist): stopband peak | < -100.0 dB | -100.2 dB | 88.2 k mono | PASS 6/6 |
| O Oversampling filters | 2x decimator (stopband from base Nyquist): passband ripple | < 0.0010 dB | 0.000101 dB | 88.2 k mono | PASS 6/6 |
| 10 CPU | 48 kHz / 128 / stereo / 2x, 60 s program: CPU of one core | < 2.000 % | 1.659 % | 48 k stereo | PASS 1/1 |
| 10 CPU | 48 kHz / 128 / stereo / 2x: real-time factor | (info) | 60.276 x | all | - |
| 10 CPU | silence after program (denormal safety): CPU silence / CPU program | < 1.500 x | 0.8012 x | 48 k stereo | PASS 1/1 |
| 10 CPU | 48 kHz / 128 / stereo / 1x: CPU of one core | (info) | 1.040 % | all | - |
| 10 CPU | 48 kHz / 128 / stereo / 4x: CPU of one core | (info) | 2.957 % | all | - |
| 10 CPU | stereo / 128 / 2x at 44.100 kHz: CPU of one core | (info) | 1.527 % | all | - |
| 10 CPU | stereo / 128 / 2x at 96.000 kHz: CPU of one core | (info) | 3.307 % | all | - |
| 10 CPU | stereo / 128 / 2x at 192.0 kHz: CPU of one core | (info) | 6.807 % | all | - |

## Detail per configuration

| Check | 44.1 k mono | 44.1 k stereo | 48 k mono | 48 k stereo | 88.2 k mono | 88.2 k stereo | 96 k mono | 96 k stereo | 192 k mono | 192 k stereo |
|---|---|---|---|---|---|---|---|---|---|---|
| 1 Static curve: FET 4:1: slope below threshold (-60..-44 dBFS) | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 |
| 1 Static curve: FET 4:1: ratio above threshold (-12..0 dBFS in) | 4.006 | 4.006 | 4.002 | 4.002 | 4.008 | 4.008 | 4.013 | 4.013 | 4.009 | 4.009 |
| 1 Static curve: FET 8:1: slope below threshold (-60..-44 dBFS) | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 |
| 1 Static curve: FET 8:1: ratio above threshold (-12..0 dBFS in) | 8.030 | 8.030 | 8.031 | 8.031 | 8.100 | 8.100 | 8.134 | 8.134 | 8.033 | 8.033 |
| 1 Static curve: FET 12:1: slope below threshold (-60..-44 dBFS) | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 |
| 1 Static curve: FET 12:1: ratio above threshold (-12..0 dBFS in) | 12.057 | 12.057 | 12.069 | 12.069 | 12.191 | 12.191 | 12.274 | 12.274 | 11.910 | 11.910 |
| 1 Static curve: FET 20:1: slope below threshold (-60..-44 dBFS) | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 |
| 1 Static curve: FET 20:1: ratio above threshold (-12..0 dBFS in) | 20.132 | 20.132 | 20.183 | 20.183 | 20.402 | 20.402 | 20.641 | 20.641 | 19.124 | 19.124 |
| 1 Static curve: FET All: slope below threshold (-60..-44 dBFS) | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 |
| 1 Static curve: FET All: over-compression, drop from curve peak to 0 dBFS in | 0.7380 | 0.7380 | 0.7422 | 0.7422 | 0.7353 | 0.7353 | 0.7314 | 0.7314 | 0.7479 | 0.7479 |
| 1 Static curve: FET All: curve peak position above threshold | 8.000 | 8.000 | 8.000 | 8.000 | 8.000 | 8.000 | 8.000 | 8.000 | 8.000 | 8.000 |
| 1 Static curve: FET All: drop stays 'slight' (peak to 0 dBFS in) | 0.7380 | 0.7380 | 0.7422 | 0.7422 | 0.7353 | 0.7353 | 0.7314 | 0.7314 | 0.7479 | 0.7479 |
| 1 Static curve: Opto Compress: slope below threshold (-60..-44 dBFS) | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 |
| 1 Static curve: Opto Compress: ratio above threshold (-12..0 dBFS in) | 3.000 | 3.000 | 3.000 | 3.000 | 3.000 | 3.000 | 3.000 | 3.000 | 2.999 | 2.999 |
| 1 Static curve: Opto Limit: slope below threshold (-60..-44 dBFS) | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 | 1.0000 |
| 1 Static curve: Opto Limit: ratio above threshold (-12..0 dBFS in) | 10.000 | 10.000 | 10.000 | 10.000 | 10.000 | 10.000 | 10.000 | 10.000 | 10.000 | 10.000 |
| 2 FET attack/release: attack 20.000 us: time to 63 % GR | 20.642 | 20.642 | 20.274 | 20.274 | 20.277 | 20.277 | 20.177 | 20.177 | 20.161 | 20.161 |
| 2 FET attack/release: attack 800.0 us: time to 63 % GR | 803.6 | 803.6 | 803.8 | 803.8 | 802.2 | 802.2 | 802.6 | 802.6 | 801.3 | 801.3 |
| 2 FET attack/release: release 50.000 ms: time to 63 % recovery | 55.169 | 55.169 | 55.204 | 55.204 | 55.574 | 55.574 | 55.554 | 55.554 | 55.573 | 55.573 |
| 2 FET attack/release: release 1100.0 ms: time to 63 % recovery | 1164.3 | 1164.3 | 1165.3 | 1165.3 | 1172.6 | 1172.6 | 1172.7 | 1172.7 | 1173.4 | 1173.4 |
| 2 FET attack/release: program-dependent: GR during hold | 16.507 | 16.507 | 16.513 | 16.513 | 16.520 | 16.520 | 16.526 | 16.526 | 16.532 | 16.532 |
| 2 FET attack/release: program-dependent: release after 6 s vs 0.3 s of >10 dB GR | 1.295 | 1.295 | 1.295 | 1.295 | 1.295 | 1.295 | 1.295 | 1.295 | 1.294 | 1.294 |
| 2 FET attack/release: All mode: attack time relative to 20:1 (800 us knob) | 0.6008 | 0.6008 | 0.6005 | 0.6005 | 0.6002 | 0.6002 | 0.6004 | 0.6004 | 0.6015 | 0.6015 |
| 3 Opto attack/release: attack: time to 63 % of 10 dB GR | 9.798 | 9.798 | 9.796 | 9.796 | 9.787 | 9.787 | 9.786 | 9.786 | 9.771 | 9.771 |
| 3 Opto attack/release: release after 2 s of 10 dB GR: 50 % recovery | 71.245 | 71.245 | 71.233 | 71.233 | 71.255 | 71.255 | 71.260 | 71.260 | 71.218 | 71.218 |
| 3 Opto attack/release: release tail time constant after 2 s (70->90 % recovery) | 1.928 | 1.928 | 1.926 | 1.926 | 1.937 | 1.937 | 1.936 | 1.936 | 1.937 | 1.937 |
| 3 Opto attack/release: 90 % recovery after 1 s of 10 dB GR | 1.921 | 1.921 | 1.918 | 1.918 | 1.931 | 1.931 | 1.929 | 1.929 | 1.929 | 1.929 |
| 3 Opto attack/release: 90 % recovery after 10 s of 10 dB GR | 4.864 | 4.864 | 4.864 | 4.864 | 4.871 | 4.871 | 4.874 | 4.874 | 4.866 | 4.866 |
| 3 Opto attack/release: light-cell memory: 90 % recovery 10 s hold / 1 s hold | 2.533 | 2.533 | 2.537 | 2.537 | 2.523 | 2.523 | 2.527 | 2.527 | 2.523 | 2.523 |
| 3 Opto attack/release: release smoothness (slope residual vs slope, 10 s hold) | 0.0026 | 0.0026 | 0.0026 | 0.0026 | 0.0033 | 0.0033 | 0.0035 | 0.0035 | 0.0055 | 0.0055 |
| 4 THD (1 kHz): FET 0 dB GR, Color 0 | 1.72e-06 | 1.72e-06 | 5.1e-06 | 5.1e-06 | 1.26e-06 | 1.26e-06 | 4.94e-06 | 4.94e-06 | 3.62e-06 | 3.62e-06 |
| 4 THD (1 kHz): FET 10 dB GR, Color 50 (4:1) | 0.6635 | 0.6635 | 0.6633 | 0.6633 | 0.6633 | 0.6633 | 0.6630 | 0.6630 | 0.6619 | 0.6619 |
| 4 THD (1 kHz): FET 10 dB GR, Color 50: share of 2nd+3rd in distortion power | 99.830 | 99.830 | 99.830 | 99.830 | 99.830 | 99.830 | 99.830 | 99.830 | 99.832 | 99.832 |
| 4 THD (1 kHz): FET All 10 dB GR, Color 50 | 1.110 | 1.110 | 1.110 | 1.110 | 1.110 | 1.110 | 1.109 | 1.109 | 1.107 | 1.107 |
| 4 THD (1 kHz): FET All vs 4:1 THD at 10 dB GR, Color 50 | 1.673 | 1.673 | 1.673 | 1.673 | 1.673 | 1.673 | 1.673 | 1.673 | 1.673 | 1.673 |
| 4 THD (1 kHz): Opto 10 dB GR, Color 0 (clean) | 2.19e-06 | 2.19e-06 | 7.7e-06 | 7.7e-06 | 3.51e-05 | 3.51e-05 | 4.67e-05 | 4.67e-05 | 2.47e-06 | 2.47e-06 |
| 4 THD (1 kHz): Opto 10 dB GR, Color 50 | 0.1348 | 0.1348 | 0.1348 | 0.1348 | 0.1348 | 0.1348 | 0.1348 | 0.1348 | 0.1348 | 0.1348 |
| 4 THD (1 kHz): Opto Color 50: H2 / H3 (predominantly 2nd) | 118.7 | 118.7 | 118.6 | 118.6 | 122.4 | 122.4 | 123.4 | 123.4 | 118.7 | 118.7 |
| 4 THD (1 kHz): Opto Color 50: THD rises with level (0 dB vs -20 dB rel.) | 3.169 | 3.169 | 3.169 | 3.169 | 3.169 | 3.169 | 3.169 | 3.169 | 3.169 | 3.169 |
| 4 THD (1 kHz): Opto Color 100: DC offset | -203.7 | -203.7 | -191.0 | -191.0 | -206.8 | -206.8 | -177.7 | -177.7 | -176.7 | -176.7 |
| 5 Aliasing (15 kHz, -3 dBFS, FET 20:1, Color 100): FET only, 1x (no oversampling) | -51.574 | -51.574 | -51.385 | -51.385 | -53.498 | -53.498 | -84.380 | -84.380 | -117.4 | -117.4 |
| 5 Aliasing (15 kHz, -3 dBFS, FET 20:1, Color 100): FET only, 2x | -84.472 | -84.472 | -84.383 | -84.383 | -156.3 | -156.3 | -163.4 | -163.4 | -164.8 | -164.8 |
| 5 Aliasing (15 kHz, -3 dBFS, FET 20:1, Color 100): FET only, 4x | -157.2 | -157.2 | -154.2 | -154.2 | -156.7 | -156.7 | -159.2 | -159.2 | -160.8 | -160.8 |
| 5 Aliasing (15 kHz, -3 dBFS, FET 20:1, Color 100): FET + Opto (default), 1x (no oversampling) | -54.778 | -54.778 | -54.586 | -54.586 | -60.529 | -60.529 | -89.443 | -89.443 | -124.4 | -124.4 |
| 5 Aliasing (15 kHz, -3 dBFS, FET 20:1, Color 100): FET + Opto (default), 2x | -91.519 | -91.519 | -91.428 | -91.428 | -163.7 | -163.7 | -160.5 | -160.5 | -165.4 | -165.4 |
| 5 Aliasing (15 kHz, -3 dBFS, FET 20:1, Color 100): FET + Opto (default), 4x | -135.6 | -135.6 | -152.9 | -152.9 | -164.2 | -164.2 | -163.8 | -163.8 | -161.4 | -161.4 |
| 6 Null / latency: Mix 0 %: residual vs input delayed by reported latency, 1x | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 |
| 6 Null / latency: reported latency matches factor, 1x | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| 6 Null / latency: wet impulse peak offset - reported latency, 1x | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| 6 Null / latency: Mix 50 %, stages off: residual vs delayed dry (<18 kHz), 1x | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 |
| 6 Null / latency: Mix 0 %: residual vs input delayed by reported latency, 2x | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 |
| 6 Null / latency: reported latency matches factor, 2x | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| 6 Null / latency: wet impulse peak offset - reported latency, 2x | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| 6 Null / latency: Mix 50 %, stages off: residual vs delayed dry (<18 kHz), 2x | -131.9 | -131.9 | -132.5 | -132.5 | -129.1 | -129.1 | -126.7 | -126.7 | -130.3 | -130.3 |
| 6 Null / latency: Mix 0 %: residual vs input delayed by reported latency, 4x | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 | -600.0 |
| 6 Null / latency: reported latency matches factor, 4x | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| 6 Null / latency: wet impulse peak offset - reported latency, 4x | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| 6 Null / latency: Mix 50 %, stages off: residual vs delayed dry (<18 kHz), 4x | -121.3 | -121.3 | -121.4 | -121.4 | -123.3 | -123.3 | -123.7 | -123.7 | -127.0 | -127.0 |
| 7 Silence / stability: zeros after +24 dBFS noise: max |out| after 1 s of silence | 4.01e-17 | 3.5e-17 | 3.59e-17 | 3.37e-17 | 3.41e-17 | 3.35e-17 | 3.72e-17 | 3.55e-17 | 3.3e-17 | 3.13e-17 |
| 7 Silence / stability: zeros from fresh instance: max |out| | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| 7 Silence / stability: random automation every block, +24 dBFS: all samples finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| 7 Silence / stability: random automation: peak output (sanity bound) | 43.563 | 43.570 | 37.739 | 37.736 | 39.161 | 39.160 | 37.960 | 37.965 | 38.069 | 38.069 |
| 7 Silence / stability: random automation then silence: max |out| in last 0.5 s | 2.71e-26 | 2.27e-26 | 0.0000 | 0.0000 | 1.28e-24 | 1.19e-24 | 2.8e-23 | 2.68e-23 | 0.0000 | 0.0000 |
| 8 Block-size invariance: max |diff| blocks 1/17/64/2048/4096 vs 512, 1x | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| 8 Block-size invariance: max |diff| blocks 1/17/64/2048/4096 vs 512, 2x | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| 8 Block-size invariance: max |diff| blocks 1/17/64/2048/4096 vs 512, 4x | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| F Features (spec section 2): FET SC HPF: GR reduction on 60 Hz, HPF 20 -> 300 Hz | 8.908 | 8.908 | 8.909 | 8.909 | 8.909 | 8.909 | 8.909 | 8.909 | 8.909 | 8.909 |
| F Features (spec section 2): Opto SC HPF: GR reduction on 60 Hz, HPF 20 -> 300 Hz | 17.625 | 17.625 | 17.626 | 17.626 | 17.632 | 17.632 | 17.633 | 17.633 | 17.640 | 17.640 |
| F Features (spec section 2): SC Listen (HPF 300 Hz): 50 Hz relative to 3 kHz in output | -31.132 | -31.132 | -31.131 | -31.131 | -31.130 | -31.130 | -31.130 | -31.130 | -31.129 | -31.129 |
| F Features (spec section 2): Emphasis 100 %: extra opto GR on 8 kHz | 7.790 | 7.790 | 7.780 | 7.780 | 7.741 | 7.741 | 7.738 | 7.738 | 7.726 | 7.726 |
| F Features (spec section 2): Emphasis 100 %: GR change on 200 Hz (should be ~0) | 0.000173 | 0.000173 | 0.000176 | 0.000176 | 0.000181 | 0.000181 | 0.000182 | 0.000182 | 0.000188 | 0.000188 |
| F Features (spec section 2): Auto makeup: final makeup vs average GR (|diff|) | 0.0968 | 0.0968 | 0.1052 | 0.1052 | 0.1936 | 0.1936 | 0.2113 | 0.2113 | 0.4186 | 0.4186 |
| F Features (spec section 2): Auto makeup: time to 63 % of final makeup (>= 1 s) | 2.312 | 2.312 | 2.305 | 2.305 | 2.292 | 2.292 | 2.295 | 2.295 | 2.235 | 2.235 |
| F Features (spec section 2): Auto makeup: cap under extreme GR | 23.987 | 23.987 | 23.986 | 23.986 | 23.975 | 23.975 | 23.973 | 23.973 | 23.945 | 23.945 |
| F Features (spec section 2): OS switching every 37 blocks: all samples finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| F Features (spec section 2): OS switching: peak output vs peak input | -7.057 | -7.394 | -7.315 | -7.724 | -7.190 | -7.347 | -7.726 | -8.250 | -8.354 | -8.473 |
| F Features (spec section 2): OS switched to 4x: reported latency - latency of 4x | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| F Features (spec section 2): OS switched to 4x: impulse peak - reported latency | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| F Features (spec section 2): Order switch while playing: max sample step vs steady tone | 0.9999 | 0.9999 | 1.0000 | 1.0000 | 1.000 | 1.000 | 1.030 | 1.030 | 1.029 | 1.029 |
| F Features (spec section 2): Output +12 dB step: 10-90 % transition | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 |
| F Features (spec section 2): FET Output -20 dB step: 10-90 % transition | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 |
| F Features (spec section 2): Opto Gain +12 dB step: 10-90 % transition | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 |
| F Features (spec section 2): FET Input -18 dB step: 10-90 % transition | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 |
| F Features (spec section 2): Mix 100 -> 0 % step (wet 20 dB lower): 10-90 % transition | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 | 44.000 |
| O Oversampling filters: 2x stage (up/down): stopband peak | -100.9 | -100.9 | -100.9 | -100.9 | -100.3 | -100.3 | -100.3 | -100.3 | -100.3 | -100.3 |
| O Oversampling filters: 2x stage (up/down): passband ripple | 7.82e-05 | 7.82e-05 | 7.82e-05 | 7.82e-05 | 8.43e-05 | 8.43e-05 | 8.43e-05 | 8.43e-05 | 8.43e-05 | 8.43e-05 |
| O Oversampling filters: 4x stage: stopband peak | -100.4 | -100.4 | -100.4 | -100.4 | -100.4 | -100.4 | -100.4 | -100.4 | -100.4 | -100.4 |
| O Oversampling filters: 4x stage: passband ripple | 8.34e-05 | 8.34e-05 | 8.34e-05 | 8.34e-05 | 8.34e-05 | 8.34e-05 | 8.34e-05 | 8.34e-05 | 8.34e-05 | 8.34e-05 |
| O Oversampling filters: latency 1x | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| O Oversampling filters: wet path (stages off) flatness 20 Hz-20 kHz, 1x | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| O Oversampling filters: latency 2x | 71.000 | 71.000 | 71.000 | 71.000 | 25.000 | 25.000 | 25.000 | 25.000 | 25.000 | 25.000 |
| O Oversampling filters: wet path (stages off) flatness 20 Hz-20 kHz, 2x | 0.000157 | 0.000157 | 0.000102 | 0.000102 | 7.09e-05 | 7.09e-05 | 6.93e-05 | 6.93e-05 | 5.01e-05 | 5.01e-05 |
| O Oversampling filters: latency 4x | 79.000 | 79.000 | 79.000 | 79.000 | 33.000 | 33.000 | 33.000 | 33.000 | 33.000 | 33.000 |
| O Oversampling filters: wet path (stages off) flatness 20 Hz-20 kHz, 4x | 0.000205 | 0.000205 | 0.000138 | 0.000138 | 0.000123 | 0.000123 | 0.000123 | 0.000123 | 0.000123 | 0.000123 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Rap Lead: output vs input loudness | 0.4610 | 0.1376 | 0.3980 | 0.0855 | 0.2829 | 0.0221 | 0.2108 | -0.0746 | 0.0566 | -0.1258 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Rap Lead: output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Rap Lead: FET peak GR | 6.680 | 7.254 | 7.106 | 7.106 | 7.533 | 7.533 | 7.783 | 8.116 | 7.497 | 7.756 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Rap Lead: opto average GR | 3.037 | 3.061 | 2.995 | 3.005 | 2.935 | 2.903 | 2.914 | 2.859 | 2.778 | 2.737 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Boom Bap Double: output vs input loudness | 0.3287 | -0.0315 | 0.2594 | -0.0697 | 0.0091 | -0.2431 | -0.0886 | -0.3773 | -0.3528 | -0.5382 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Boom Bap Double: output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Boom Bap Double: FET peak GR | 9.534 | 9.863 | 9.912 | 9.912 | 10.755 | 10.755 | 10.165 | 10.787 | 10.217 | 10.383 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Boom Bap Double: opto average GR | 4.569 | 4.538 | 4.517 | 4.483 | 4.490 | 4.445 | 4.501 | 4.393 | 4.421 | 4.342 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Aggressive Parallel (All): output vs input loudness | 0.0239 | -0.0991 | -0.0015 | -0.1207 | -0.0515 | -0.1511 | -0.0780 | -0.1867 | -0.1407 | -0.2183 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Aggressive Parallel (All): output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Aggressive Parallel (All): FET peak GR | 17.108 | 17.556 | 17.740 | 17.740 | 18.549 | 18.549 | 18.376 | 19.009 | 18.220 | 18.494 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Aggressive Parallel (All): opto average GR | 1.661 | 1.580 | 1.598 | 1.516 | 1.450 | 1.360 | 1.409 | 1.303 | 1.224 | 1.148 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Gentle Leveling: output vs input loudness | 0.2353 | -0.0989 | 0.1735 | -0.1347 | 0.1342 | -0.1130 | 0.0796 | -0.2253 | -0.0124 | -0.1924 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Gentle Leveling: output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Gentle Leveling: FET peak GR | 2.961 | 3.589 | 3.203 | 3.241 | 3.764 | 3.765 | 4.112 | 4.352 | 3.638 | 3.992 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Gentle Leveling: opto average GR | 3.974 | 4.055 | 3.913 | 3.996 | 3.844 | 3.865 | 3.822 | 3.796 | 3.630 | 3.620 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Spoken Word: output vs input loudness | 0.1594 | -0.1725 | 0.0924 | -0.2207 | 0.0211 | -0.2351 | -0.0446 | -0.3397 | -0.1642 | -0.3418 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Spoken Word: output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Spoken Word: FET peak GR | 4.854 | 5.452 | 5.175 | 5.175 | 5.682 | 5.682 | 5.977 | 6.262 | 5.591 | 5.899 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Spoken Word: opto average GR | 4.007 | 4.057 | 3.942 | 3.998 | 3.889 | 3.882 | 3.862 | 3.828 | 3.689 | 3.666 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Classic Serial (FET > Opto): output vs input loudness | 0.1985 | -0.1165 | 0.1438 | -0.1623 | 0.1022 | -0.1495 | 0.0458 | -0.2304 | -0.0300 | -0.2123 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Classic Serial (FET > Opto): output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Classic Serial (FET > Opto): FET peak GR | 4.863 | 5.456 | 5.180 | 5.180 | 5.690 | 5.690 | 5.986 | 6.269 | 5.596 | 5.904 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Classic Serial (FET > Opto): opto average GR | 3.062 | 3.111 | 3.011 | 3.048 | 2.907 | 2.900 | 2.884 | 2.850 | 2.693 | 2.666 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Opto First, FET Catch: output vs input loudness | 0.6020 | 0.2881 | 0.4781 | 0.1839 | 0.3453 | 0.0579 | 0.2727 | -0.0823 | -0.0135 | -0.2429 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Opto First, FET Catch: output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Opto First, FET Catch: FET peak GR | 4.006 | 3.891 | 3.927 | 3.836 | 4.657 | 4.486 | 4.721 | 4.588 | 4.598 | 4.515 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Opto First, FET Catch: opto average GR | 3.762 | 3.973 | 3.760 | 3.953 | 3.749 | 3.891 | 3.761 | 3.911 | 3.740 | 3.838 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: New York Parallel: output vs input loudness | 0.0541 | -0.0643 | 0.0294 | -0.0855 | -0.0118 | -0.1100 | -0.0390 | -0.1452 | -0.0966 | -0.1718 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: New York Parallel: output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: New York Parallel: FET peak GR | 13.602 | 14.227 | 14.211 | 14.211 | 14.787 | 14.787 | 14.887 | 15.354 | 14.652 | 14.920 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: New York Parallel: opto average GR | 1.581 | 1.536 | 1.531 | 1.483 | 1.398 | 1.337 | 1.373 | 1.294 | 1.202 | 1.144 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Upfront Hip-Hop: output vs input loudness | 0.2586 | -0.0714 | 0.1987 | -0.1122 | 0.0545 | -0.2065 | -0.0221 | -0.2890 | -0.1844 | -0.3752 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Upfront Hip-Hop: output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Upfront Hip-Hop: FET peak GR | 7.379 | 7.891 | 7.877 | 7.877 | 8.349 | 8.349 | 8.431 | 8.813 | 8.238 | 8.457 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Upfront Hip-Hop: opto average GR | 3.056 | 3.057 | 3.017 | 3.007 | 2.960 | 2.912 | 2.939 | 2.873 | 2.821 | 2.763 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Ad-libs & Hype: output vs input loudness | 0.2385 | -0.1276 | 0.1748 | -0.1675 | -0.1398 | -0.3983 | -0.2247 | -0.5051 | -0.5326 | -0.7582 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Ad-libs & Hype: output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Ad-libs & Hype: FET peak GR | 10.679 | 11.169 | 10.969 | 10.969 | 12.031 | 12.031 | 11.107 | 11.740 | 11.307 | 11.445 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Ad-libs & Hype: opto average GR | 4.135 | 4.059 | 4.077 | 4.014 | 4.090 | 3.998 | 4.073 | 3.936 | 4.027 | 3.909 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Sibilance-Aware Leveling: output vs input loudness | 0.1582 | -0.1044 | 0.0481 | -0.1889 | -0.4327 | -0.6306 | -0.5547 | -0.8008 | -1.104 | -1.252 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Sibilance-Aware Leveling: output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Sibilance-Aware Leveling: FET peak GR | 2.589 | 3.191 | 2.915 | 2.915 | 3.414 | 3.414 | 3.709 | 3.997 | 3.330 | 3.637 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Sibilance-Aware Leveling: opto average GR | 3.930 | 3.968 | 3.926 | 3.967 | 4.272 | 4.256 | 4.305 | 4.263 | 4.568 | 4.543 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Transparent Peak Control: output vs input loudness | 0.5901 | 0.3124 | 0.4629 | 0.1892 | 0.3216 | 0.0571 | 0.2299 | -0.0983 | -0.0768 | -0.2881 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Transparent Peak Control: output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Transparent Peak Control: FET peak GR | 5.475 | 6.123 | 5.849 | 5.852 | 6.304 | 6.305 | 6.602 | 6.911 | 6.264 | 6.565 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Transparent Peak Control: opto average GR | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Sung Hook: output vs input loudness | 0.4391 | 0.1038 | 0.3772 | 0.0664 | 0.3043 | 0.0528 | 0.2390 | -0.0553 | 0.1260 | -0.0481 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Sung Hook: output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Sung Hook: FET peak GR | 3.711 | 4.316 | 4.040 | 4.040 | 4.538 | 4.539 | 4.834 | 5.122 | 4.455 | 4.762 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Sung Hook: opto average GR | 5.040 | 5.103 | 4.978 | 5.052 | 4.933 | 4.936 | 4.906 | 4.873 | 4.733 | 4.716 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Lo-fi Grit (All): output vs input loudness | 0.0398 | -0.2145 | 0.0233 | -0.2210 | -0.1262 | -0.3055 | -0.1658 | -0.3614 | -0.2752 | -0.4527 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Lo-fi Grit (All): output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Lo-fi Grit (All): FET peak GR | 14.792 | 15.554 | 14.805 | 14.805 | 16.506 | 16.506 | 15.000 | 15.487 | 15.540 | 15.540 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Lo-fi Grit (All): opto average GR | 2.114 | 2.019 | 2.036 | 1.963 | 1.867 | 1.786 | 1.826 | 1.733 | 1.629 | 1.532 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Broadcast Voice: output vs input loudness | 0.2001 | -0.1485 | 0.1535 | -0.1786 | 0.1092 | -0.1509 | 0.0483 | -0.2272 | 0.0017 | -0.1685 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Broadcast Voice: output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Broadcast Voice: FET peak GR | 5.604 | 6.275 | 6.101 | 6.101 | 6.599 | 6.599 | 6.891 | 7.280 | 6.557 | 6.861 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Vocal: Broadcast Voice: opto average GR | 5.149 | 5.185 | 5.072 | 5.094 | 4.953 | 4.912 | 4.918 | 4.831 | 4.644 | 4.581 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Mix Bus: Glue: output vs input loudness | 0.1715 | 0.1046 | 0.1263 | 0.0454 | 0.1031 | 0.0448 | 0.0840 | 0.0220 | 0.0537 | 0.0054 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Mix Bus: Glue: output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Mix Bus: Glue: FET peak GR | 1.922 | 1.926 | 1.846 | 2.021 | 1.808 | 1.810 | 2.359 | 2.361 | 2.038 | 2.053 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Mix Bus: Glue: opto average GR | 1.497 | 1.509 | 1.473 | 1.472 | 1.471 | 1.471 | 1.458 | 1.458 | 1.442 | 1.436 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Mix Bus: Punch: output vs input loudness | 0.0447 | -0.0365 | 0.0080 | -0.0773 | -0.0628 | -0.1158 | -0.0582 | -0.1341 | -0.1059 | -0.1636 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Mix Bus: Punch: output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Mix Bus: Punch: FET peak GR | 3.674 | 3.732 | 3.858 | 4.052 | 3.994 | 4.003 | 4.173 | 4.179 | 4.057 | 4.059 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Mix Bus: Punch: opto average GR | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Master: Transparent Leveling: output vs input loudness | 0.2189 | 0.1842 | 0.2153 | 0.1889 | 0.1894 | 0.1673 | 0.1898 | 0.1683 | 0.1842 | 0.1690 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Master: Transparent Leveling: output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Master: Transparent Leveling: FET peak GR | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Master: Transparent Leveling: opto average GR | 1.973 | 2.009 | 1.974 | 2.002 | 1.972 | 1.996 | 1.972 | 1.993 | 1.974 | 1.991 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Master: Peak Catch: output vs input loudness | 0.0417 | 0.0122 | 0.0040 | -0.0331 | -0.0320 | -0.0497 | -0.0254 | -0.0807 | -0.0690 | -0.0913 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Master: Peak Catch: output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Master: Peak Catch: FET peak GR | 1.621 | 1.859 | 1.890 | 2.135 | 1.884 | 1.978 | 2.288 | 2.288 | 2.145 | 2.145 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Master: Peak Catch: opto average GR | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Master: Parallel Density: output vs input loudness | -0.2117 | -0.2302 | -0.2193 | -0.2377 | -0.2351 | -0.2476 | -0.2331 | -0.2525 | -0.2476 | -0.2627 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Master: Parallel Density: output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Master: Parallel Density: FET peak GR | 7.268 | 7.399 | 7.511 | 7.798 | 7.682 | 7.682 | 7.952 | 7.952 | 7.752 | 7.752 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Master: Parallel Density: opto average GR | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Drum Bus: Boom Bap Smash: output vs input loudness | -0.1681 | -0.2162 | -0.1837 | -0.2332 | -0.2132 | -0.2596 | -0.2216 | -0.2696 | -0.2580 | -0.2987 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Drum Bus: Boom Bap Smash: output finite | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 | 1.000 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Drum Bus: Boom Bap Smash: FET peak GR | 14.671 | 14.926 | 14.989 | 15.266 | 14.982 | 15.085 | 15.450 | 15.450 | 15.255 | 15.255 |
| P Factory presets (vocal -18 dBFS RMS / mix -14 dBFS RMS): Drum Bus: Boom Bap Smash: opto average GR | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 |
| 9 Stereo identity: identical L/R in: differing output samples (defaults) | - | 0.0000 | - | 0.0000 | - | 0.0000 | - | 0.0000 | - | 0.0000 |
| 9 Stereo identity: identical L/R in: differing samples (4x, link 0, All, auto makeup) | - | 0.0000 | - | 0.0000 | - | 0.0000 | - | 0.0000 | - | 0.0000 |
| 9 Stereo identity: mono instance vs dual-mono stereo: max |diff| | - | 0.0000 | - | 0.0000 | - | 0.0000 | - | 0.0000 | - | 0.0000 |
| F Features (spec section 2): Link 100 %: quiet side follows loud side's GR (|GR_L - GR_R|) | - | 6.11e-08 | - | 2.1e-07 | - | 1.93e-08 | - | 2.28e-07 | - | 2.29e-07 |
| F Features (spec section 2): Link 0 %: quiet side GR (independent detection) | - | -4.08e-06 | - | -7e-06 | - | 7.36e-06 | - | 6.39e-06 | - | 1.42e-06 |
| O Oversampling filters: 2x decimator (stopband from base Nyquist): stopband peak | - | - | - | - | -100.2 | -100.2 | -100.2 | -100.2 | -100.2 | -100.2 |
| O Oversampling filters: 2x decimator (stopband from base Nyquist): passband ripple | - | - | - | - | 0.000101 | 0.000101 | 0.000101 | 0.000101 | 0.000101 | 0.000101 |
| 10 CPU: 48 kHz / 128 / stereo / 2x, 60 s program: CPU of one core | - | - | - | 1.659 | - | - | - | - | - | - |
| 10 CPU: 48 kHz / 128 / stereo / 2x: real-time factor | - | - | - | 60.276 | - | - | - | - | - | - |
| 10 CPU: silence after program (denormal safety): CPU silence / CPU program | - | - | - | 0.8012 | - | - | - | - | - | - |
| 10 CPU: 48 kHz / 128 / stereo / 1x: CPU of one core | - | - | - | 1.040 | - | - | - | - | - | - |
| 10 CPU: 48 kHz / 128 / stereo / 4x: CPU of one core | - | - | - | 2.957 | - | - | - | - | - | - |
| 10 CPU: stereo / 128 / 2x at 44.100 kHz: CPU of one core | - | - | - | 1.527 | - | - | - | - | - | - |
| 10 CPU: stereo / 128 / 2x at 96.000 kHz: CPU of one core | - | - | - | 3.307 | - | - | - | - | - | - |
| 10 CPU: stereo / 128 / 2x at 192.0 kHz: CPU of one core | - | - | - | 6.807 | - | - | - | - | - | - |
