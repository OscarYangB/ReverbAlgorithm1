#pragma once
#include <vector>

class Delay
{
private:
	int sampleDelay = 8;
	int getDelayBufferSize();
	std::vector<float> delayBuffer;
	int writeIndex = 0;
	float feedbackMultiplier = 0.0f;

public:
	Delay() : Delay(0.5f, 44100) {};
	Delay(const float delaySeconds, const int sampleRate);
	Delay(const float delaySeconds, const int sampleRate, const float inFeedbackMultiplier) : Delay(delaySeconds, sampleRate) {
		feedbackMultiplier = inFeedbackMultiplier;
	};

	float processSample(const float sample);
};

