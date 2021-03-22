#pragma once

#include "../traverser.hpp"
#include "../cached.hpp"
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
		return n * math::p_to_ff(min);
	}

	return sum_of_geometric_series(math::p_to_ff(min), r, n);
}

template <class T> T weird_math_that_i_dont_understand_ff(T min, T max, T distance, T n)
{
	return math::p_to_ff(min) * std::pow(ratio(min, max, distance), n);
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
	float calculate(float transpose, const blink_EnvelopePoints* pitch_points, blink_Position block_position, float* derivative = nullptr)
	{
		struct PitchPointData
		{
			bool set;
			float transpose;
			float pitch;
			float ff;
		};

		struct PitchPoint
		{
			PitchPoint(const blink_EnvelopePoint& p, float transpose)
				: position(p.position)
				, data_((PitchPointData*)(p.plugin_data))
			{
				if (data_->transpose != transpose)
				{
					data_->set = false;
					data_->transpose = transpose;
				}
			}

			void calculate() const
			{
				data_->set = true;
				data_->pitch = position.y + data_->transpose;
				data_->ff = math::p_to_ff(data_->pitch);
			}

			float get_pitch() const
			{
				if (!data_->set) calculate();

				return data_->pitch;
			}

			float get_ff() const
			{
				if (!data_->set) calculate();

				return data_->ff;
			}

			blink_EnvelopePointPosition position;

		private:

			PitchPointData* data_;
		};

		for (blink_Index i = point_search_index_; i < pitch_points->count; i++)
		{
			PitchPoint p1(pitch_points->points[i], transpose);

			if (block_position < p1.position.x)
			{
				if (i == 0)
				{
					point_search_index_ = 0;

					if (derivative) *derivative = p1.get_ff();

					return float(block_position * p1.get_ff()) + segment_start_;
				}

				PitchPoint p0(pitch_points->points[i - 1], transpose);

				auto n = block_position - p0.position.x;
				auto segment_size = double(p1.position.x) - p0.position.x;

				if (segment_size > 0.0f)
				{
					point_search_index_ = i;

					if (derivative) *derivative = float(weird_math_that_i_dont_understand_ff(double(p0.get_pitch()), double(p1.get_pitch()), segment_size, n));

					return float(weird_math_that_i_dont_understand(double(p0.get_pitch()), double(p1.get_pitch()), segment_size, n)) + segment_start_;
				}
			}
			else
			{
				if (i == 0)
				{
					point_search_index_ = 1;

					segment_start_ += float(p1.position.x * p1.get_ff()) + segment_start_;
				}
				else
				{
					point_search_index_ = i + 1;

					PitchPoint p0(pitch_points->points[i - 1], transpose);

					auto segment_size = double(p1.position.x) - p0.position.x;

					if (segment_size > 0.0f)
					{
						segment_start_ = float(weird_math_that_i_dont_understand(double(p0.get_pitch()), double(p1.get_pitch()), segment_size, segment_size)) + segment_start_;
					}
				}
			}
		}

		PitchPoint p0(pitch_points->points[pitch_points->count - 1], transpose);

		auto n = block_position - p0.position.x;

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

	float get_position(float transpose, const blink_EnvelopePoints* env_pitch_points, Traverser* traverser, int sample_offset, float* derivative = nullptr);
	ml::DSPVector get_positions(float transpose, const blink_EnvelopePoints* env_pitch_points, Traverser* traverser, int sample_offset, float* derivatives = nullptr);

private:

	ClassicCalculator calculator_;
	TraverserPointDataResetter traverser_resetter_;
};

inline float Classic::get_position(float transpose, const blink_EnvelopePoints* env_pitch_points, Traverser* traverser, int sample_offset, float* derivative)
{
	const auto& read_position = traverser->get_read_position();

	if (!env_pitch_points || env_pitch_points->count < 1)
	{
		const auto ff = math::p_to_ff(transpose);

		if (derivative) *derivative = ff;

		return (read_position[0] * ff) - float(sample_offset);
	}

	traverser_resetter_.check(env_pitch_points, traverser);

	const auto& resets = traverser->get_resets();

	if (resets[0] > 0)
	{
		calculator_.reset();
	}

	return calculator_.calculate(transpose, env_pitch_points, read_position[0], derivative) - sample_offset;
}

inline ml::DSPVector Classic::get_positions(float transpose, const blink_EnvelopePoints* env_pitch_points, Traverser* traverser, int sample_offset, float* derivatives)
{
	const auto& read_position = traverser->get_read_position();

	if (!env_pitch_points || env_pitch_points->count < 1)
	{
		const auto ff = math::p_to_ff(transpose);

		if (derivatives) ml::storeAligned(ml::DSPVector(ff), derivatives);

		return (read_position * ff) - float(sample_offset);
	}

	traverser_resetter_.check(env_pitch_points, traverser);

	const auto& resets = traverser->get_resets();

	ml::DSPVector out;

	for (int i = 0; i < kFloatsPerDSPVector; i++)
	{
		if (resets[i] > 0)
		{
			calculator_.reset();
		}

		out[i] = calculator_.calculate(transpose, env_pitch_points, read_position[i], derivatives ? &(derivatives[i]) : nullptr) - sample_offset;
	}

	return out;
}

}}