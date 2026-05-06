#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       parameters(*this, nullptr, "parameters", createParameterLayout())
{
    /* Add parameter listeners */
    parameters.addParameterListener("Envelope Follower Attack", this);
    parameters.addParameterListener("Envelope Follower Decay", this);
    parameters.addParameterListener("Envelope Follower Amount", this);
    parameters.addParameterListener("Filter Center Frequency", this);
    parameters.addParameterListener("Filter Renonance", this);
    parameters.addParameterListener("Filter Morph", this);
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (samplesPerBlock);

    /* Prepare the L & R autowah objects */
    leftWah.prepare(sampleRate);
    rightWah.prepare(sampleRate);

    /* Update initial autowah settings */
    updateAllWahSettings();
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    /* Obtain The Left And Right Audio Data Pointers */
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);

    for (int i = 0;i < buffer.getNumSamples();i++)
    {
        buffer.setSample(0, i, leftWah.process(leftChannel[i]));
        buffer.setSample(1, i, rightWah.process(rightChannel[i]));
    }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    copyXmlToBinary (*parameters.copyState().createXml(), destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName(parameters.state.getType()))
        {
            parameters.replaceState(juce::ValueTree::fromXml (*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    /* Envelope follower parameters */
    layout.add(std::make_unique<juce::AudioParameterFloat>("Envelope Follower Attack",
                                                           "Envelope Follower Attack",
                                                           juce::NormalisableRange<float>(0.1f, 5000.0f, 0.1f, 0.5f),
                                                           100.0f
                                                          ));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Envelope Follower Decay",
                                                           "Envelope Follower Decay",
                                                           juce::NormalisableRange<float>(10.0f, 2000.0f, 0.1f, 0.5f),
                                                           500.0f
                                                          ));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Envelope Follower Amount",
                                                           "Envelope Follower Amount",
                                                           /* Ensure that the skewing amount is in the middle. */
                                                           []{
                                                               juce::NormalisableRange<float> r(-20.0f, 20.0f, 1.0f);
                                                               r.setSkewForCentre(0.0f);
                                                               return r;
                                                           }(),
                                                           1.0f
                                                          ));

    /* Filter parameters */
    layout.add(std::make_unique<juce::AudioParameterFloat>("Filter Center Frequency",
                                                           "Filter Center Frequency",
                                                           juce::NormalisableRange<float>(50.0f, 15000.0f, 1.0f, 0.5f),
                                                           50.0f
                                                          ));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Filter Renonance",
                                                           "Filter Renonance",
                                                           juce::NormalisableRange<float>(0.1f, 10.0f, 0.01f, 0.5f),
                                                           0.7f
                                                          ));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Filter Morph",
                                                           "Filter Morph",
                                                           juce::NormalisableRange<float>(0.0f, 1.0f, 0.1f, 1.0f),
                                                           0.5f
                                                          ));

    return layout;
}

//==============================================================================
void AudioPluginAudioProcessor::updateAllWahSettings()
{
    wahSettings.envAttack = parameters.getRawParameterValue("Envelope Follower Attack")->load();
    wahSettings.envDecay = parameters.getRawParameterValue("Envelope Follower Decay")->load();
    wahSettings.envAmnt = parameters.getRawParameterValue("Envelope Follower Amount")->load();
    wahSettings.filtFreq = parameters.getRawParameterValue("Filter Center Frequency")->load();
    wahSettings.filtRes = parameters.getRawParameterValue("Filter Renonance")->load();
    wahSettings.filtMorph = parameters.getRawParameterValue("Filter Morph")->load();

    leftWah.updateSettings(wahSettings);
    rightWah.updateSettings(wahSettings);
}

//==============================================================================

void AudioPluginAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "Envelope Follower Attack")
    {
        leftWah.updateEnvAttack(newValue);
        rightWah.updateEnvAttack(newValue);
    }
    else if (parameterID == "Envelope Follower Decay")
    {
        leftWah.updateEnvDecay(newValue);
        rightWah.updateEnvDecay(newValue);
    }
    else if (parameterID == "Envelope Follower Amount")
    {
        leftWah.updateEnvAmount(newValue);
        rightWah.updateEnvAmount(newValue);
    }
    else if (parameterID == "Filter Center Frequency")
    {
        leftWah.updateFiltFreq(newValue);
        rightWah.updateFiltFreq(newValue);
    }
    else if (parameterID == "Filter Renonance")
    {
        leftWah.updateFiltRes(newValue);
        rightWah.updateFiltRes(newValue);
    }
    else if (parameterID == "Filter Morph")
    {
        leftWah.updateFiltMorph(newValue);
        rightWah.updateFiltMorph(newValue);
    }
}

AutoWah<float>& AudioPluginAudioProcessor::getLeftWahFilter()
{
    return leftWah;
}

AutoWah<float>& AudioPluginAudioProcessor::getRightWahFilter()
{
    return rightWah;
}
