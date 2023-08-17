#include "Delay.h"
#include "math.h"

int Delay::getDelayBufferSize()
{
	return sampleDelay + 1;
}

int Delay::getReadIndex()
{
	int readIndex = (writeIndex + 1) % getDelayBufferSize();
	return readIndex;
}

Delay::~Delay()
{
	delete[] delayBuffer;
}

Delay::Delay(const float delaySeconds, const int sampleRate)
{
	const float inSampleDelay = delaySeconds * sampleRate;
	sampleDelay = round(inSampleDelay);
	delayBuffer = new float[getDelayBufferSize()];
}

float* Delay::processBuffer(const int bufferSize, const float* samples)
{
	float* returnBuffer = new float[bufferSize];

	for (int i = 0; i < bufferSize; i++) {
		float sample = samples[bufferSize];
		returnBuffer[i] = processSample(sample);
	}

	return returnBuffer;
}

float Delay::processSample(const float sample)
{
	delayBuffer[writeIndex] = sample;
	int readIndex = getReadIndex();
	writeIndex++;
	writeIndex %= getDelayBufferSize();
	return delayBuffer[readIndex];
}
