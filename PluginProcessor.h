#ifndef PLUGINPROCESSOR_H
#define PLUGINPROCESSOR_H

#include <juce_audio_processors/juce_audio_processors.h>

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
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    float m_a0;
    float m_a1;
    float m_a2;
    float m_b1;
    float m_b2;
    
    // formulas
    Coefficients calculateHPF();
    Coefficients calculateBPF();
    Coefficients calculateLPF(double frequency);

private:
    //==============================================================================
    float m_z_1_a[2];
    float m_z_2_a[2];
    float m_z_1_b[2];
    float m_z_2_b[2];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LearningBiquadAudioProcessor)
};

#endif