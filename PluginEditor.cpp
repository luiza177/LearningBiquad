#include "PluginEditor.h"

//==============================================================================
LearningBiquadAudioProcessorEditor::LearningBiquadAudioProcessorEditor (LearningBiquadAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 300);
    
    auto coefSliders = getCoefficientSliders();
    auto coefLabels = getCoefficientLabels();
    
    for (auto* slider : coefSliders)
    {
        slider->setSliderStyle(juce::Slider::LinearVertical);
        slider->setRange(-1.0, 1.0, 0.0);
        slider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
        slider->setValue(0.0);
        slider->setNumDecimalPlacesToDisplay(2);
        slider->setDoubleClickReturnValue(true, 0.0);
        addAndMakeVisible(slider);
    }

    // m_a0_slider.setSliderStyle(juce::Slider::LinearVertical);
    // m_a0_slider.setRange(-1.0, 1.0, 0.0);
    // m_a0_slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    // m_a0_slider.setValue(0.0);
    // m_a0_slider.setNumDecimalPlacesToDisplay(2);
    // m_a0_slider.setDoubleClickReturnValue(true, 0.0);
    m_a0_slider.onValueChange = [this](){ 
        processorRef.m_f_a0 = static_cast<float>(m_a0_slider.getValue());
    };
    addAndMakeVisible(m_a0_slider);

    m_a0_label.setText("a0", juce::dontSendNotification);
    m_a0_label.attachToComponent(&m_a0_slider, false);
    m_a0_label.setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(m_a0_label);

    // m_a1_slider.setSliderStyle(juce::Slider::LinearVertical);
    // m_a1_slider.setRange(-1.0, 1.0, 0.0);
    // m_a1_slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    // m_a1_slider.setValue(0.0);
    // m_a1_slider.setNumDecimalPlacesToDisplay(2);
    // m_a1_slider.setDoubleClickReturnValue(true, 0.0);
    m_a1_slider.onValueChange = [this](){ processorRef.m_f_a1 = static_cast<float>(m_a1_slider.getValue());};
    
    addAndMakeVisible(m_a1_slider);

    m_a1_label.setText("a1", juce::dontSendNotification);
    m_a1_label.attachToComponent(&m_a1_slider, false);
    m_a1_label.setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(m_a1_label);
    
    // m_a2_slider.setSliderStyle(juce::Slider::LinearVertical);
    // m_a2_slider.setRange(-1.0, 1.0, 0.0);
    // m_a2_slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    // m_a2_slider.setValue(0.0);
    // m_a2_slider.setNumDecimalPlacesToDisplay(2);
    // m_a2_slider.setDoubleClickReturnValue(true, 0.0);
    m_a2_slider.onValueChange = [this](){ processorRef.m_f_a2 = static_cast<float>(m_a2_slider.getValue());};
    addAndMakeVisible(m_a2_slider);

    m_a2_label.setText("a2", juce::dontSendNotification);
    m_a2_label.attachToComponent(&m_a2_slider, false);
    m_a2_label.setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(m_a2_label);
    
    // m_b1_slider.setSliderStyle(juce::Slider::LinearVertical);
    // m_b1_slider.setRange(-1.0, 1.0, 0.0);
    // m_b1_slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    // m_b1_slider.setValue(0.0);
    // m_b1_slider.setNumDecimalPlacesToDisplay(2);
    // m_b1_slider.setDoubleClickReturnValue(true, 0.0);
    m_b1_slider.onValueChange = [this](){ processorRef.m_f_b1 = static_cast<float>(m_b1_slider.getValue());};
    addAndMakeVisible(m_b1_slider);

    m_b1_label.setText("b1", juce::dontSendNotification);
    m_b1_label.attachToComponent(&m_b1_slider, false);
    m_b1_label.setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(m_b1_label);
    
    // m_b2_slider.setSliderStyle(juce::Slider::LinearVertical);
    // m_b2_slider.setRange(-1.0, 1.0, 0.0);
    // m_b2_slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    // m_b2_slider.setValue(0.0);
    // m_b2_slider.setNumDecimalPlacesToDisplay(2);
    // m_b2_slider.setDoubleClickReturnValue(true, 0.0);
    m_b2_slider.onValueChange = [this](){ processorRef.m_f_b2 = static_cast<float>(m_b2_slider.getValue());};
    addAndMakeVisible(m_b2_slider);

    m_b2_label.setText("b2", juce::dontSendNotification);
    m_b2_label.attachToComponent(&m_b2_slider, false);
    m_b2_label.setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(m_b2_label);
}


LearningBiquadAudioProcessorEditor::~LearningBiquadAudioProcessorEditor()
{
}

//==============================================================================
void LearningBiquadAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    // g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void LearningBiquadAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    auto label = 30;
    area.reduce(30, 20); // with outer padding
    area.removeFromTop(label);
    auto sliderWidth = static_cast<int>(area.getWidth()/5);

    m_a0_slider.setBounds(area.removeFromLeft(sliderWidth));
    m_a1_slider.setBounds(area.removeFromLeft(sliderWidth));
    m_a2_slider.setBounds(area.removeFromLeft(sliderWidth));
    m_b1_slider.setBounds(area.removeFromLeft(sliderWidth));
    m_b2_slider.setBounds(area.removeFromLeft(sliderWidth));
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