# ZachTune

Red-and-black JUCE VST3 plugin project.

## Build
GitHub Actions builds the Windows VST3 artifact on every push to `main`, or manually from Actions > Build ZachTune VST3 > Run workflow.

## Current status
This initial build includes the plugin shell and parameter/UI controls for Retune Speed, Humanize, Mix, Key, and Scale. **Pitch-correction DSP is not implemented yet**; audio currently passes through unchanged. Do not treat this version as a working autotune effect.

## Output
The workflow uploads `ZachTune-VST3` as a GitHub Actions artifact when the build succeeds.
