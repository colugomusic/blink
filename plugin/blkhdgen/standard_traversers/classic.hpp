#pragma once

#include "../traverser.hpp"

namespace blkhdgen {
namespace std_traversers {

class Classic
{
public:

	float get_position(float transpose, const blkhdgen_EnvelopePoints* pitch_points, Traverser* traverser, int sample_offset, float* derivative = nullptr);
	ml::DSPVector get_positions(float transpose, const blkhdgen_EnvelopePoints* pitch_points, Traverser* traverser, int sample_offset, float* derivatives = nullptr);

private:

	float calculate(float transpose, const blkhdgen_EnvelopePoints* pitch_points, blkhdgen_Position position, float* derivative = nullptr);

	float segment_start_ = 0.0f;
	int point_search_index_ = 0;
	TraverserPointDataResetter traverser_resetter_;
};

template <class T> T ratio(T min, T max, T distance)
{
	return std::pow(T(2), ((max - min) / distance) / T(12));
}

template <class T> T sum_of_geometric_series(T first, T ratio, T n)
{
	return first * ((T(1) - std::pow(ratio, n)) / (T(1) - ratio));
}

//
// pull requests extremely welcome
//

template <class T> T weird_math_that_i_dont_understand(T min, T max, T distance, T n)
{
	auto r = ratio(min, max, distance);

	if (std::abs(T(1) - r) <= T(0))
	{
		return n * math::p_to_ff(min);
	}

	return sum_of_geometric_series(math::p_to_ff(min), r, n);
}

template <class T> T weird_math_that_i_dont_understand_ff(T min, T max, T distance, T n)
{
	return math::p_to_ff(min) * std::pow(ratio(min, max, distance), n);
}

// We use this for both sample playback and waveform generation. This
// calculation needs to be fast, preferably O(n) or better.
//
// If we are calculating waveforms the start of the waveform might be
// millions of pixels off the left edge of the screen therefore it is
// not good enough to simply traverse the entire sample.
//
// blkhdgen_EnvelopePoint has an unused 'curve' member which could be
// used to represent an ease-in/ease-out curve from one envelope point
// to the next. This is not implemented because I do not understand
// the mathematics involved in calculating the resulting sample
// position.
//

float Classic::calculate(float transpose, const blkhdgen_EnvelopePoints* pitch_points, blkhdgen_Position block_position, float* derivative)
{
	for (blkhdgen_Index i = point_search_index_; i < pitch_points->count; i++)
	{
		auto p1 = pitch_points->points[i];

		if (block_position < p1.position.x)
		{
			if (i == 0)
			{
				point_search_index_ = 0;

				const auto y1 = double(p1.position.y) + transpose;
				const auto y1_ff = math::p_to_ff(y1);

				if (derivative) *derivative = float(y1_ff);

				return float(block_position * y1_ff) + segment_start_;
			}

			auto p0 = pitch_points->points[i - 1];
			auto n = block_position - p0.position.x;
			auto segment_size = double(p1.position.x) - p0.position.x;

			if (segment_size > 0.0f)
			{
				point_search_index_ = i;

				auto y0 = double(p0.position.y) + transpose;
				auto y1 = double(p1.position.y) + transpose;

				if (derivative) *derivative = float(weird_math_that_i_dont_understand_ff(y0, y1, segment_size, n));

				return float(weird_math_that_i_dont_understand(y0, y1, segment_size, n)) + segment_start_;
			}
		}
		else
		{
			if (i == 0)
			{
				point_search_index_ = 1;

				const auto y1 = double(p1.position.y) + transpose;
				const auto y1_ff = math::p_to_ff(y1);

				segment_start_ += float(p1.position.x * y1_ff) + segment_start_;
			}
			else
			{
				point_search_index_ = i + 1;

				auto p0 = pitch_points->points[i - 1];
				auto segment_size = double(p1.position.x) - p0.position.x;

				if (segment_size > 0.0f)
				{
					auto y0 = double(p0.position.y) + transpose;
					auto y1 = double(p1.position.y) + transpose;

					segment_start_ = float(weird_math_that_i_dont_understand(y0, y1, segment_size, segment_size)) + segment_start_;
				}
			}
		}
	}

	auto p0 = pitch_points->points[pitch_points->count - 1];
	auto n = block_position - p0.position.x;

	const auto y0 = double(p0.position.y) + transpose;
	const auto y0_ff = math::p_to_ff(y0);

	if (derivative) *derivative = float(y0_ff);

	return float(n * y0_ff) + segment_start_;
}

float Classic::get_position(float transpose, const blkhdgen_EnvelopePoints* pitch_points, Traverser* traverser, int sample_offset, float* derivative)
{
	const auto& read_position = traverser->get_read_position();

	if (!pitch_points || pitch_points->count < 1)
	{
		const auto ff = math::p_to_ff(transpose);

		if (derivative) *derivative = ff;

		return float(read_position[0] * ff);
	}

	traverser_resetter_.check(pitch_points, traverser);

	const auto& resets = traverser->get_resets();

	if (resets[0] > 0)
	{
		segment_start_ = 0.0f;
		point_search_index_ = 0;
	}

	return calculate(transpose, pitch_points, read_position[0], derivative) + sample_offset;
}

ml::DSPVector Classic::get_positions(float transpose, const blkhdgen_EnvelopePoints* pitch_points, Traverser* traverser, int sample_offset, float* derivatives)
{
	const auto& read_position = traverser->get_read_position();

	if (!pitch_points || pitch_points->count < 1)
	{
		const auto ff = math::p_to_ff(transpose);

		if (derivatives) ml::storeAligned(ml::DSPVector(ff), derivatives);

		return read_position * ff;
	}

	traverser_resetter_.check(pitch_points, traverser);

	const auto& resets = traverser->get_resets();

	ml::DSPVector out;

	for (int i = 0; i < kFloatsPerDSPVector; i++)
	{
		if (resets[i] > 0)
		{
			segment_start_ = 0.0f;
			point_search_index_ = 0;
		}

		out[i] = calculate(transpose, pitch_points, read_position[i], &(derivatives[i])) + sample_offset;
	}

	return out;
}

}}