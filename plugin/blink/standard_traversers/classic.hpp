#pragma once

#include <algorithm>
#include "../traverser.hpp"
#include "../envelope_parameter.hpp"
#include "../slider_parameter.hpp"

namespace blink {
namespace std_traversers {

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
		return n * math::convert::p_to_ff(min);
	}

	return sum_of_geometric_series(math::convert::p_to_ff(min), r, n);
}

template <class T> T weird_math_that_i_dont_understand_ff(T min, T max, T distance, T n)
{
	return math::convert::p_to_ff(min) * std::pow(ratio(min, max, distance), n);
}

class ClassicCalculator
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
	float calculate(float transpose, const blink_EnvelopeData* envelope, blink_Position block_position, float* derivative = nullptr)
	{
		struct PitchPoint
		{
			int x;
			float pitch;

			PitchPoint(const blink_EnvelopePoint& p, float min, float max, float transpose)
				: x(p.position.x)
				, pitch(std::clamp(p.position.y, min, max) + transpose)
				, ff_(1.0f)
			{
			}

			float get_ff() const
			{
				if (!ff_ready_)
				{
					ff_ = math::convert::p_to_ff(pitch);
					ff_ready_ = true;
				}

				return ff_;
			}

		private:

			mutable bool ff_ready_ = false;
			mutable float ff_;
		};

		for (blink_Index i = point_search_index_; i < envelope->points.count; i++)
		{
			const PitchPoint p1(envelope->points.points[i], envelope->min, envelope->max, transpose);

			if (block_position < p1.x)
			{
				if (i == 0)
				{
					point_search_index_ = 0;

					if (derivative) *derivative = p1.get_ff();

					return float(block_position * p1.get_ff()) + segment_start_;
				}

				PitchPoint p0(envelope->points.points[i - 1], envelope->min, envelope->max, transpose);

				auto n = block_position - p0.x;
				auto segment_size = double(p1.x) - p0.x;

				if (segment_size > 0.0f)
				{
					point_search_index_ = i;

					if (derivative) *derivative = float(weird_math_that_i_dont_understand_ff(double(p0.pitch), double(p1.pitch), segment_size, n));

					return float(weird_math_that_i_dont_understand(double(p0.pitch), double(p1.pitch), segment_size, n)) + segment_start_;
				}
			}
			else
			{
				if (i == 0)
				{
					point_search_index_ = 1;

					segment_start_ += float(p1.x * p1.get_ff()) + segment_start_;
				}
				else
				{
					point_search_index_ = i + 1;

					PitchPoint p0(envelope->points.points[i - 1], envelope->min, envelope->max, transpose);

					auto segment_size = double(p1.x) - p0.x;

					if (segment_size > 0.0f)
					{
						segment_start_ = float(weird_math_that_i_dont_understand(double(p0.pitch), double(p1.pitch), segment_size, segment_size)) + segment_start_;
					}
				}
			}
		}

		PitchPoint p0(envelope->points.points[envelope->points.count - 1], envelope->min, envelope->max, transpose);

		auto n = block_position - p0.x;

		if (derivative) *derivative = p0.get_ff();

		return float(n * p0.get_ff()) + segment_start_;
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

class Classic
{
public:

	ml::DSPVector get_positions(float transpose, const blink_EnvelopeData* env_pitch, const Traverser& traverser, int sample_offset, int count, ml::DSPVector* derivatives = nullptr);

private:

	ClassicCalculator calculator_;
};

inline ml::DSPVector Classic::get_positions(float transpose, const blink_EnvelopeData* env_pitch, const Traverser& traverser, int sample_offset, int count, ml::DSPVector* derivatives)
{
	const auto& block_positions = traverser.block_positions();

	if (!env_pitch || env_pitch->points.count < 1)
	{
		const auto ff = math::convert::p_to_ff((env_pitch ? std::clamp(0.0f, env_pitch->min, env_pitch->max) : 0.0f) + transpose);

		if (derivatives) *derivatives = ff;

		return (block_positions.positions * ff) - float(sample_offset);
	}

	const auto& resets = traverser.get_resets();

	ml::DSPVector out;

	for (int i = 0; i < count; i++)
	{
		if (resets[i] > 0)
		{
			calculator_.reset();
		}

		out[i] = calculator_.calculate(transpose, env_pitch, block_positions.positions[i], derivatives ? &(derivatives->getBuffer()[i]) : nullptr) - sample_offset;
	}

	return out;
}

}}