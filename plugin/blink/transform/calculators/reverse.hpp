#pragma once

#include <functional>
#include <snd/misc.hpp>
#include "pitch.hpp"
#include "warp.hpp"

namespace blink {
namespace transform {
namespace calculators {

class ReverseUnit
{
public:

	struct Config
	{
		const blink_OptionData* reversal_data;
		std::function<blink_IntPosition(blink_IntPosition)> transform_position;
	};

	blink_Position operator()(Config config, blink_Position block_position)
	{
		static constexpr auto MIRROR { 0 };
		static constexpr auto TAPE { 1 };
		static constexpr auto SLIP { 2 };
		
		config_ = std::move(config);

		for (blink_Index i = point_search_index_; i < config.reversal_data->points.count; i++)
		{
			const auto p1 { get_cache_point(1, i) };

			if (block_position < p1.x)
			{
				if (i == 0) return block_position;

				const auto p0 { get_cache_point(0, i - 1) };
				const auto length { p1.x - p0.x };

				if (length > 0)
				{
					// If we got here then we are somewhere between two reverse
					// points
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
			else
			{
				if (i != 0)
				{
					// If we got here then we are somewhere in between two
					// reverse points and we are about to go to the next point

					// The starting frame of the next segment needs to be
					// calculated

					const auto p0 { get_cache_point(0, i - 1) };
					const auto distance { p1.x - p0.x };

					switch (p0.y)
					{
						case TAPE:
						{
							segment_start_frame_ -= distance;
							break;
						}

						default:
						{
							segment_start_frame_ += distance;
							break;
						}
					}
				}
				else
				{
					// If we got here then we are about to pass the first reverse
					// point, in which case the segment start is the x position of
					// the point
					segment_start_frame_ = p1.x;
				}

				point_search_index_++;
			}
		}

		const auto p0 { get_cache_point(0, config.reversal_data->points.count-1) };
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
		if (cache_.dirt[cache_index] != point_index)
		{
			cache_.points[cache_index] = config_.reversal_data->points.data[point_index];
			cache_.points[cache_index].x = config_.transform_position(cache_.points[cache_index].x);
			cache_.dirt[cache_index] = point_index;

			return cache_.points[cache_index];
		}

		return cache_.points[cache_index];
	}

	Config config_ {};
	blink_Position segment_start_frame_ { 0.0f };
	int point_search_index_ { 0 };

	mutable struct
	{
		std::array<blink_IntPoint, 2> points;
		std::array<int, 2> dirt { -1, -1 };
	} cache_;
};

class Reverse
{
public:

	struct Config
	{
		uint64_t unit_state_id;
		std::function<blink_IntPosition(blink_IntPosition)> transform_position;

		struct
		{
			const blink_OptionData* reverse;
		} option;

		struct
		{
			BlockPositions* positions;
		} outputs;
	};

	void operator()(Config config, const BlockPositions& block_positions, int count)
	{
		if (!config.option.reverse || config.option.reverse->points.count < 1)
		{
			*config.outputs.positions = block_positions;
			return;
		}

		ReverseUnit::Config unit_config;

		unit_config.reversal_data = config.option.reverse;
		unit_config.transform_position = config.transform_position;

		traverser_.generate(config.unit_state_id, block_positions, count);

		const auto& resets { traverser_.get_resets() };

		for (int i = 0; i < count; i++)
		{
			if (resets[i] > 0)
			{
				unit_calculator_.reset();
			}

			const auto position { unit_calculator_(unit_config, block_positions.positions[i]) };

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