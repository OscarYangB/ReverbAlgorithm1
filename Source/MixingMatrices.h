#pragma once
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

class MixingMatrices
{
public:
	static void MixingMatrices::Householder(std::vector<float>& samples) {
		const float multiplier = -2.0f / samples.size();
		float sum = MixDown(samples);
		sum *= multiplier;
		for (int i = 0; i < samples.size(); ++i) {
			samples[i] += sum;
		}
	}

	static float MixDown(std::vector<float>& samples, float scale = 1.0f) {
		float sum = 0.0f;
		for (int i = 0; i < samples.size(); ++i) {
			sum += samples[i] * scale;
		}
		return sum;
	}

	static void Shuffle(std::vector<float>& samples) {
		auto rng = std::default_random_engine{};
		std::shuffle(samples.begin(), samples.end(), rng);

		for (int i = 0; i < samples.size(); ++i) {
			const bool flip = i % 2;
			if (!flip) continue;
			samples[i] *= -1;
		}
	}

	static void Hadamard(std::vector<float>& samples) {
		HadamardRecursiveStep(samples, 0, samples.size() - 1);

		float scalingFactor = std::sqrt(1.0f / samples.size());
		for (int i = 0; i < samples.size(); ++i) samples[i] *= scalingFactor;
	}

private:
	static void HadamardRecursiveStep(std::vector<float>& samples, const int start, const int end) {
		const int size = (end - start) + 1;
		if (size <= 1) return;

		const int halfSize = size / 2;

		HadamardRecursiveStep(samples, start, start + halfSize - 1);
		HadamardRecursiveStep(samples, start + halfSize, end);

		for (int i = start; i < start + halfSize; ++i) {
			const float fromFirstHalf = samples[i];
			const float fromSecondHalf = samples[i + halfSize];
			samples[i] = fromFirstHalf + fromSecondHalf;
			samples[i + halfSize] = fromFirstHalf - fromSecondHalf;
		}
	}
};