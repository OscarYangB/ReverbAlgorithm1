#include "MultichannelDelay.h"

MultichannelDelay::MultichannelDelay(const int sampleRate, const float feedbackMultiplier, const std::vector<float> delayLengths)
{
	delays.reserve(delayLengths.size());

	for (int i = 0; i < delayLengths.size(); i++) {
		delays.push_back(Delay(delayLengths[i], sampleRate, feedbackMultiplier));
	}
}

float MultichannelDelay::processSampleMultichannel(const float sample)
{
	float result = 0.0f;
	for (int i = 0; i < delays.size(); i++) {
		result += processSample(i, sample);
	}
	return result;
}

float MultichannelDelay::processSample(const int channel, const float sample)
{
	if (channel < 0 || channel >= delays.size()) return 0.0f;
	
	return delays[channel].processSample(sample);
}

int MultichannelDelay::getNumberOfChannels()
{
	return delays.size();
}
