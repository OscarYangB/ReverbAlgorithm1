#pragma once
class Delay
{
private:
	int sampleDelay = 8;
	int getDelayBufferSize();
	float* delayBuffer = nullptr;

	int writeIndex = 0;
	int getReadIndex();


public:
	~Delay();
	Delay(const float delaySeconds, const int sampleRate);

	float* processBuffer(const int bufferSize, const float* buffer);
	float processSample(const float sample);
};

