#pragma once
#include "MultichannelDelay.h"
#include "MixingMatrices.h"

class Diffusion
{
private:
	MultichannelDelay delay;
	std::vector<int> channelShuffles;
	std::vector<bool> flips;

public:
	Diffusion(const int sampleRate, const int channels, const float delayAmount);
	std::vector<float> processSamples(std::vector<float> samples);
	int getNumberOfChannels();

};

