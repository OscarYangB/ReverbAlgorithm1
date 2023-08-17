#pragma once
class Delay
{
private:
	int sampleDelay = 8;
	int getDelayBufferSize();
	float* delayBuffer = nullptr;
	int writeIndex = 0;
	float feedbackMultiplier = 0.0f;

public:
	~Delay();
	Delay() : Delay(0.5f, 44100) {};
	Delay(const float delaySeconds, const int sampleRate);
	Delay(const float delaySeconds, const int sampleRate, const float inFeedbackMultiplier) : Delay(delaySeconds, sampleRate) {
		feedbackMultiplier = inFeedbackMultiplier;
	};

	float* processBuffer(const int bufferSize, const float* buffer);
	float processSample(const float sample);
};

