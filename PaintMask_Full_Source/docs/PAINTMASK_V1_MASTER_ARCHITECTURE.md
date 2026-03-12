# PaintMask v1 Master Architecture

PaintMask should feel like a **serious visual composition instrument**, not a gimmick.

## System graph
PaintMask
- Paint Canvas
- Music Mapping
- Scan Engine
- Preview Synth
- Pattern Mutation Engine
- Performance Mode
- State / Presets
- Product Layer

## Recommended implementation order
1. preview synth
2. layer manager
3. gesture brushes
4. pattern mutation engine
5. Euclidean rhythm / lane tools
6. performance mode
7. audio-to-paint import
8. preset marketplace / pack flow
