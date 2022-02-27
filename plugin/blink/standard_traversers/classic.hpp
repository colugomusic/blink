#pragma once

#include <algorithm>
#include "../traverser.hpp"
#include <blink/envelope_data.hpp>
#include <blink/parameters/toggle_data.hpp>
#include <blink/parameters/envelope_parameter.hpp>
#include <blink/parameters/slider_parameter.hpp>
#include "warp.hpp"

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

class ReverseCalculator
{
public:

	blink_Position calculate(const blink_ToggleData* reverse, blink_Position block_position)
	{
		for (blink_Index i = point_search_index_; i < reverse->points.count; i++)
		{
			const auto distance { block_position - segment_start_ };
			const auto direction { i != 0 && reverse->points.data[i - 1].y > 0 ? -1 : 1 };

			if (block_position < reverse->points.data[i].x)
			{
				return segment_start_frame_ + (distance * direction);
			}
			else
			{
				if (i != 0)
				{
					const auto distance { reverse->points.data[i].x - reverse->points.data[i-1].x };

					segment_start_frame_ = segment_start_ + (distance * direction);
				}
				else
				{
					segment_start_frame_ = reverse->points.data[i].x * direction;

				}

				segment_start_ = reverse->points.data[i].x;
				point_search_index_++;
			}
		}

		const auto distance { block_position - segment_start_ };
		const auto direction { reverse->points.data[reverse->points.count - 1].y > 0 ? -1 : 1 };

		return segment_start_frame_ + (distance * direction);
	}

	void reset()
	{
		segment_start_ = 0.0f;
		segment_start_frame_ = 0.0f;
		point_search_index_ = 0;
	}

private:

	blink_Position segment_start_ = 0.0f;
	blink_Position segment_start_frame_ = 0.0f;
	int point_search_index_ = 0;
};

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
	blink_Position calculate(float transpose, const blink_EnvelopeData* envelope, blink_Position block_position, float* derivative = nullptr)
	{
		struct PitchPoint
		{
			int x;
			float pitch;

			PitchPoint(const blink_FloatPoint& p, float min, float max, float transpose)
				: x(p.x)
				, pitch(std::clamp(p.y, min, max) + transpose)
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
			const PitchPoint p1(envelope->points.data[i], envelope->points.min, envelope->points.max, transpose);

			if (block_position < p1.x)
			{
				if (i == 0)
				{
					point_search_index_ = 0;

					if (derivative) *derivative = p1.get_ff();

					return (block_position * p1.get_ff()) + segment_start_;
				}

				PitchPoint p0(envelope->points.data[i - 1], envelope->points.min, envelope->points.max, transpose);

				auto n = block_position - p0.x;
				auto segment_size = double(p1.x) - p0.x;

				if (segment_size > 0.0f)
				{
					point_search_index_ = i;

					if (derivative) *derivative = float(weird_math_that_i_dont_understand_ff(double(p0.pitch), double(p1.pitch), segment_size, n));

					return (weird_math_that_i_dont_understand(double(p0.pitch), double(p1.pitch), segment_size, n)) + segment_start_;
				}
			}
			else
			{
				if (i == 0)
				{
					point_search_index_ = 1;

					segment_start_ += (p1.x * p1.get_ff()) + segment_start_;
				}
				else
				{
					point_search_index_ = i + 1;

					PitchPoint p0(envelope->points.data[i - 1], envelope->points.min, envelope->points.max, transpose);

					auto segment_size = double(p1.x) - p0.x;

					if (segment_size > 0.0f)
					{
						segment_start_ = (weird_math_that_i_dont_understand(double(p0.pitch), double(p1.pitch), segment_size, segment_size)) + segment_start_;
					}
				}
			}
		}

		PitchPoint p0(envelope->points.data[envelope->points.count - 1], envelope->points.min, envelope->points.max, transpose);

		auto n = block_position - p0.x;

		if (derivative) *derivative = p0.get_ff();

		return (n * p0.get_ff()) + segment_start_;
	}

	void reset()
	{
		segment_start_ = 0.0f;
		point_search_index_ = 0;
	}

private:

	blink_Position segment_start_ = 0.0f;
	int point_search_index_ = 0;
};

class Classic
{
public:

	void get_positions(
		float transpose,
		blink::EnvelopeIndexData env_pitch,
		const blink_WarpPoints* warp_points,
		blink::ToggleIndexData toggle_reverse,
		const Traverser& traverser,
		int64_t sample_offset,
		int count,
		snd::transport::DSPVectorFramePosition* out_sculpted_positions,
		snd::transport::DSPVectorFramePosition* out_warped_positions,
		ml::DSPVector* out_derivatives = nullptr);

private:
	
	void get_reversed_positions(
		blink::ToggleIndexData toggle_reverse,
		const Traverser& traverser,
		int count,
		snd::transport::DSPVectorFramePosition* positions);

	void get_sculpted_positions(
		float transpose,
		blink::EnvelopeIndexData env_pitch,
		const Traverser& traverser,
		int count,
		snd::transport::DSPVectorFramePosition* positions,
		ml::DSPVector* derivatives = nullptr);

