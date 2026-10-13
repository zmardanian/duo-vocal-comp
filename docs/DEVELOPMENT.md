# Building Duo Vocal Comp from source

The plugin is a CMake project. JUCE 8.0.15 is downloaded automatically at configure time
(`FetchContent`), so the only things you need are a compiler, CMake and JUCE's Linux dependencies.

## 1. Dependencies

You need CMake ≥ 3.22, a C++17 compiler (GCC 13 or newer is what releases are built with), Ninja
(optional, but faster), git, and the X11 / ALSA / FreeType / fontconfig development headers.

**Ubuntu / Debian:**

```bash
sudo apt install build-essential cmake ninja-build git pkg-config libasound2-dev libfreetype-dev libfontconfig1-dev libx11-dev libxcomposite-dev libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev libgl1-mesa-dev
```

**Fedora:**

```bash
sudo dnf install gcc-c++ cmake ninja-build git pkgconf alsa-lib-devel freetype-devel fontconfig-devel libX11-devel libXcomposite-devel libXcursor-devel libXext-devel libXinerama-devel libXrandr-devel libXrender-devel mesa-libGL-devel
```

### Immutable distributions (Bazzite, Silverblue, Kinoite, …)

Don't layer build tools onto the host. Build inside a [distrobox](https://distrobox.it) container
instead. Your home folder is shared with the container, so the finished plugin is visible on the
host. The helper script creates an Ubuntu 24.04 box and installs the dependencies listed above:

```bash
bash scripts/setup-toolbox.sh
```

The helper scripts `scripts/build.sh`, `scripts/test.sh` and `scripts/setup-toolbox.sh` run their
commands inside a distrobox named `juce-build`. You don't have to use them: the plain CMake commands
below work the same on any system, inside or outside a container.

## 2. Build

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build -j"$(nproc)"
```

The plugin bundle ends up in:

```
build/DuoVocalComp_artefacts/Release/VST3/Duo Vocal Comp.vst3
```

Useful CMake options:

| Option | Default | Effect |
|---|---|---|
| `DUO_BUILD_TESTS` | `ON` | Build the `DuoDspTests` and `DuoPluginTests` suites |
| `DUO_BUILD_SNAPSHOT` | `ON` | Build `DuoGuiSnapshot`, which renders the editor to PNG headlessly |
| `DUO_SANITIZE` | `OFF` | Build the test suites with AddressSanitizer + UBSan |

## 3. Test

```bash
cmake --build build --target DuoDspTests DuoPluginTests -j"$(nproc)"
ctest --test-dir build --output-on-failure
```

`dsp_suite` runs every measurement from `docs/ENGINEERING_SPEC.md` §3 at 44.1–192 kHz, mono and
stereo, at several block sizes, and rewrites `tests/report.md` with the results (including a CPU
benchmark). `plugin_suite` checks parameters, saved state, latency reporting, presets and A/B.

With the distrobox helper scripts, the same thing is:

```bash
bash scripts/build.sh
bash scripts/test.sh
bash scripts/test.sh --asan
```

`scripts/pluginval.sh` downloads [pluginval](https://github.com/Tracktion/pluginval) into `tools/`
and validates the Release build at strictness level 10.

## 4. Install your build

```bash
bash scripts/install.sh
```

This copies the bundle to `~/.vst3` and checks that no system libraries are missing. Restart your
DAW afterwards so it rescans plugins. `scripts/check-reaper.sh` checks that REAPER's VST path
includes `~/.vst3` and that REAPER has scanned the plugin.

## Project layout

```
CMakeLists.txt            JUCE via FetchContent, VST3 target + test targets
Source/Dsp.h              JUCE-free DSP building blocks (FET, opto, filters)
Source/Engine.h           JUCE-free engine: oversampling, stage order, mix, metering
Source/Oversampling.h     JUCE-free polyphase oversampling filters
Source/Presets.h          factory presets
Source/PluginProcessor.*  thin JUCE wrapper: parameters, state, presets, latency
Source/PluginEditor.*     GUI
tests/                    DuoDspTests (plain C++17), DuoPluginTests, GUI snapshot tool
docs/ENGINEERING_SPEC.md  quality targets
docs/DECISIONS.md         design decisions, each backed by a measured number
```

## Rules for contributors

- **Parameter IDs are frozen**, so saved projects keep loading: `fetOn fetIn fetOut fetAtk fetRel
  fetRatio optoOn optoPR optoGain optoMode order scHpf color mix out` (version 1) and
  `link optoEmph os scListen autoGain` (version 2). New parameters may be added with a higher
  version number; existing ones are never renamed, removed or reordered.
- `processBlock` must be real-time safe: no allocation, locks, I/O or exceptions.
- `Source/Dsp.h`, `Source/Engine.h` and `Source/Oversampling.h` stay free of JUCE so the DSP can be
  tested with plain g++.
- Latency must always be reported correctly, including when the oversampling factor changes.
- Keep both test suites green.
