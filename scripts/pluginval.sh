#!/usr/bin/env bash
# Downloads pluginval (once) and validates the built plugin at strictness 10.
set -euo pipefail
cd "$(dirname "$0")/.."
mkdir -p tools
if [ ! -x tools/pluginval ]; then
  curl -fL -o tools/pluginval.zip https://github.com/Tracktion/pluginval/releases/latest/download/pluginval_Linux.zip
  (cd tools && unzip -o pluginval.zip && chmod +x pluginval)
fi
BUNDLE="build/DuoVocalComp_artefacts/Release/VST3/Duo Vocal Comp.vst3"
./tools/pluginval --strictness-level 10 --validate-in-process --output-dir tools/pluginval-logs --validate "$BUNDLE" "$@"
