/**************************************************************************************************
This file is part of nyoom.

nyoom is free software: you can redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation, either version 3 of the License,
or (at your option) any later version.

nyoom is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with nyoom.
If not, see <https://www.gnu.org/licenses/>.
**************************************************************************************************/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p,
                                                        juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processorRef (p), m_valueTreeState (vts)
{
    addAndMakeVisible(m_slider);
    m_sliderAttachment.reset(new SliderAttachment (m_valueTreeState, "uSliderPosition", m_slider));

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    m_slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    m_slider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
    setSize (400, 100);
    juce::ignoreUnused (processorRef);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::red);
    g.setFont (25.0f);
    g.drawFittedText ("nyoom", getLocalBounds(), juce::Justification::centredTop, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    m_slider.setSize (400, 100);
}
