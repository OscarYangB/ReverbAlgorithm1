#pragma once
#include <vector>

class MixingMatrices
{
public:
	static std::vector<float> MixingMatrices::Householder(std::vector<float> samples) {
		const float multiplier = -2.0f / samples.size();
		float sum = MixDown(samples);
		sum *= multiplier;
		for (int i = 0; i < samples.size(); ++i) {
			samples[i] += sum;
		}
		return samples;
	}

	static float MixDown(std::vector<float> samples) {
		float sum = 0.0f;
		for (int i = 0; i < samples.size(); ++i) {
			sum += samples[i];
		}
		return sum;
	}

	static std::vector<float> Shuffle(std::vector<float> samples, std::vector<int> channelShuffles, std::vector<bool> flips) {
		std::vector<float> result(samples.size(), 0.0f);
		if (!(samples.size() == channelShuffles.size() && channelShuffles.size() == flips.size())) return result;

		for (int i = 0; i < samples.size(); ++i) {
			int currentChannel = channelShuffles[i];
			result[currentChannel] = flips[i] ? samples[i] * -1 : samples[i];
		}

		return result;
	}

	static std::vector<float> Hadamard(std::vector<float> samples) {
		std::vector<float> processedSamples = HadamardRecursiveStep(samples);

		float scalingFactor = std::sqrt(1.0f / samples.size());
		for (int i = 0; i < samples.size(); ++i) samples[i] *= scalingFactor;

		return samples;
	}

private:
	static std::vector<float> HadamardRecursiveStep(std::vector<float> samples) {
		if (samples.size() == 1) return samples;

		const int halfSize = samples.size() / 2;

		std::vector<float> firstHalf = HadamardRecursiveStep(std::vector<float>(samples.begin(), samples.begin() + halfSize - 1));
		std::vector<float> secondHalf = HadamardRecursiveStep(std::vector<float>(samples.begin() + halfSize, samples.end()));

		for (int i = 0; i < halfSize; ++i) {
			float fromFirstHalf = samples[i];
			float fromSecondHalf = samples[i + halfSize];
			samples[i] = fromFirstHalf + fromSecondHalf;
			samples[i + halfSize] = fromFirstHalf - fromSecondHalf;
		}

		return samples;
	}
};