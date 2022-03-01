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

		const auto get_point { [&config](blink_Index i)
		{
			return config.reversal_data->points.data[i];
		}};

		const auto get_xformed_point { [&config](blink_Index i)
		{
			auto p { config.reversal_data->points.data[i] };

			p.x = config.transform_position(p.x);

			return p;
		}};

		const auto get_direction { [&](blink_Index i)
		{
			if (i == 0) return 1;

			const auto p0 { get_xformed_point(i-1) };

			return p0.y >= 0 ? -1 : 1;
		}};

		for (blink_Index i = point_search_index_; i < config.reversal_data->points.count; i++)
		{
			const auto direction { get_direction(i) };
			const auto p1 { get_xformed_point(i) };

			if (block_position < p1.x)
			{
				if (i == 0) return block_position;

				const auto p0 { get_xformed_point(i-1) };
				const auto length { p1.x - p0.x };

				if (length > 0)
				{
					const auto x { block_position };
					const auto distance { x - p0.x };

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
					const auto p0 { get_xformed_point(i-1) };
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
					segment_start_frame_ = p1.x;
				}

				point_search_index_++;
			}
		}

		const auto p0 { get_xformed_point(config.reversal_data->points.count-1) };
		const auto x { block_position };
		const auto distance { x - p0.x };
		const auto direction { get_direction(config.reversal_data->points.count) };

		return segment_start_frame_ + (distance * direction);
	}

	void reset()
	{
		segment_start_frame_ = 0.0f;
		point_search_index_ = 0;
	}

private:

	blink_Position segment_start_frame_ { 0.0f };
	int point_search_index_ { 0 };
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