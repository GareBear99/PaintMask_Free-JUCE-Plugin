# MIDI Import Workflow v0.8

## Required UI flow
1. user selects MIDI file or drags a clip into the import target
2. plugin parses note events
3. user chooses:
   - loop length
   - note range
   - import mode (Lane / Phrase / Chord)
   - channel handling
   - seed
4. importer builds a PaintDocument
5. document is shown on canvas
6. user can re-export to MIDI and preserve the same musical structure

## Priority integration targets
- PluginEditor: add import button / drop target
- PluginProcessor: route parsed MIDI sequence into importer
- StateSerializer: store import mode + seed for reproducibility
