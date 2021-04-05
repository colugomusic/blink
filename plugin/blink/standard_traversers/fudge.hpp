#pragma once

#include "../traverser.hpp"
#include "../cached.hpp"
#include "../envelope_parameter.hpp"
#include "../slider_parameter.hpp"

namespace blink {
namespace std_traversers {

class FudgeCalculator
{
public:

	// We use this for both sample playback and waveform generation. This
	// calculation needs to be fast, preferably O(n) or better.
	//
	// If we are calculating waveforms the start of the waveform might be
	// millions of pixels off the left edge of the screen therefore it is
	// not good enough to simply traverse the entire sample.
	//
	// blink_EnvelopePoint has an unused 'curve' member which could be
	// used to represent an ease-in/ease-out curve from one envelope point
	// to the next. This is not implemented because I do not understand
	// the mathematics involved in calculating the resulting sample
	// position.
	//
	float calculate(float speed, const blink_EnvelopeData* envelope, blink_Position block_position, float* derivative = nullptr)
	{
		return 0.0f;
	}

	void reset()
	{
		segment_start_ = 0.0f;
		point_search_index_ = 0;
	}

private:

	float segment_start_ = 0.0f;
	int point_search_index_ = 0;
};

class Fudge
{
public:

	float get_position(float speed, const blink_EnvelopeData* env_speed, const Traverser& traverser, int sample_offset, float* derivative = nullptr);
	ml::DSPVector get_positions(float speed, const blink_EnvelopeData* env_speed, const Traverser& traverser, int sample_offset, int count, float* derivatives = nullptr);

private:

	FudgeCalculator calculator_;
};

inline float Fudge::get_position(float speed, const blink_EnvelopeData* env_speed, const Traverser& traverser, int sample_offset, float* derivative)
{
	const auto& read_position = traverser.get_read_position();

	if (!env_speed || env_speed->points.count < 1)
	{
		const auto ff = (env_speed ? std::clamp(1.0f, env_speed->min, env_speed->max) : 1.0f) * speed;

		if (derivative) *derivative = ff;

		return (read_position[0] * ff) - float(sample_offset);
	}

	const auto& resets = traverser.get_resets();

	if (resets[0] > 0)
	{
		calculator_.reset();
	}

	return calculator_.calculate(speed, env_speed, read_position[0], derivative) - sample_offset;
}

inline ml::DSPVector Fudge::get_positions(float speed, const blink_EnvelopeData* env_speed, const Traverser& traverser, int sample_offset, int count, float* derivatives)
{
	const auto& read_position = traverser.get_read_position();

	if (!env_speed || env_speed->points.count < 1)
	{
		const auto ff = (env_speed ? std::clamp(1.0f, env_speed->min, env_speed->max) : 1.0f) * speed;

		if (derivatives) ml::storeAligned(ml::DSPVector(ff), derivatives);

		return (read_position * ff) - float(sample_offset);
	}

	const auto& resets = traverser.get_resets();

	ml::DSPVector out;

	for (int i = 0; i < count; i++)
	{
		if (resets[i] > 0)
		{
			calculator_.reset();
		}

		out[i] = calculator_.calculate(speed, env_speed, read_position[i], derivatives ? &(derivatives[i]) : nullptr) - sample_offset;
	}

	return out;
}

}}