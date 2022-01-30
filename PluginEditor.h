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
    
    float m_omega{0.f};
    
    std::vector<juce::Slider*> getCoefficientSliders();
    std::vector<juce::Label*> getCoefficientLabels();
    
    // vertical coef sliders
    juce::Slider m_a0_slider;
    juce::Slider m_a1_slider;
    juce::Slider m_a2_slider;
    juce::Slider m_b1_slider;
    juce::Slider m_b2_slider;

    juce::Label m_a0_label;
    juce::Label m_a1_label;
    juce::Label m_a2_label;
    juce::Label m_b1_label;
    juce::Label m_b2_label;
    
    // frequency slider
    juce::Slider m_freqSlider;
    juce::Label m_freqLabel;
    
    // buttons
    // juce::TextButton m_hpfButton;
    // juce::TextButton m_bpfButton;
    // juce::TextButton m_lpfButton;
    
    // formulas
    // void calculateHPF();
    // void calculateBPF();
    // void calculateLPF();


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LearningBiquadAudioProcessorEditor)
};
#endif