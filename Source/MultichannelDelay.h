#pragma once
#include "Delay.h"
#include <vector>

class MultichannelDelay
{
private:
	std::vector<Delay> delays;
	float feedbackMultiplier = 0.0f;

public:
	MultichannelDelay(const int sampleRate, const float singlechannelFeedbackMultiplier, const float multichannelFeedbackMultiplier, const std::vector<float> delayLengths);
	MultichannelDelay() : MultichannelDelay(44100, 0.0f, 0.0f, std::vector<float>{}) {};
	float processSampleMultichannel(const float sample);
	float processSample(const int channel, const float sample);
	void processFeedback(std::vector<float> samples);
	int getNumberOfChannels();
};

