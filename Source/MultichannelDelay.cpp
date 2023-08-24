#include "MultichannelDelay.h"
#include "MixingMatrices.h"

MultichannelDelay::MultichannelDelay(const int sampleRate, const float singlechannelFeedbackMultiplier, const float multichannelFeedbackMultiplier, const std::vector<float> delayLengths)
{
	feedbackMultiplier = multichannelFeedbackMultiplier;

	delays.reserve(delayLengths.size());
	for (int i = 0; i < delayLengths.size(); i++) delays.push_back(Delay(delayLengths[i], sampleRate, singlechannelFeedbackMultiplier));
}

float MultichannelDelay::processSampleMultichannel(const float sample)
{
	std::vector<float> processedChannels;
	processedChannels.reserve(delays.size());

	for (int i = 0; i < delays.size(); ++i) {
		processedChannels.push_back(processSample(i, sample));
	}

	processFeedback(processedChannels);
	float result = MixingMatrices::MixDown(processedChannels);
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

void MultichannelDelay::processFeedback(std::vector<float> samples)
{
	if (feedbackMultiplier <= 0) return;
	if (samples.size() != delays.size()) return;

	for (int i = 0; i < samples.size(); ++i) {
		samples[i] *= feedbackMultiplier;
	}

	std::vector<float> mixedSamples = MixingMatrices::Householder(samples);

	for (int i = 0; i < delays.size(); ++i) {
		delays[i].processFeedback(mixedSamples[i]);
	}
}
