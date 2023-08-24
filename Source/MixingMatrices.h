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
};