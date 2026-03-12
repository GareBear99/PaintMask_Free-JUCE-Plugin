# Implementation Notes — PaintMask DARPA v0.6

## Preview synth
Start lightweight:
- sine / triangle / noise
- one filter
- one envelope

## Layer manager
Store:
- name
- mute / solo
- scan mode
- MIDI channel
- root note
- visibility

## Performance mode
Keep deterministic:
- scene A / B / C snapshots
- layer toggle actions
- momentary density boost
- momentary scan-speed push
