# AW-01 - AutoWah JUCE based audio plugin
# Copyright (C) 2026 Erwan Gateau-Magdeleine
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program. If not, see <https://www.gnu.org/licenses/>.

add_executable(juce_pluginTests
    catch_main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/PluginProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/PluginEditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/gui/customLookAndFeel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/gui/CustomRotarySlider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/gui/FilterComponent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/gui/EnvelopeComponent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/gui/ScreenComponent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/gui/FilterCurveDisplay.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/gui/FilterTypeSelector.cpp
)

target_compile_features(juce_pluginTests PRIVATE cxx_std_20)

target_sources(juce_pluginTests PRIVATE ${SourceFiles})

target_link_libraries(juce_pluginTests PRIVATE
    Catch2::Catch2WithMain
    juce::juce_audio_processors
    BINARY_DATA)

target_include_directories(juce_pluginTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../JUCE/modules
)

target_compile_definitions(juce_pluginTests PRIVATE
    JUCE_UNIT_TESTS=1
    JucePlugin_Name="TEST"
)

add_test(NAME JUCE_PLUGIN_TESTS COMMAND juce_pluginTests)
