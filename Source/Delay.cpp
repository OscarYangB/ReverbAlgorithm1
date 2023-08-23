#include "Delay.h"
#include "math.h"

int Delay::getDelayBufferSize()
{
	return sampleDelay + 1;
}

Delay::~Delay()
{
	if (delayBuffer != nullptr) delete[] delayBuffer;
}

Delay::Delay(const float delaySeconds, const int sampleRate)
{
	const float inSampleDelay = delaySeconds * sampleRate;
	sampleDelay = round(inSampleDelay);
	delayBuffer = new float[getDelayBufferSize()]();
}

float Delay::processSample(const float sample)
{
	const int readIndex = (writeIndex + 1) % getDelayBufferSize();
	delayBuffer[writeIndex] = sample + (delayBuffer[readIndex] * feedbackMultiplier);
	writeIndex = readIndex;
	return delayBuffer[readIndex];
}
