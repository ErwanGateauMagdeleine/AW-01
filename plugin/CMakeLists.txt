cmake_minimum_required(VERSION 3.31)

set(PROJECT_NAME "AD-01")

# Sets the format the plugin will be built
set(FORMATS VST3)

project(${PROJECT_NAME} VERSION 0.0.1)

# No ZERO_CHECK target (it helps bust cache for cmake)
set(CMAKE_SUPPRESS_REGENERATION true)

# Adds all the module sources so they appear correctly in the IDE
# Must be set before JUCE is added as a sub-dir (or any targets are made)
# https://github.com/juce-framework/JUCE/commit/6b1b4cf7f6b1008db44411f2c8887d71a3348889
set_property(GLOBAL PROPERTY USE_FOLDERS YES)

option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Show all module sources in IDE projects" ON)

add_subdirectory(JUCE)

juce_add_binary_data(
    BinaryData
    SOURCES
    src/gui/knob.png
)

juce_add_plugin("${PROJECT_NAME}"
                COMPANY_NAME GateAudio
                IS_SYNTH FALSE
                NEEDS_MIDI_INPUT FALSE
                NEEDS_MIDI_OUTPUT FALSE
                IS_MIDI_EFFECT FALSE
                EDITOR_WANTS_KEYBOARD_FOCUS FALSE
                VST3_COPY_DIR "../OUT/"
                COPY_PLUGIN_AFTER_BUILD TRUE
                PLUGIN_MANUFACTURER_CODE GAIO
                PLUGIN_CODE PIDE
                FORMATS "${FORMATS}"
                PRODUCT_NAME "${PROJECT_NAME}")

target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_20)

set(SourceFiles
    src/PluginEditor.h
    src/PluginProcessor.h
    src/PluginEditor.cpp
    src/PluginProcessor.cpp)

target_sources("${PROJECT_NAME}" PRIVATE ${SourceFiles})

set_target_properties("${PROJECT_NAME}" PROPERTIES FOLDER "")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "" FILES ${SourceFiles})

# Move the INTERFACE auto-created JUCE library stuff into its own folder
source_group("JUCE Library Code" REGULAR_EXPRESSION "juce_")

# We'll need to link to these from our plugin as well as our tests
set(JUCE_DEPENDENCIES
    juce::juce_audio_utils)

target_compile_definitions("${PROJECT_NAME}"
    PUBLIC
    # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries("${PROJECT_NAME}"
    PRIVATE
    ${JUCE_DEPENDENCIES}
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
    BinaryData)

add_library(kissfft STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/external/kissfft/kiss_fft.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/external/kissfft/kiss_fftr.c
)

add_subdirectory(src/tests/Catch2)

add_executable(Catch2Tests
    src/tests/catch_main.cpp
    src/tests/EnvelopeFollowerTests.h
    src/tests/WahFilterTests.h
    src/tests/kissFFTUtility.cpp
)

target_link_libraries(Catch2Tests
    PRIVATE
        Catch2::Catch2WithMain
        kissfft
)

target_include_directories(Catch2Tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dsp/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/external/kissfft
)

enable_testing()

add_test(NAME CATCH2_TESTS COMMAND Catch2Tests)