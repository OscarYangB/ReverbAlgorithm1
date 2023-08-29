/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MixingMatrices.h"

//==============================================================================
ReverbAlgorithm1AudioProcessor::ReverbAlgorithm1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

ReverbAlgorithm1AudioProcessor::~ReverbAlgorithm1AudioProcessor()
{
    delete leftDelay;
    delete rightDelay;
}

//==============================================================================
const juce::String ReverbAlgorithm1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReverbAlgorithm1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReverbAlgorithm1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReverbAlgorithm1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReverbAlgorithm1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReverbAlgorithm1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ReverbAlgorithm1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReverbAlgorithm1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ReverbAlgorithm1AudioProcessor::getProgramName (int index)
{
    return {};
}

void ReverbAlgorithm1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ReverbAlgorithm1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    leftDiffusions.reserve(diffusionSteps);
    rightDiffusions.reserve(diffusionSteps);
    for (int i = 0; i < diffusionSteps; ++i) {
        leftDiffusions.push_back(Diffusion(sampleRate, channels, startingDelayAmount * std::pow(2, i + 1)));
        rightDiffusions.push_back(Diffusion(sampleRate, channels, startingDelayAmount * std::pow(2, i + 1)));
    }

    std::vector leftDelayLengths{0.05f, 0.1f, 0.15f, 0.2f};
    std::vector rightDelayLengths{0.05f, 0.1f, 0.15f, 0.2f};
    leftDelay = new MultichannelDelay(sampleRate, 0.0f, 0.5f, leftDelayLengths);
    rightDelay = new MultichannelDelay(sampleRate, 0.0f, 0.5f, rightDelayLengths);

    leftSamples.assign(channels, 0.0f);
    rightSamples.assign(channels, 0.0f);
}

void ReverbAlgorithm1AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReverbAlgorithm1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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
#endif

void ReverbAlgorithm1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if (leftDelay == nullptr || rightDelay == nullptr) return;

    float* leftData = buffer.getWritePointer(0);
    float* rightData = buffer.getWritePointer(1);

    for (int i = 0; i < buffer.getNumSamples(); i++) {

        for (int j = 0; j < channels; ++j) {
            leftSamples[j] = leftData[i];
            rightSamples[j] = rightData[i];
        }

        for (int j = 0; j < leftDiffusions.size(); ++j) {
            leftDiffusions[j].processSamples(leftSamples);
        }

        for (int j = 0; j < rightDiffusions.size(); ++j) {
             rightDiffusions[j].processSamples(rightSamples);
        }

        leftDelay->processSamplesMultichannel(leftSamples);
        rightDelay->processSamplesMultichannel(rightSamples);
        
        leftData[i] = leftSamples[0];
        rightData[i] = rightSamples[0];
    }
}

//==============================================================================
bool ReverbAlgorithm1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ReverbAlgorithm1AudioProcessor::createEditor()
{
    return new ReverbAlgorithm1AudioProcessorEditor (*this);
}

//==============================================================================
void ReverbAlgorithm1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ReverbAlgorithm1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbAlgorithm1AudioProcessor();
}
