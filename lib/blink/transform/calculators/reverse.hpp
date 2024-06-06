#pragma once

#include <functional>
#include <snd/misc.hpp>
#include "pitch.hpp"
#include "warp.hpp"
#include "../correction_grains.hpp"

namespace blink {
namespace transform {
namespace calculators {

class ReverseUnit
{
public:

	struct Config
	{
		const blink_UniformOptionData* reversal_data;
		std::function<blink_Position(blink_Position, float*)> transform_position;
		CorrectionGrains* correction_grains {};
	};

	static constexpr auto MIRROR { 0 };
	static constexpr auto TAPE { 1 };
	static constexpr auto SLIP { 2 };
		
	blink_Position operator()(Config config, int buffer_index, blink_Position block_position)
	{
		config_ = std::move(config);
		const auto start_index { point_search_index_ };

		for (int i = point_search_index_; i < config.reversal_data->points.count; i++)
		{
			const auto p1 { get_cache_point(1, i) };

			if (block_position >= p1.x)
			{
				// We're past the right-hand point, so
				// skip to the next segment
				segment_start_frame_ = calculate_next_segment_start_frame(i);
				point_search_index_++;

				continue;
			}

			// If block position is before the first reverse
			// point, just return the block position
			if (i == 0)
			{
				return block_position;
			}

			if (point_search_index_ != start_index)
			{
				// generate a correction grain
				if (config.correction_grains)
				{
					generate_correction_grain(i - 1, buffer_index);
				}
			}

			const auto p0 { get_cache_point(0, i - 1) };
			const auto length { p1.x - p0.x };

			if (length > 0)
			{
				// If we got here then we are somewhere
				// between two reverse points
				const auto distance { block_position - p0.x };

				switch (p0.y)
				{
					case SLIP:
					case TAPE:
					{
						return segment_start_frame_ - distance;
					}

					case MIRROR:
					{
						return p1.x - distance;
					}

					default:
					{
						return segment_start_frame_ + distance;
					}
				}
			}
		}

		if (point_search_index_ != start_index)
		{
			// generate a correction grain
			if (config.correction_grains)
			{
				generate_correction_grain(point_search_index_ - 1, buffer_index);
			}
		}

		const auto p0 { get_cache_point(0, int(config_.reversal_data->points.count-1)) };
		const auto distance { block_position - p0.x };

		return segment_start_frame_ + distance;
	}

	void reset()
	{
		segment_start_frame_ = 0.0f;
		point_search_index_ = 0;
		cache_.dirt = { -1, -1 };
	}

private:

	// Two points are kept in cache because the transformation might be expensive (?)
	//
	// If point_index is not equal to whatever it was the last time this point was
	// accessed, the point is considered dirty and will update
	//
	blink_IntPoint get_cache_point(int cache_index, int point_index) const
	{
		update_cache_point(cache_index, point_index);

		return cache_.points[cache_index];
	}

	float get_cache_point_ff(int cache_index, int point_index) const
	{
		update_cache_point(cache_index, point_index);

		return cache_.derivatives[cache_index];
	}

	void update_cache_point(int cache_index, int point_index) const
	{
		if (cache_.dirt[cache_index] != point_index)
		{
			cache_.points[cache_index] = config_.reversal_data->points.data[point_index];
			cache_.points[cache_index].x = config_.transform_position(cache_.points[cache_index].x, &cache_.derivatives[cache_index]);
			cache_.dirt[cache_index] = point_index;
		}
	}

	blink_Position calculate_next_segment_start_frame(int current_search_index)
	{
		const auto p1 { get_cache_point(1, current_search_index) };

		if (current_search_index == 0)
		{
			// If we got here then we are about to pass the first reverse
			// point, in which case the segment start is the x position of
			// the point
			return blink_Position(p1.x);
		}

		// If we got here then we are somewhere in between two
		// reverse points and we are about to go to the next point

		// The starting frame of the next segment needs to be
		// calculated

		const auto p0 { get_cache_point(0, current_search_index - 1) };
		const auto distance { p1.x - p0.x };

		switch (p0.y)
		{
			case TAPE:
			{
				return segment_start_frame_ - distance;
			}

			default:
			{
				return segment_start_frame_ + distance;
			}
		}
	}

	void generate_correction_grain(int current_search_index, int buffer_index)
	{
		if (current_search_index < 1) return;

		auto ff { get_cache_point_ff(1, current_search_index) };
		const auto p0 { get_cache_point(0, current_search_index - 1) };

		if (p0.y >= 0) ff *= -1;

		float length { kFloatsPerDSPVector * 64 };

		if (current_search_index + 1 < config_.reversal_data->points.count)
		{
			const auto p1 { config_.reversal_data->points.data[current_search_index].x };
			const auto p2 { config_.reversal_data->points.data[current_search_index + 1].x };
			const auto next_segment_length { static_cast<float>(p2 - p1) };

			length = std::min(length, next_segment_length * 0.75f);
		}

		config_.correction_grains->push({ buffer_index, ff, length });
	}

	Config config_ {};
	blink_Position segment_start_frame_ { 0.0f };
	int point_search_index_ { 0 };

	mutable struct
	{
		std::array<blink_IntPoint, 2> points;
		std::array<float, 2> derivatives;
		std::array<int, 2> dirt { -1, -1 };
	} cache_;
};

class Reverse
{
public:

	struct Config
	{
		uint64_t unit_state_id;
		std::function<blink_Position(blink_Position, float*)> transform_position;

		struct
		{
			const blink_UniformOptionData* reverse;
		} option;

		struct
		{
			BlockPositions* positions;
			CorrectionGrains* correction_grains {};
		} outputs;
	};

	void operator()(Config config, const BlockPositions& block_positions, int count)
	{
		if (config.outputs.correction_grains)
		{
			config.outputs.correction_grains->count = 0;
		}

		if (!config.option.reverse || config.option.reverse->points.count < 2)
		{
			*config.outputs.positions = block_positions;
			return;
		}

		ReverseUnit::Config unit_config;

		unit_config.reversal_data = config.option.reverse;
		unit_config.transform_position = config.transform_position;
		unit_config.correction_grains = config.outputs.correction_grains;

		traverser_.generate(config.unit_state_id, block_positions, count);

		const auto& resets { traverser_.get_resets() };

		config.outputs.positions->rotate_prev_pos();

		for (int i = 0; i < count; i++)
		{
			if (resets[i] > 0)
			{
				unit_calculator_.reset();
			}

			const auto position { unit_calculator_(unit_config, i, block_positions.positions[i]) };

			config.outputs.positions->positions.set(i, position);
		}
	}

private:

	ReverseUnit unit_calculator_;
	Traverser traverser_;
};

} // calculators
} // transform
} // blink