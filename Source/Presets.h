#pragma once
// Factory presets (framework independent so the test suite can verify them).
//
// The first six are the ones named in ENGINEERING_SPEC.md section 4. The rest recreate widely used,
// well-documented vocal and mix-bus compression techniques (serial FET -> opto, opto-first,
// New York parallel, all-buttons smash, mix-bus glue, mastering leveling/peak control). Names
// describe the technique; no third-party preset data or branding is used.
//
// Gain staging (tuned with the engine, verified by DuoDspTests at every sample rate):
//   vocal presets: vocal-like test signal at -18 dBFS RMS (peaks -4 dBFS)
//   mix/master presets: mix-like signal (kick, snare, pad, vocal) at -14 dBFS RMS
// INPUT sets the FET peak GR, FET OUTPUT recovers it (as on a 1176), PEAK REDUCTION sets the
// average opto GR and Opto GAIN (or OUTPUT for parallel presets) makes it up. Output loudness
// is within +-1.5 dB of the input, so switching presets compares sound, not level.
// The intended GR is listed in each description (FET = peak GR, opto = average GR).
#include "Engine.h"

#include <array>

namespace duo
{
enum class PresetCategory { vocal, mixMaster, utility };

struct FactoryPreset
{
    const char* name;
    PresetCategory category;
    const char* description;
    Params params;
};

namespace preset_detail
{
struct Row
{
    const char* name;
    PresetCategory cat;
    const char* desc;
    bool fetOn; float fetIn, fetOut, atkMs, relMs; int ratio;
    bool optoOn; float pr, optoGain; int mode;
    int order; float hpf, color, mix, out, link, emph; int os;
};

using C = PresetCategory;
// clang-format off
// fields: fetOn in fetOut atk(ms) rel(ms) ratio | optoOn PR optoGain mode | order hpf color mix out link emph os
constexpr Row kRows[] = {
    { "Rap Lead", C::vocal, "Upfront lead rap vocal: FET grabs consonant peaks (~7 dB), opto adds ~3 dB of steady leveling.",
      true, 1.0f, 4.0f, 0.25f, 120.f, 0,  true, 16.5f, 3.5f, 0,  0, 100.f, 35.f, 100.f, 0.0f, 100.f, 20.f, 1 },
    { "Boom Bap Double", C::vocal, "Doubles/stacks: harder 8:1 FET (~10 dB) and ~4.5 dB opto so the double sits flat behind the lead.",
      true, 3.0f, 5.0f, 0.4f, 200.f, 1,  true, 22.5f, 4.5f, 0,  0, 140.f, 45.f, 100.f, 0.0f, 100.f, 35.f, 1 },
    { "Aggressive Parallel (All)", C::vocal, "All-buttons smash (~18 dB) blended 45 % under the dry vocal: loud and dense, transients kept.",
      true, 9.0f, -2.0f, 0.15f, 80.f, 4,  true, 26.0f, 1.0f, 0,  0, 120.f, 60.f, 45.f, 2.5f, 100.f, 0.f, 1 },
    { "Gentle Leveling", C::vocal, "The opto does the work (~4 dB), the FET only touches the loudest peaks (~3 dB).",
      true, -4.0f, 6.0f, 0.6f, 300.f, 0,  true, 21.0f, 4.5f, 0,  0, 80.f, 20.f, 100.f, 0.0f, 100.f, 0.f, 1 },
    { "Spoken Word", C::vocal, "Podcast/interview: even and natural (FET ~5 dB, opto ~4 dB), low colour, no pumping between words.",
      true, -1.5f, 5.5f, 0.4f, 250.f, 0,  true, 20.0f, 4.0f, 0,  0, 90.f, 15.f, 100.f, 0.0f, 100.f, 10.f, 1 },
    { "Init", C::utility, "All controls at their defaults.",
      true, 6.0f, 0.0f, 0.2f, 150.f, 0,  true, 30.0f, 0.0f, 0,  0, 80.f, 30.f, 100.f, 0.0f, 100.f, 0.f, 1 },
    // vocal techniques
    { "Vocal: Classic Serial (FET > Opto)", C::vocal, "The classic chain: fast FET catches peaks (~5 dB), opto behind it levels the phrase (~3 dB).",
      true, -1.5f, 4.5f, 0.3f, 90.f, 0, true, 17.5f, 3.5f, 0, 0, 90.f, 30.f, 100.f, 0.0f, 100.f, 0.f, 1 },
    { "Vocal: Opto First, FET Catch", C::vocal, "Opto levels the performance first (~4 dB), then an 8:1 FET only catches what still pokes out (~4 dB).",
      true, -3.5f, 6.0f, 0.1f, 80.f, 1, true, 20.5f, 4.0f, 0, 1, 90.f, 25.f, 100.f, 0.0f, 100.f, 0.f, 1 },
    { "Vocal: New York Parallel", C::vocal, "Heavy 12:1 FET (~14 dB) mixed 40 % under the dry vocal: thickness without losing the attack.",
      true, 7.0f, 0.0f, 0.1f, 60.f, 2, true, 19.0f, 1.0f, 0, 0, 110.f, 50.f, 40.f, 1.5f, 100.f, 0.f, 1 },
    { "Vocal: Upfront Hip-Hop", C::vocal, "Punchy, in-your-face rap: fast attack and release (~8 dB FET) with some grit, opto ~3 dB.",
      true, 2.0f, 2.5f, 0.15f, 60.f, 0, true, 19.0f, 4.0f, 0, 0, 110.f, 45.f, 100.f, 0.0f, 100.f, 25.f, 1 },
    { "Vocal: Ad-libs & Hype", C::vocal, "Shouted ad-libs and hype tracks: 8:1 FET (~11 dB), thin low end in the detector, extra grit.",
      true, 4.5f, 2.0f, 0.1f, 70.f, 1, true, 26.0f, 5.5f, 0, 0, 160.f, 55.f, 100.f, 0.0f, 100.f, 40.f, 1 },
    { "Vocal: Sibilance-Aware Leveling", C::vocal, "Opto with strong HF emphasis (~4 dB): reacts more to harsh S/T sounds, gentle FET (~3 dB).",
      true, -4.5f, 6.0f, 0.5f, 200.f, 0, true, 17.0f, 4.5f, 0, 0, 90.f, 15.f, 100.f, 0.0f, 100.f, 80.f, 1 },
    { "Vocal: Transparent Peak Control", C::vocal, "Clean 4:1 FET only (~6 dB), slow attack keeps consonants natural, no colour, opto off.",
      true, -0.5f, 5.0f, 0.8f, 200.f, 0, false, 0.0f, 0.0f, 0, 0, 90.f, 0.f, 100.f, 0.0f, 100.f, 0.f, 1 },
    { "Vocal: Sung Hook", C::vocal, "Melodic hooks/choruses: smooth opto (~5 dB) does most of it, light FET (~4 dB), slower release.",
      true, -3.0f, 5.5f, 0.5f, 300.f, 0, true, 26.0f, 6.0f, 0, 0, 90.f, 25.f, 100.f, 0.0f, 100.f, 10.f, 1 },
    { "Vocal: Lo-fi Grit (All)", C::vocal, "Dirty all-buttons crunch (~15 dB) with full colour, 70 % wet, detector ignores lows.",
      true, 7.5f, -2.0f, 0.2f, 100.f, 4, true, 30.5f, 1.0f, 0, 0, 200.f, 100.f, 70.f, 4.0f, 100.f, 0.f, 1 },
    { "Vocal: Broadcast Voice", C::vocal, "Radio/voice-over density: 8:1 FET (~6 dB) into opto Limit mode (~5 dB), very consistent level.",
      true, -1.5f, 5.0f, 0.3f, 150.f, 1, true, 21.5f, 6.0f, 1, 0, 100.f, 10.f, 100.f, 0.0f, 100.f, 10.f, 1 },
    // mix bus / master
    { "Mix Bus: Glue", C::mixMaster, "Gentle bus glue: slow-attack 4:1 FET (~2 dB) plus light opto (~1.5 dB); SC HPF stops kick pumping.",
      true, -7.5f, 8.5f, 0.8f, 300.f, 0, true, 3.0f, 1.5f, 0, 0, 120.f, 10.f, 100.f, 0.0f, 100.f, 0.f, 2 },
    { "Mix Bus: Punch", C::mixMaster, "Slow attack, quick release FET (~4 dB): lets drum transients through and brings up the body.",
      true, -5.5f, 7.0f, 0.8f, 100.f, 0, false, 0.0f, 0.0f, 0, 0, 90.f, 20.f, 100.f, 0.0f, 100.f, 0.f, 2 },
    { "Master: Transparent Leveling", C::mixMaster, "Mastering: opto only (~2 dB average), no colour, 4x oversampling.",
      false, 0.0f, 0.0f, 0.2f, 150.f, 0, true, 2.0f, 2.5f, 0, 0, 60.f, 0.f, 100.f, 0.0f, 100.f, 0.f, 2 },
    { "Master: Peak Catch", C::mixMaster, "Mastering: fast 20:1 FET only shaves the highest peaks (~2 dB), no colour, 4x oversampling.",
      true, -9.5f, 10.0f, 0.05f, 80.f, 3, false, 0.0f, 0.0f, 0, 0, 60.f, 0.f, 100.f, 0.0f, 100.f, 0.f, 2 },
    { "Master: Parallel Density", C::mixMaster, "Mastering parallel: 8:1 FET (~8 dB) blended 25 % for density without squashing transients.",
      true, -2.5f, 0.0f, 0.2f, 120.f, 1, false, 0.0f, 0.0f, 0, 0, 80.f, 20.f, 25.f, 1.0f, 100.f, 0.f, 2 },
    { "Drum Bus: Boom Bap Smash", C::mixMaster, "Beat/drum bus: all-buttons smash (~15 dB) at 40 %, attack lets the kick/snare crack through.",
      true, 3.5f, -2.0f, 0.3f, 60.f, 4, false, 0.0f, 0.0f, 0, 0, 60.f, 60.f, 40.f, 2.0f, 100.f, 0.f, 1 },
};
// clang-format on
} // namespace preset_detail

constexpr int kNumFactoryPresets = (int) (sizeof (preset_detail::kRows) / sizeof (preset_detail::kRows[0]));

inline std::array<FactoryPreset, kNumFactoryPresets> factoryPresets()
{
    std::array<FactoryPreset, kNumFactoryPresets> list {};
    for (int i = 0; i < kNumFactoryPresets; ++i)
    {
        const auto& r = preset_detail::kRows[i];
        Params p;
        p.fetOn = r.fetOn;   p.fetInDb = r.fetIn;   p.fetOutDb = r.fetOut;
        p.fetAtkMs = r.atkMs; p.fetRelMs = r.relMs; p.fetRatio = r.ratio;
        p.optoOn = r.optoOn; p.optoPR = r.pr;       p.optoGainDb = r.optoGain; p.optoMode = r.mode;
        p.order = r.order;   p.scHpfHz = r.hpf;     p.colorPct = r.color;
        p.mixPct = r.mix;    p.outDb = r.out;       p.linkPct = r.link;
        p.emphasisPct = r.emph; p.osLog2 = r.os;
        list[(size_t) i] = { r.name, r.cat, r.desc, p };
    }
    return list;
}
} // namespace duo
