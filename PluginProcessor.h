#ifndef PLUGINPROCESSOR_H
#define PLUGINPROCESSOR_H

#include <juce_audio_processors/juce_audio_processors.h>

enum FilterOrder
{
    OnePole = 1,
    TwoPole
};

struct Coefficients
{
    double a0{0.0}, a1{0.0}, a2{0.0}, b1{0.0}, b2{0.0};
};


//==============================================================================
class LearningBiquadAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    LearningBiquadAudioProcessor();
    ~LearningBiquadAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    //==============================================================================
    const juce::String getName() const override { return "Learning Biquad"; }

    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    //==============================================================================
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int index) override { juce::ignoreUnused(index); }
    const juce::String getProgramName (int index) override { { juce::ignoreUnused(index); return {}; }}
    void changeProgramName (int index, const juce::String& newName) override { juce::ignoreUnused (index, newName);}

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // coefficients
    float m_a0;
    float m_a1;
    float m_a2;
    float m_b1;
    float m_b2;
    
    // formulas
    Coefficients calculateHPF(double frequency);
    Coefficients calculateBPF(double frequency);
    Coefficients calculateLPF(double frequency);
    
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    juce::AudioProcessorValueTreeState m_apvts {
        *this, 
        nullptr, 
        juce::Identifier("Parameters"), 
        createParameterLayout()
    };

private:
    //==============================================================================
    // TODO: change to allow more channels? juce::Array??
    float m_z_1_a[2];
    float m_z_2_a[2];
    float m_z_1_b[2];
    float m_z_2_b[2];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LearningBiquadAudioProcessor)
};

#endif