	void get_warp_positions(
		const blink_WarpPoints* warp_points,
		const Traverser& traverser,
		int count,
		snd::transport::DSPVectorFramePosition* positions,
		ml::DSPVector* derivatives = nullptr);

	ClassicCalculator calculator_;
	BlockPositions sculpted_block_positions_;
	BlockPositions warped_block_positions_;
	Traverser warp_traverser_;
	Traverser reverse_traverser_;
	WarpCalculator warp_calculator_;
	ReverseCalculator reverse_calculator_;
};

inline void Classic::get_sculpted_positions(
	float transpose,
	blink::EnvelopeIndexData env_pitch,
	const Traverser& traverser,
	int count,
	snd::transport::DSPVectorFramePosition *positions,
	ml::DSPVector* derivatives)
{
	const auto& block_positions = traverser.block_positions();

	if (!env_pitch.data || env_pitch.data->points.count < 1)
	{
		const auto ff = math::convert::p_to_ff((env_pitch.data ? std::clamp(0.0f, env_pitch.data->points.min, env_pitch.data->points.max) : 0.0f) + transpose);

		*positions = block_positions.positions * ff;

		if (derivatives) *derivatives = ff;

		return;
	}

	const auto& resets = traverser.get_resets();

	for (int i = 0; i < count; i++)
	{
		if (resets[i] > 0)
		{
			calculator_.reset();
		}

		positions->set(i, calculator_.calculate(transpose, env_pitch.data, block_positions.positions[i], derivatives ? &(derivatives->getBuffer()[i]) : nullptr));
	}
}

inline void Classic::get_warp_positions(
	const blink_WarpPoints* warp_points,
	const Traverser& traverser,
	int count,
	snd::transport::DSPVectorFramePosition* positions,
	ml::DSPVector* derivatives)
{
	const auto& block_positions = traverser.block_positions();

	if (!warp_points || warp_points->count < 1)
	{
		*positions = block_positions.positions;

		if (derivatives) *derivatives = 1.0f;

		return;
	}

	const auto& resets = traverser.get_resets();

	for (int i = 0; i < count; i++)
	{
		if (resets[i] > 0)
		{
			warp_calculator_.reset();
		}

		positions->set(i, warp_calculator_.calculate(warp_points, block_positions.positions[i], derivatives ? &(derivatives->getBuffer()[i]) : nullptr));
	}
}

inline void Classic::get_reversed_positions(
	blink::ToggleIndexData toggle_reverse,
	const Traverser& traverser,
	int count,
	snd::transport::DSPVectorFramePosition* positions)
{
	const auto& block_positions { traverser.block_positions() };

	if (!toggle_reverse.data || toggle_reverse.data->points.count < 1)
	{
		*positions = block_positions.positions;

		return;
	}

	const auto& resets = traverser.get_resets();

	for (int i = 0; i < count; i++)
	{
		if (resets[i] > 0)
		{
			reverse_calculator_.reset();
		}

		positions->set(i, reverse_calculator_.calculate(toggle_reverse.data, block_positions.positions[i]));
	}
}

inline void Classic::get_positions(
	float transpose,
	blink::EnvelopeIndexData env_pitch,
	const blink_WarpPoints* warp_points,
	blink::ToggleIndexData toggle_reverse,
	const Traverser& traverser,
	int64_t sample_offset,
	int count,
	snd::transport::DSPVectorFramePosition* out_sculpted_positions,
	snd::transport::DSPVectorFramePosition* out_warped_positions,
	ml::DSPVector* out_derivatives)
{
	snd::transport::DSPVectorFramePosition sculpted_positions;
	snd::transport::DSPVectorFramePosition warped_positions;
	snd::transport::DSPVectorFramePosition reversed_positions;

	ml::DSPVector sculpted_derivatives;
	ml::DSPVector warped_derivatives;


	get_sculpted_positions(transpose, env_pitch, traverser, count, &sculpted_positions, out_derivatives ? &sculpted_derivatives : nullptr);

	sculpted_positions -= int(sample_offset);

	sculpted_block_positions_(sculpted_positions, 0, count);

	warp_traverser_.generate(sculpted_block_positions_, count);

	get_warp_positions(warp_points, warp_traverser_, count, &warped_positions, out_derivatives ? &warped_derivatives : nullptr);

	warped_block_positions_(warped_positions, 0, count);

	reverse_traverser_.generate(warped_block_positions_, count);

	get_reversed_positions(toggle_reverse, reverse_traverser_, count, &reversed_positions);

	if (out_sculpted_positions) *out_sculpted_positions = sculpted_positions;
	if (out_warped_positions) *out_warped_positions = reversed_positions;
	if (out_derivatives) *out_derivatives = sculpted_derivatives * warped_derivatives;

	



	//if (out_sculpted_positions) *out_sculpted_positions = reversed_positions;
	//if (out_warped_positions) *out_warped_positions = reversed_positions;
	//if (out_derivatives) *out_derivatives = reversed_derivatives;
}

}}