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

std::vector<float> Diffusion::processSamples(std::vector<float> samples)
{
	if (samples.size() != delay.getNumberOfChannels()) return samples;

	samples = delay.processSamplesMultichannel(samples);
	//samples = MixingMatrices::Shuffle(samples, channelShuffles, flips);
	//samples = MixingMatrices::Hadamard(samples);
	samples = MixingMatrices::Householder(samples);
	return samples;
}

int Diffusion::getNumberOfChannels()
{
	return delay.getNumberOfChannels();
}
