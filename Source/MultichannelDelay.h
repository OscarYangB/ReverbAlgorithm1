#pragma once
#include "Delay.h"

class MultichannelDelay
{
private:
	int numberOfChannels = 0;
	Delay** delays = nullptr;

public:
	~MultichannelDelay();
	MultichannelDelay(const int sampleRate, const int inNumberOfChannels, const float feedbackMultiplier, const float* delayLengths);
	MultichannelDelay() : MultichannelDelay(44100, 0, 0.0f, nullptr) {};
	float processSampleMultichannel(float sample);
	float processSample(const int channel, const float sample);
	int getNumberOfChannels();
};

