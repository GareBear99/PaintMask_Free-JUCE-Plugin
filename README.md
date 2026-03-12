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

Unlock everything for $1/month.

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

• subscribe to Studio for $1/month  
• share it with other producers  
• create brush packs  


--------------------------------------------------

AUTHOR

Gary Doman

Creator of experimental audio tools and procedural music systems.


--------------------------------------------------

PHILOSOPHY

PaintMask is not meant to replace traditional sequencing.

It exists to explore a different creative workflow.

Painting sound instead of programming it.
