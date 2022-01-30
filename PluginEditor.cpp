#include "PluginEditor.h"

//==============================================================================
LearningBiquadAudioProcessorEditor::LearningBiquadAudioProcessorEditor (LearningBiquadAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);
    
    // TODO: future -> buttons are toggles, one shuts off the others, once set, moving frequency slider, adjusts other sliders. Adjusting sliders untoggles buttons.
    
    for (auto* slider : getCoefficientSliders())
    {
        slider->setSliderStyle(juce::Slider::LinearVertical);
        slider->setRange(-1.0, 1.0, 0.0);
        slider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
        slider->setValue(0.0);
        slider->setNumDecimalPlacesToDisplay(2);
        slider->setDoubleClickReturnValue(true, 0.0);
        addAndMakeVisible(slider);
    }
    
    for (auto* label : getCoefficientLabels())
    {
        label->setJustificationType(juce::Justification::horizontallyCentred);
        addAndMakeVisible(label);
    }

    m_a0_label.attachToComponent(&m_a0_slider, false);
    m_a1_label.attachToComponent(&m_a1_slider, false);
    m_a2_label.attachToComponent(&m_a2_slider, false);
    m_b1_label.attachToComponent(&m_b1_slider, false);
    m_b2_label.attachToComponent(&m_b2_slider, false);
    
    m_a0_slider.onValueChange = [this](){ processorRef.m_a0 = static_cast<float>(m_a0_slider.getValue());};
    m_a1_slider.onValueChange = [this](){ processorRef.m_a1 = static_cast<float>(m_a1_slider.getValue());};
    m_a2_slider.onValueChange = [this](){ processorRef.m_a2 = static_cast<float>(m_a2_slider.getValue());};
    m_b1_slider.onValueChange = [this](){ processorRef.m_b1 = static_cast<float>(m_b1_slider.getValue());};
    m_b2_slider.onValueChange = [this](){ processorRef.m_b2 = static_cast<float>(m_b2_slider.getValue());};
    
    addAndMakeVisible(&m_lpfButton);
    m_lpfButton.onClick = [this]()
    { 
        auto coefs = processorRef.calculateLPF(m_freqSlider.getValue()); 
        m_a0_slider.setValue(coefs.a0);
        m_a1_slider.setValue(coefs.a1);
        m_a2_slider.setValue(coefs.a2);
        m_b1_slider.setValue(coefs.b1);
        m_b2_slider.setValue(coefs.b2);
    };
    
    // m_freqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    m_freqSlider.setRange(20.0, 20000.0);
    m_freqSlider.setNumDecimalPlacesToDisplay(0);
    m_freqSlider.setTextBoxIsEditable(true);
    m_freqSlider.setValue(1000.0);
    m_freqSlider.setDoubleClickReturnValue(true, 1000.0);
    m_freqSlider.setSkewFactorFromMidPoint(2000.0);
    m_freqSlider.setTextValueSuffix("Hz");
    addAndMakeVisible(&m_freqSlider);
    
    m_freqLabel.setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(&m_freqLabel);
}

LearningBiquadAudioProcessorEditor::~LearningBiquadAudioProcessorEditor()
{
}

//==============================================================================
void LearningBiquadAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void LearningBiquadAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto labelAreaHeight = 30;
    
    bounds.reduce(30, 20); // with outer padding
    auto labelArea = bounds.removeFromTop(labelAreaHeight);
    auto buttonArea = bounds.removeFromBottom(100);
    auto freqArea = buttonArea.removeFromTop(70); //TODO: remove magic numbers
    
    auto labelWidth = static_cast<int>(labelArea.getWidth()/5);
    auto sliderWidth = static_cast<int>(bounds.getWidth()/5);
    auto buttonWidth = static_cast<int>(buttonArea.getWidth()/3);

    for (auto* label : getCoefficientLabels())
    {
        label->setBounds(labelArea.removeFromLeft(labelWidth));
    }
    
    for (auto* slider : getCoefficientSliders())
    {
        slider->setBounds(bounds.removeFromLeft(sliderWidth));
    }
    
    m_freqLabel.setBounds(freqArea.removeFromLeft(100)); // TODO: find text width etc
    m_freqSlider.setBounds(freqArea);
    m_lpfButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
}

std::vector<juce::Slider*> LearningBiquadAudioProcessorEditor::getCoefficientSliders()
{
    return {
        &m_a0_slider,
        &m_a1_slider,
        &m_a2_slider,
        &m_b1_slider,
        &m_b2_slider
    };
}

std::vector<juce::Label*> LearningBiquadAudioProcessorEditor::getCoefficientLabels()
{
    return {
        &m_a0_label,
        &m_a1_label,
        &m_a2_label,
        &m_b1_label,
        &m_b2_label
    };
}