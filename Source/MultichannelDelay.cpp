#include "MultichannelDelay.h"

MultichannelDelay::~MultichannelDelay()
{
	for (int i = 0; i < numberOfChannels; i++) {
		delete delays[i];
	}
	if (delays != nullptr) 	delete[] delays;
}

MultichannelDelay::MultichannelDelay(const int sampleRate, const int inNumberOfChannels, const float feedbackMultiplier, const float* delayLengths)
{
	if (inNumberOfChannels <= 0) return;

	numberOfChannels = inNumberOfChannels;

	delays = new Delay*[numberOfChannels]();

	for (int i = 0; i < numberOfChannels; i++) {
		delays[i] = new Delay(delayLengths[i], sampleRate, feedbackMultiplier);
	}
}

float MultichannelDelay::processSampleMultichannel(const float sample)
{
	float result = 0.0f;
	for (int i = 0; i < numberOfChannels; i++) {
		result += processSample(i, sample);
	}
	return result;
}

float MultichannelDelay::processSample(const int channel, const float sample)
{
	if (channel < 0 || channel >= numberOfChannels) return 0.0f;
	
	return delays[channel]->processSample(sample);
}

int MultichannelDelay::getNumberOfChannels()
{
	return numberOfChannels;
}
