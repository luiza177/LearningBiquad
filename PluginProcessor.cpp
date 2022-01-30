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

    m_z_1_a[0] = 0.f;
    m_z_2_a[0] = 0.f;
    m_z_1_b[0] = 0.f;
    m_z_2_b[0] = 0.f;
    m_z_1_a[1] = 0.f;
    m_z_2_a[1] = 0.f;
    m_z_1_b[1] = 0.f;
    m_z_2_b[1] = 0.f;
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
            auto xn_1 = m_z_1_a[channel];
            auto xn_2 = m_z_2_a[channel];
            auto yn_1 = m_z_1_b[channel];
            auto yn_2 = m_z_2_b[channel];
            auto yn = m_a0 * xn + m_a1 * xn_1 + m_a2 * xn_2 - m_b1 * yn_1 - m_b2 * yn_2;
            m_z_2_a[channel] = xn_1;
            m_z_2_b[channel] = yn_1;
            m_z_1_a[channel] = xn;
            m_z_1_b[channel] = yn;
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

//==============================================================================
Coefficients LearningBiquadAudioProcessor::calculateLPF(double frequency)
{
    Coefficients lpf;
    auto omega = juce::MathConstants<double>::twoPi * (frequency / getSampleRate());
    auto sin_omega = std::sin(omega);
    auto cos_omega = std::cos(omega);
    
    //? https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwiMo5bKkdn1AhWoiv0HHSMZBRQQFnoECCQQAQ&url=https%3A%2F%2Fe2e.ti.com%2Fcfs-file%2F__key%2Fcommunityserver-discussions-components-files%2F6%2FConfigure-the-Coefficients-for-Digital-Biquad-Filters-in-TLV320AIc3xxx-F_2E00__2E00__2E00_.pdf&usg=AOvVaw1xITX6wqSZTOUlkMWoiDEf
    // auto alpha = sin_omega / 2.f;
    // lpf.a0 = 1 + alpha;
    // lpf.a1 = -2.f * cos_omega;
    // lpf.a2 = 1.f - alpha;
    // auto b0 = (1.f - cos_omega) / 2.f; //?
    // lpf.b1 = 1.f - cos_omega;
    // lpf.b2 = (1.f - cos_omega) / 2.f;
    
    //? compare to:
    auto gamma = cos_omega / (1.f + sin_omega);
    lpf.a0 = (1.f - gamma) / 2.f;
    lpf.a1 = lpf.a0;
    lpf.a2 = 0.f;
    lpf.b1 = -1.f * gamma;
    lpf.b2 = 0.f;
    
    m_a0 = static_cast<float>(lpf.a0);
    m_a1 = static_cast<float>(lpf.a1);
    m_a2 = static_cast<float>(lpf.a2);
    m_b1 = static_cast<float>(lpf.b1);
    m_b2 = static_cast<float>(lpf.b2);
    
    return lpf;
}