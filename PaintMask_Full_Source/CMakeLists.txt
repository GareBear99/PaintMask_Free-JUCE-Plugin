cmake_minimum_required(VERSION 3.22)
project(PaintMask VERSION 0.7.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED JUCE_DIR)
    message(WARNING "JUCE_DIR not set. Configure with -DJUCE_DIR=/path/to/JUCE or add_subdirectory(JUCE).")
endif()

if(EXISTS "${JUCE_DIR}/CMakeLists.txt")
    add_subdirectory(${JUCE_DIR} JUCE)
else()
    find_package(JUCE CONFIG REQUIRED)
endif()

juce_add_plugin(PaintMask
    COMPANY_NAME "Gary Doman"
    BUNDLE_ID com.garydoman.paintmask
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT TRUE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE GDom
    PLUGIN_CODE Pmsk
    FORMATS AU VST3 Standalone
    PRODUCT_NAME "PaintMask")

juce_generate_juce_header(PaintMask)

target_sources(PaintMask PRIVATE
    Source/PluginEntry.cpp
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/PaintCanvas.cpp
    Source/PaintCanvas.h
    Source/ColorMapper.cpp
    Source/ColorMapper.h
    Source/ScanEngine.cpp
    Source/ScanEngine.h
    Source/TrialManager.cpp
    Source/TrialManager.h
    Source/StateSerializer.cpp
    Source/StateSerializer.h
    Source/PreviewSynth.cpp
    Source/PreviewSynth.h
    Source/LayerManager.cpp
    Source/LayerManager.h
    Source/PatternMutationEngine.cpp
    Source/PatternMutationEngine.h
    Source/EuclideanRhythm.cpp
    Source/EuclideanRhythm.h
    Source/AudioToPaintAnalyzer.cpp
    Source/AudioToPaintAnalyzer.h
    Source/GestureBrushEngine.cpp
    Source/GestureBrushEngine.h
    Source/PerformanceMode.cpp
    Source/PerformanceMode.h
    Source/PresetLibrary.cpp
    Source/PresetLibrary.h
    Source/ScanVisualizer.cpp
    Source/ScanVisualizer.h
)

target_compile_definitions(PaintMask PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)

target_link_libraries(PaintMask PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_dsp
    juce::juce_gui_extra
    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags)

# DARPA v0.8 round-trip module
# Add to target sources if not already present:
# Source/MidiToPaintImporter.cpp
