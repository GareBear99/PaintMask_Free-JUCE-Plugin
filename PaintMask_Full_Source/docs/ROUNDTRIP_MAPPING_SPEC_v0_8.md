# PaintMask Round-Trip Mapping Spec v0.8

Goal: make **Paint → MIDI → Paint** deterministic enough that the same MIDI clip can be re-projected into a canonical visual structure.

## Canonical mapping
- note start beat -> x position (0..1 over loop length)
- note pitch -> y position (normalized over selected note range)
- note duration -> stroke length
- note velocity -> opacity / brightness
- note channel -> layer index
- simultaneous note groups -> geometry grouping
- import mode + seed -> deterministic projection style

## Import modes
1. Lane Mode
   - each note becomes a point/short line
   - best for drum and step patterns

2. Phrase Mode
   - consecutive notes on the same channel become connected strokes
   - best for melodies / arps

3. Chord Mode
   - simultaneous note groups are clustered into triangle / star / circle structures
   - best for harmonic material

## Determinism rules
- same MIDI data + same import mode + same note range + same seed = same paint result
- no hidden randomness outside the supplied seed
- layer assignment must be channel-first, then fallback by lane grouping
