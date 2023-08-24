#pragma once
#include "Delay.h"
#include <vector>

class MultichannelDelay
{
private:
	std::vector<Delay> delays;

public:
	MultichannelDelay(const int sampleRate, const float feedbackMultiplier, const std::vector<float> delayLengths);
	MultichannelDelay() : MultichannelDelay(44100, 0.0f, std::vector<float>{}) {};
	float processSampleMultichannel(const float sample);
	float processSample(const int channel, const float sample);
	int getNumberOfChannels();
};

