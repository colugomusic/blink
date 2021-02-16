#pragma once

#include "../traverser.hpp"
#include "../cached.hpp"
#include "../envelope_parameter.hpp"
#include "../slider_parameter.hpp"

namespace blkhdgen {
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

	ClassicCalculator()
		// These values are expensive to calculate and usually will not change so
		// they are cached instead of calculating them every frame
		: p0_([this]() { return transform(p0_.p.position.y) + transpose_; },
			  [this]() { return math::p_to_ff(p0_.y.get()); })
		, p1_([this]() { return transform(p1_.p.position.y) + transpose_; },
			  [this]() { return math::p_to_ff(p1_.y.get()); })
	{}

	void set_transpose(float transpose)
	{
		if (transpose_ != transpose)
		{
			p0_.set_dirty();
			p1_.set_dirty();

			transpose_ = transpose;
		}
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
	float calculate(const EnvelopeParameter& env_pitch, const blkhdgen_EnvelopePoints* pitch_points, blkhdgen_Position block_position, float* derivative = nullptr)
	{
		env_pitch_ = &env_pitch;

		for (blkhdgen_Index i = point_search_index_; i < pitch_points->count; i++)
		{
			p1_.get(pitch_points, i);

			if (block_position < p1_.p.position.x)
			{
				if (i == 0)
				{
					point_search_index_ = 0;

					if (derivative) *derivative = p1_.ff.get();

					return float(block_position * p1_.ff.get()) + segment_start_;
				}

				p0_.get(pitch_points, i - 1);

				auto n = block_position - p0_.p.position.x;
				auto segment_size = double(p1_.p.position.x) - p0_.p.position.x;

				if (segment_size > 0.0f)
				{
					point_search_index_ = i;

					if (derivative) *derivative = float(weird_math_that_i_dont_understand_ff(double(p0_.y.get()), double(p1_.y.get()), segment_size, n));

					return float(weird_math_that_i_dont_understand(double(p0_.y.get()), double(p1_.y.get()), segment_size, n)) + segment_start_;
				}
			}
			else
			{
				if (i == 0)
				{
					point_search_index_ = 1;

					segment_start_ += float(p1_.p.position.x * p1_.ff.get()) + segment_start_;
				}
				else
				{
					point_search_index_ = i + 1;

					p0_.get(pitch_points, i - 1);

					auto segment_size = double(p1_.p.position.x) - p0_.p.position.x;

					if (segment_size > 0.0f)
					{
						segment_start_ = float(weird_math_that_i_dont_understand(double(p0_.y.get()), double(p1_.y.get()), segment_size, segment_size)) + segment_start_;
					}
				}
			}
		}

		p0_.get(pitch_points, pitch_points->count - 1);

		auto n = block_position - p0_.p.position.x;

		if (derivative) *derivative = p0_.ff.get();

		return float(n * p0_.ff.get()) + segment_start_;
	}

	void reset()
	{
		segment_start_ = 0.0f;
		point_search_index_ = 0;

		p0_.set_dirty();
		p1_.set_dirty();
	}

private:

	float transform(float y) const
	{
		const auto curve = [this](float value)
		{
			return env_pitch_->curve(value);
		};

		const auto& range = env_pitch_->range();

		return math::transform_and_denormalize(curve, range.min().get(), range.max().get(), y);
	}

	struct CachePoint
	{
		Cached<float> y;
		Cached<float> ff;
		blkhdgen_EnvelopePoint p;
		blkhdgen_Index index;

		CachePoint(std::function<float(void)> update_y, std::function<float(void)> update_ff)
			: y(update_y)
			, ff(update_ff)
		{
		}

		bool set_index(blkhdgen_Index new_index)
		{
			if (index != new_index)
			{
				index = new_index;

				y.set_dirty();
				ff.set_dirty();

				return true;
			}

			return false;
		}

		void get(const blkhdgen_EnvelopePoints* points, blkhdgen_Index new_index)
		{
			if (set_index(new_index))
			{
				p = points->points[new_index];
			}
		}

		void set_dirty()
		{
			y.set_dirty();
			ff.set_dirty();
		}
	};

	float transpose_;
	const EnvelopeParameter* env_pitch_;
	float segment_start_ = 0.0f;
	int point_search_index_ = 0;

	CachePoint p0_;
	CachePoint p1_;
};

class Classic
{
public:

	float get_position(float transpose, const EnvelopeParameter& env_pitch, const blkhdgen_EnvelopePoints* env_pitch_points, Traverser* traverser, int sample_offset, float* derivative = nullptr);
	ml::DSPVector get_positions(float transpose, const EnvelopeParameter& env_pitch, const blkhdgen_EnvelopePoints* env_pitch_points, Traverser* traverser, int sample_offset, float* derivatives = nullptr);

private:

	ClassicCalculator calculator_;
	TraverserPointDataResetter traverser_resetter_;
};

inline float Classic::get_position(float transpose, const EnvelopeParameter& env_pitch, const blkhdgen_EnvelopePoints* env_pitch_points, Traverser* traverser, int sample_offset, float* derivative)
{
	const auto& read_position = traverser->get_read_position();

	if (!env_pitch_points || env_pitch_points->count < 1)
	{
		const auto ff = math::p_to_ff(transpose);

		if (derivative) *derivative = ff;

		return float(read_position[0] * ff);
	}

	traverser_resetter_.check(env_pitch_points, traverser);

	const auto& resets = traverser->get_resets();

	if (resets[0] > 0)
	{
		calculator_.reset();
	}

	calculator_.set_transpose(transpose);

	return calculator_.calculate(env_pitch, env_pitch_points, read_position[0], derivative) + sample_offset;
}

inline ml::DSPVector Classic::get_positions(float transpose, const EnvelopeParameter& env_pitch, const blkhdgen_EnvelopePoints* env_pitch_points, Traverser* traverser, int sample_offset, float* derivatives)
{
	const auto& read_position = traverser->get_read_position();

	if (!env_pitch_points || env_pitch_points->count < 1)
	{
		const auto ff = math::p_to_ff(transpose);

		if (derivatives) ml::storeAligned(ml::DSPVector(ff), derivatives);

		return read_position * ff;
	}

	traverser_resetter_.check(env_pitch_points, traverser);

	const auto& resets = traverser->get_resets();

	calculator_.set_transpose(transpose);

	ml::DSPVector out;

	for (int i = 0; i < kFloatsPerDSPVector; i++)
	{
		if (resets[i] > 0)
		{
			calculator_.reset();
		}

		out[i] = calculator_.calculate(env_pitch, env_pitch_points, read_position[i], &(derivatives[i])) + sample_offset;
	}

	return out;
}

}}