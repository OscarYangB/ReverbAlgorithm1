#include "MultichannelDelay.h"
#include "MixingMatrices.h"

MultichannelDelay::MultichannelDelay(const int sampleRate, const float singlechannelFeedbackMultiplier, const float multichannelFeedbackMultiplier, const std::vector<float>& delayLengths)
{
	feedbackMultiplier = multichannelFeedbackMultiplier;

	delays.reserve(delayLengths.size());
	for (int i = 0; i < delayLengths.size(); i++) delays.push_back(Delay(delayLengths[i], sampleRate, singlechannelFeedbackMultiplier));
	feedbackSamples.assign(delayLengths.size(), 0.0f);
}

void MultichannelDelay::processSamplesMultichannel(std::vector<float>& samples)
{
	if (samples.size() != delays.size()) return;

	for (int i = 0; i < samples.size(); ++i) {
		const float processedSample = processSample(i, samples[i]);
		samples[i] = processedSample;
	}

	processFeedback(samples);
}

void MultichannelDelay::processSampleMultichannel(const float sample)
{
	std::vector<float> samples(delays.size(), sample);
	processSamplesMultichannel(samples);
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

void MultichannelDelay::processFeedback(const std::vector<float>& samples)
{
	if (feedbackMultiplier <= 0) return;
	if (samples.size() != delays.size()) return;

	for (int i = 0; i < samples.size(); ++i) {
		feedbackSamples[i] = samples[i];
		feedbackSamples[i] *= feedbackMultiplier;
	}

	MixingMatrices::Householder(feedbackSamples);

	for (int i = 0; i < delays.size(); ++i) {
		delays[i].processFeedback(feedbackSamples[i]);
	}
}
