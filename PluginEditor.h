#ifndef PLUGINEDITOR_H
#define PLUGINEDITOR_H
#include "PluginProcessor.h"


//==============================================================================
class LearningBiquadAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit LearningBiquadAudioProcessorEditor (LearningBiquadAudioProcessor&);
    ~LearningBiquadAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LearningBiquadAudioProcessor& processorRef;
        
    std::vector<juce::Slider*> getCoefficientSliders();
    std::vector<juce::Label*> getCoefficientLabels();
    
    // vertical coef sliders
    juce::Slider m_a0_slider;
    juce::Slider m_a1_slider;
    juce::Slider m_a2_slider;
    juce::Slider m_b1_slider;
    juce::Slider m_b2_slider;

    juce::Label m_a0_label{"a0_label", "a0"};
    juce::Label m_a1_label{"a1_label", "a1"};
    juce::Label m_a2_label{"a2_label", "a2"};
    juce::Label m_b1_label{"b1_label", "b1"};
    juce::Label m_b2_label{"b2_label", "b2"};
    
    // frequency slider
    juce::Slider m_freqSlider{juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextBoxRight};
    juce::Label m_freqLabel{"freq_label", "Frequency"};
    
    // buttons
    juce::TextButton m_hpfButton{"HPF"};
    juce::TextButton m_bpfButton{"BPF"};
    juce::TextButton m_lpfButton{"LPF"};
    
    // functions
    void updateCoefficients(Coefficients coefs);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LearningBiquadAudioProcessorEditor)
};
#endif