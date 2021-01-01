#pragma once

#include "../traverser.hpp"

namespace blkhdgen {
namespace std_traversers {

class Classic : public Traverser
{
public:

	float operator()(float transpose, const blkhdgen_EnvelopePoints* pitch_points, blkhdgen_Position block_position, int sample_offset, float* derivative = nullptr);

private:

	float calculate(float transpose, const blkhdgen_EnvelopePoints* pitch_points, blkhdgen_Position block_position, float* derivative = nullptr);

	float segment_start_;
	int point_search_index_ = 0;
	float prev_pos_ = std::numeric_limits<float>::min();
	const blkhdgen_EnvelopePoints* last_pitch_point_data_ = nullptr;
	mutable blkhdgen_Position last_search_position_ = std::numeric_limits<blkhdgen_Position>::min();
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
// used to represent a ease-in/ease-out curve from one envelope point
// to the next. This is not implemented because I do not understand
// the mathematics involved in calculating the resulting sample
// position.
//

float Classic::calculate(float transpose, const blkhdgen_EnvelopePoints* pitch_points, blkhdgen_Position block_position, float* derivative)
{
	if (!pitch_points || pitch_points->count < 1)
	{
		auto ff = math::p_to_ff(transpose);

		if (derivative) *derivative = ff;

		return float(block_position * ff);
	}

	for (blkhdgen_Index i = point_search_index_; i < pitch_points->count; i++)
	{
		auto p1 = pitch_points->points[i];

		if (block_position < p1.position.x)
		{
			if (i == 0)
			{
				point_search_index_ = 0;

				auto y1 = double(p1.position.y) + transpose;

				if (derivative) *derivative = float(weird_math_that_i_dont_understand_ff(y1, y1, 1.0, block_position));

				return float(weird_math_that_i_dont_understand(y1, y1, 1.0, double(block_position))) + segment_start_;
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

				auto y0 = double(p1.position.y) + transpose;
				auto y1 = double(p1.position.y) + transpose;

				segment_start_ = float(weird_math_that_i_dont_understand(y0, y1, 1.0, double(p1.position.x))) + segment_start_;
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

	auto y0 = double(p0.position.y) + transpose;
	auto y1 = double(p0.position.y) + transpose;

	if (derivative) *derivative = float(weird_math_that_i_dont_understand_ff(y0, y1, 1.0, n));

	return float(weird_math_that_i_dont_understand(y0, y1, 1.0, n)) + segment_start_;
}

float Classic::operator()(float transpose, const blkhdgen_EnvelopePoints* pitch_points, blkhdgen_Position block_position, int sample_offset, float* derivative)
{
	block_position -= data_offset_;

	if (pitch_points != last_pitch_point_data_ || block_position < last_search_position_)
	{
		last_pitch_point_data_ = pitch_points;
		point_search_index_ = 0;
	}

	last_search_position_ = block_position;

	return calculate(transpose, pitch_points, block_position, derivative) + sample_offset;
}

}}