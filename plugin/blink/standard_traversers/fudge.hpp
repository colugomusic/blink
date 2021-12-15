#pragma once

#include <algorithm>
#include <blink_sampler.h>
#include "../traverser.hpp"
#include <blink/parameters/envelope_parameter.hpp>
#include <blink/parameters/slider_parameter.hpp>
#include "warp.hpp"

namespace blink {
namespace std_traversers {

template <class T> T quadratic_formula(T a, T b, T c, T n)
{
	return (a * (n * n)) + (b * n) + c;
}

template <class T> T spooky_maths(T f0, T f1, T N, T n, T C)
{
	auto accel = (f1 - f0) / (T(2) * N);

	return quadratic_formula(accel, f0, C, n);
}

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
		struct FFPoint
		{
			int x;
			float y;
			double ff;

			FFPoint(const blink_FloatPoint& p, float min, float max, float speed)
				: x(p.x)
				, y(p.y)
				, ff(double(std::clamp(p.y, min, max)) * speed)
			{
			}
		};

		for (blink_Index i = point_search_index_; i < envelope->points.count; i++)
		{
			const FFPoint p1(envelope->points.points[i], envelope->points.min, envelope->points.max, speed);

			if (block_position < p1.x)
			{
				if (i == 0)
				{
					point_search_index_ = 0;

					if (derivative) *derivative = float(p1.ff);

					return float(spooky_maths(p1.ff, p1.ff, 1.0, block_position, double(segment_start_)));
				}

				FFPoint p0(envelope->points.points[i - 1], envelope->points.min, envelope->points.max, speed);

				auto n = block_position - p0.x;
				auto segment_size = double(p1.x) - p0.x;

				if (segment_size > 0.0f)
				{
					point_search_index_ = i;

					if (derivative) *derivative = float(math::lerp(p0.ff, p1.ff, n / segment_size));

					const auto f0 = p0.ff;
					const auto f1 = p1.ff;

					return float(spooky_maths(f0, f1, segment_size, double(n), double(segment_start_)));
				}
			}
			else
			{
				if (i == 0)
				{
					point_search_index_ = 1;

					const auto f1 = p1.ff;

					segment_start_ = float(spooky_maths(f1, f1, 1.0, double(p1.x), double(segment_start_)));
				}
				else
				{
					point_search_index_ = i + 1;

					FFPoint p0(envelope->points.points[i - 1], envelope->points.min, envelope->points.max, speed);

					auto segment_size = double(p1.x) - p0.x;

					if (segment_size > 0.0f)
					{
						if (p0.y > 0.f || p1.y > 0.f)
						{
							auto f0 = p0.ff;
							auto f1 = p1.ff;

							segment_start_ = float(spooky_maths(f0, f1, segment_size, segment_size, double(segment_start_)));
						}
					}
				}
			}
		}

		FFPoint p0(envelope->points.points[envelope->points.count - 1], envelope->points.min, envelope->points.max, speed);

		auto n = block_position - p0.x;

		if (derivative) *derivative = float(p0.ff);

		if (p0.y == 0.0f)
		{
			return segment_start_;
		}

		return float(spooky_maths(p0.ff, p0.ff, 1.0, double(n), double(segment_start_)));
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

	void get_positions(
		float speed,
		const blink_EnvelopeData* env_speed,
		const blink_WarpPoints* warp_points,
		const Traverser& traverser,
		int sample_offset,
		int count,
		snd::transport::DSPVectorFramePosition* sculpted_positions,
		snd::transport::DSPVectorFramePosition* warped_positions,
		ml::DSPVector* derivatives = nullptr);

private:
	
	void get_sculpted_positions(
		float speed,
		const blink_EnvelopeData* env_speed,
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

	FudgeCalculator calculator_;
	BlockPositions sculpted_block_positions_;
	Traverser warp_traverser_;
	WarpCalculator warp_calculator_;
};

inline void Fudge::get_sculpted_positions(
	float speed,
	const blink_EnvelopeData* env_speed,
	const Traverser& traverser,
	int count,
	snd::transport::DSPVectorFramePosition *positions,
	ml::DSPVector* derivatives)
{
	const auto& block_positions = traverser.block_positions();

	if (!env_speed || env_speed->points.count < 1)
	{
		const auto ff = (env_speed ? std::clamp(1.0f, env_speed->points.min, env_speed->points.max) : 1.0f) * speed;

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

		positions->set(i, calculator_.calculate(speed, env_speed, block_positions.positions[i], derivatives ? &(derivatives->getBuffer()[i]) : nullptr));
	}
}

inline void Fudge::get_warp_positions(
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

inline void Fudge::get_positions(
	float speed,
	const blink_EnvelopeData* env_speed,
	const blink_WarpPoints* warp_points,
	const Traverser& traverser,
	int sample_offset,
	int count,
	snd::transport::DSPVectorFramePosition* out_sculpted_positions,
	snd::transport::DSPVectorFramePosition* out_warped_positions,
	ml::DSPVector* out_derivatives)
{
	snd::transport::DSPVectorFramePosition sculpted_positions;
	snd::transport::DSPVectorFramePosition warped_positions;
	ml::DSPVector sculpted_derivatives;
	ml::DSPVector warped_derivatives;

	get_sculpted_positions(speed, env_speed, traverser, count, &sculpted_positions, out_derivatives ? &sculpted_derivatives : nullptr);

	sculpted_positions -= sample_offset;

	sculpted_block_positions_(sculpted_positions, 0, count);

	warp_traverser_.generate(sculpted_block_positions_, count);

	get_warp_positions(warp_points, warp_traverser_, count, &warped_positions, out_derivatives ? &warped_derivatives : nullptr);

	if (out_sculpted_positions) *out_sculpted_positions = sculpted_positions;
	if (out_warped_positions) *out_warped_positions = warped_positions;
	if (out_derivatives) *out_derivatives = sculpted_derivatives * warped_derivatives;
}

}}