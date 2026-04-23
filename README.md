> 🎛️ Part of the [TizWildin Plugin Ecosystem](https://garebear99.github.io/TizWildinEntertainmentHUB/) — 16 free audio plugins with a live update dashboard.
>
> [FreeEQ8](https://github.com/GareBear99/FreeEQ8) · [XyloCore](https://github.com/GareBear99/XyloCore) · [Instrudio](https://github.com/GareBear99/Instrudio) · [Therum](https://github.com/GareBear99/Therum_JUCE-Plugin) · [BassMaid](https://github.com/GareBear99/BassMaid) · [SpaceMaid](https://github.com/GareBear99/SpaceMaid) · [GlueMaid](https://github.com/GareBear99/GlueMaid) · [MixMaid](https://github.com/GareBear99/MixMaid) · [ChainMaid](https://github.com/GareBear99/ChainMaid) · [PaintMask](https://github.com/GareBear99/PaintMask_Free-JUCE-Plugin) · [WURP](https://github.com/GareBear99/WURP_Toxic-Motion-Engine_JUCE) · [AETHER](https://github.com/GareBear99/AETHER_Choir-Atmosphere-Designer) · [WhisperGate](https://github.com/GareBear99/WhisperGate_Free-JUCE-Plugin) · [RiftWave](https://github.com/GareBear99/RiftWaveSuite_RiftSynth_WaveForm_Lite) · [FreeSampler](https://github.com/GareBear99/FreeSampler_v0.3) · [VF-PlexLab](https://github.com/GareBear99/VF-PlexLab) · [PAP-Forge-Audio](https://github.com/GareBear99/PAP-Forge-Audio)
>
> 🎁 [Free Packs & Samples](#tizwildin-free-sample-packs) — jump to free packs & samples
>
> 🎵 [Awesome Audio](https://github.com/GareBear99/awesome-audio-plugins-dev) — (FREE) Awesome Audio Dev List

# 🎨 PaintMask


### Paint music with color, geometry, and splashes

PaintMask is a visual MIDI instrument where your painting becomes music.

Instead of placing notes on a piano roll, you paint strokes, splashes, and shapes on a canvas. PaintMask interprets those visuals and converts them into MIDI patterns, rhythms, and modulation.

Brush strokes become melodies.  
Splashes become rhythm clusters.  
Color becomes sound behavior.

PaintMask turns sequencing into creative exploration instead of technical editing.


--------------------------------------------------

FEATURES

PAINT-TO-MIDI CANVAS

Draw directly on the canvas and PaintMask converts strokes into playable MIDI.

Mapping:

X axis → time  
Y axis → pitch  
color → sound behavior  
thickness → note length  
brightness → velocity  


--------------------------------------------------

RGB SOUND ENGINE

Color influences how the sound behaves.

Example mapping:

Red → attack / energy  
Green → groove / sustain  
Blue → space / delay  
Yellow → brightness  
Magenta → modulation  
Cyan → shimmer  
Black → silence / mask  

Blended colors create hybrid musical behavior.


--------------------------------------------------

PAINT SPLASHES

Create scattered rhythm clusters with splatter brushes.

Perfect for:

• hi-hats  
• percussion bursts  
• glitch textures  
• rhythmic layers  


--------------------------------------------------

GEOMETRY SEQUENCERS

Stamp shapes onto the canvas to generate patterns.

Examples:

Circle → looping note clusters  
Triangle → triplet rhythms  
Spiral → pitch runs  
Star → accent bursts  
Polygon → grid rhythms  


--------------------------------------------------

SCAN ENGINE

PaintMask reads your artwork using a moving scan beam.

Scan modes include:

• left → right  
• ping-pong  
• circular  
• spiral  
• wave scan  

Each mode interprets the same painting differently.


--------------------------------------------------

BUILT-IN PREVIEW SYNTH

PaintMask includes a simple internal synth so you can hear results instantly without routing MIDI to another plugin.

Includes:

• oscillator engine  
• filter  
• envelope  
• space effects  


--------------------------------------------------

SEED SYSTEM

Every painting can be replayed with a seed value.

Seeds influence:

• splash spread  
• rhythm density  
• note variation  
• generative pattern behavior  


--------------------------------------------------

TRIAL + STUDIO MODEL

PaintMask uses a simple indie-friendly model.

FULL STUDIO TRIAL

You get 30 days of the full version with everything unlocked.


AFTER 30 DAYS

PaintMask switches to Lite Mode (still usable).

Lite includes:

• paint brush  
• splash brush  
• mask tool  
• RGB sound engine  
• left-to-right scan  
• preview synth  
• MIDI output  


STUDIO MODE

Unlock everything for $2/month.

Studio features include:

• geometry generators  
• modulation painting  
• advanced scan modes  
• brush packs  
• motion layers  
• export tools  
• preset packs  


--------------------------------------------------

WHY PAINTMASK EXISTS

Most music software makes you edit notes like spreadsheets.

PaintMask asks a different question:

What if sequencing felt like painting?

The goal is to create a tool that feels like:

• generative art  
• musical sketching  
• creative experimentation  

instead of a traditional sequencer.


--------------------------------------------------

REPOSITORY STRUCTURE

PaintMask
Source
PaintCanvas
ScanEngine
ColorMapper
TrialManager
StateSerializer

presets

addons

docs


--------------------------------------------------

ADDON SYSTEM

PaintMask supports addon packs.

Addons can include:

• new brush tools  
• geometry generators  
• preset libraries  
• visual themes  


Example structure:

addons/
geometry_pack/
brushes.json
patterns.json


--------------------------------------------------

BUILD REQUIREMENTS

PaintMask uses JUCE + CMake.

Requirements:

• JUCE framework  
• CMake  
• modern C++ compiler  


Build example:

git clone https://github.com/GareBear99/PaintMask_JUCE-Plugin.git 

cd PaintMask
mkdir build
cd build
cmake ..
cmake --build .


Outputs:

• VST3  
• AU  
• Standalone  


--------------------------------------------------

SUPPORT DEVELOPMENT

PaintMask is an indie project.

If you enjoy it:

• subscribe to Studio for $2/month  
• share it with other producers  
• create brush packs  


--------------------------------------------------

AUTHOR

Gary Doman/TizWildin

Creator of experimental audio tools and procedural music systems.


--------------------------------------------------

PHILOSOPHY

PaintMask is not meant to replace traditional sequencing.

It exists to explore a different creative workflow.

Painting sound instead of programming it.

## TizWildin FREE sample packs

| Pack | Description |
|------|-------------|
| [**TizWildin-Aurora**](https://github.com/GareBear99/TizWildin-Aurora) | 3-segment original synth melody pack with loops, stems, demo renders, and neon/cinematic phrasing |
| [**TizWildin-Obsidian**](https://github.com/GareBear99/TizWildin-Obsidian) | Dark cinematic sample pack with choir textures, menu loops, transitions, bass, atmosphere, drums, and electric-banjo extensions |
| [**TizWildin-Skyline**](https://github.com/GareBear99/TizWildin-Skyline) | 30 BPM-tagged synthwave and darkwave loops with generator snapshot and dark neon additions |
| [**TizWildin-Chroma**](https://github.com/GareBear99/TizWildin-Chroma) | Multi-segment game synthwave loop sample pack from TizWildin Entertainment |
| [**TizWildin-Chime**](https://github.com/GareBear99/TizWildin-Chime) | Multi-part 88 BPM chime collection spanning glass, void, halo, reed, and neon synthwave lanes |
| [**Free Violin Synth Sample Kit**](https://github.com/GareBear99/Free-Violin-Synth-Sample-Kit) | Physical-model violin sample kit rendered from the Instrudio violin instrument |
| [**Free Dark Piano Sound Kit**](https://github.com/GareBear99/Free-Dark-Piano-Sound-Kit) | 88 piano notes + dark/cinematic loops and MIDI |
| [**Free 808 Producer Kit**](https://github.com/GareBear99/Free-808-Producer-Kit) | 94 hand-crafted 808 bass samples tuned to every chromatic key |
| [**Free Riser Producer Kit**](https://github.com/GareBear99/Free-Riser-Producer-Kit) | 115+ risers and 63 downlifters - noise, synth, drum, FX, cinematic |
| [**Phonk Producer Toolkit**](https://github.com/GareBear99/Phonk_Producer_Toolkit) | Drift phonk starter kit - 808s, cowbells, drums, MIDI, templates |
| [**Free Future Bass Producer Kit**](https://github.com/GareBear99/Free-Future-Bass-Producer-Kit) | Loops, fills, drums, bass, synths, pads, and FX |

### Related audio projects
- [**VF-PlexLab**](https://github.com/GareBear99/VF-PlexLab) - VocalForge PersonaPlex Lab starter repo for a JUCE plugin + local backend + HTML tester around NVIDIA PersonaPlex.
- [**PAP-Forge-Audio**](https://github.com/GareBear99/PAP-Forge-Audio) - Procedural Autonomous Plugins runtime for generating, branching, validating, and restoring plugin projects from natural-language sound intent.

