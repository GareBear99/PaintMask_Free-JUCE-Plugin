# PaintMask DARPA v0.7 — Integration Status

This handoff is stronger than v0.6 because several premium-path modules are now connected to the main plugin surface.

Integrated in this pass:
- PreviewSynth is now prepared, reset, driven by MIDI, and rendered into the audio buffer.
- APVTS now includes a preview gain parameter.
- Mutation buttons are wired into the editor.
- Default layers are seeded in the processor for future multi-layer routing.
- The scan overlay is pushed onto the canvas on a timer.
- CMake now references the premium-path module sources.

Not honestly complete yet:
- No local JUCE compile happened here.
- No AU/VST3 host validation happened here.
- Layer-specific UI is still seeded/foundation level.
- Audio-to-paint is still code-level only, not a full import workflow.
- Licensing remains a local scaffold, not a real entitlement backend.
