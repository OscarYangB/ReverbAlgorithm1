#include "Delay.h"
#include "math.h"

int Delay::getNextWriteIndex()
{
	return (writeIndex + 1) % getDelayBufferSize();
}

int Delay::getDelayBufferSize()
{
	return sampleDelay + 1;
}

Delay::Delay(const float delaySeconds, const int sampleRate)
{
	const float inSampleDelay = delaySeconds * sampleRate;
	sampleDelay = round(inSampleDelay);
	if (getDelayBufferSize() < 0) return;
	delayBuffer.assign(getDelayBufferSize(), 0.0f);
}

float Delay::processSample(const float sample)
{
	writeIndex = getNextWriteIndex();
	const int readIndex = getNextWriteIndex();

	delayBuffer[writeIndex] = sample;
	processFeedback(delayBuffer[readIndex] * feedbackMultiplier);
	return delayBuffer[readIndex];
}

void Delay::processFeedback(const float sample)
{
	delayBuffer[writeIndex] += sample;
}
