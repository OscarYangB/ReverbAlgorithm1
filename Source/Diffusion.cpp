#include "Diffusion.h"

Diffusion::Diffusion(const int sampleRate, const int channels, const float delayAmount)
{
	std::vector<float> delayLengths;
	delayLengths.reserve(channels);
	channelShuffles.reserve(channels);
	flips.reserve(channels);

	for (int i = 0; i < channels; ++i) {
		delayLengths.push_back((delayAmount / channels) * (i + 1));
		channelShuffles.push_back(channels - 1 - i);
		flips.push_back(false);
	}

	delay = MultichannelDelay(sampleRate, 0.0f, 0.0f, delayLengths);
}

void Diffusion::processSamples(std::vector<float>& samples)
{
	if (samples.size() != delay.getNumberOfChannels()) return;

	delay.processSamplesMultichannel(samples);
	// MixingMatrices::Shuffle(samples, channelShuffles, flips);
	// MixingMatrices::Hadamard(samples);
	//MixingMatrices::Householder(samples);
}

int Diffusion::getNumberOfChannels()
{
	return delay.getNumberOfChannels();
}
