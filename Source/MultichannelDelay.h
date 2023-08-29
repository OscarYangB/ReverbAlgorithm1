#pragma once
#include "Delay.h"
#include <vector>

class MultichannelDelay
{
private:
	std::vector<Delay> delays;
	std::vector<float> feedbackSamples;
	float feedbackMultiplier = 0.0f;

public:
	MultichannelDelay(const int sampleRate, const float singlechannelFeedbackMultiplier, const float multichannelFeedbackMultiplier, const std::vector<float>& delayLengths);
	MultichannelDelay() : MultichannelDelay(44100, 0.0f, 0.0f, std::vector<float>{}) {};
	void processSamplesMultichannel(std::vector<float>& samples);
	void processSampleMultichannel(const float sample);
	float processSample(const int channel, const float sample);
	void processFeedback(const std::vector<float>& samples);
	int getNumberOfChannels();
};

