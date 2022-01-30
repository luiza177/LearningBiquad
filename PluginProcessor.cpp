#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================
LearningBiquadAudioProcessor::LearningBiquadAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{
}

LearningBiquadAudioProcessor::~LearningBiquadAudioProcessor()
{
}

//==============================================================================
const juce::String LearningBiquadAudioProcessor::getName() const
{
    return "Learning Biquad";
}

bool LearningBiquadAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LearningBiquadAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LearningBiquadAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LearningBiquadAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LearningBiquadAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LearningBiquadAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LearningBiquadAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String LearningBiquadAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void LearningBiquadAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void LearningBiquadAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);

    m_f_z_1_in[0] = 0.f;
    m_f_z_2_in[0] = 0.f;
    m_f_z_1_out[0] = 0.f;
    m_f_z_2_out[0] = 0.f;
    m_f_z_1_in[1] = 0.f;
    m_f_z_2_in[1] = 0.f;
    m_f_z_1_out[1] = 0.f;
    m_f_z_2_out[1] = 0.f;
}

void LearningBiquadAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool LearningBiquadAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void LearningBiquadAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        // juce::ignoreUnused (channelData);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            auto xn = channelData[sample];
            auto xn_1 = m_f_z_1_in[channel];
            auto xn_2 = m_f_z_2_in[channel];
            auto yn_1 = m_f_z_1_out[channel];
            auto yn_2 = m_f_z_2_out[channel];
            auto yn = m_f_a0 * xn + m_f_a1 * xn_1 + m_f_a2 * xn_2 - m_f_b1 * yn_1 - m_f_b2 * yn_2;
            m_f_z_2_in[channel] = xn_1;
            m_f_z_2_out[channel] = yn_1;
            m_f_z_1_in[channel] = xn;
            m_f_z_1_out[channel] = yn;
            channelData[sample] = yn;
        }
    }
}

//==============================================================================
bool LearningBiquadAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LearningBiquadAudioProcessor::createEditor()
{
    return new LearningBiquadAudioProcessorEditor (*this);
}

//==============================================================================
void LearningBiquadAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void LearningBiquadAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LearningBiquadAudioProcessor();
}