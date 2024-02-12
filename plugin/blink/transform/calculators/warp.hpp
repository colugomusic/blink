#pragma once 

#include "blink_sampler.h"
#include "blink/traverser.hpp"

namespace blink {
namespace transform {
namespace calculators {

class WarpUnit
{
public:

	blink_Position operator()(const blink_WarpPoints* warp_points, blink_Position position, float* derivative = nullptr)
	{
		if (warp_points->count == 1)
		{
			if (derivative) *derivative = 1.0f;

			return position - blink_Position(warp_points->points[0].y - warp_points->points[0].x);
		}

		for (blink_Index i = point_search_index_; i < warp_points->count; i++)
		{
			point_search_index_ = i;

			auto p1 { warp_points->points[i] };

			if (position < p1.y)
			{
				if (i == 0)
				{
					auto x { position / p1.y };

					if (derivative) *derivative = 1.0f;

					return p1.x + (position - p1.y);
				}

				auto p0 { warp_points->points[i - 1] };
				auto segment_size { p1.y - p0.y };

				if (segment_size > 0.f)
				{
					auto x { (position - p0.y) / segment_size };

					if (derivative)
					{
						const auto x_diff { (p1.x - p0.x) };
						const auto y_diff { (p1.y - p0.y) };

						if (y_diff == 0.0f)
						{
							*derivative = 0.0f;
						}
						else
						{
							*derivative = static_cast<float>(x_diff / y_diff);
						}
					}

					return math::lerp(float(p0.x), float(p1.x), float(x));
				}
			}
		}

		auto p0 { warp_points->points[warp_points->count - 1] };

		point_search_index_ = warp_points->count;

		if (derivative) *derivative = 1.0f;

		return p0.x + (position - p0.y);
	}

	void reset()
	{
		point_search_index_ = 0;
	}

private:

	int point_search_index_ { 0 };
};

class Warp
{
public:

	struct Config
	{
		uint64_t unit_state_id;
		const blink_WarpPoints* warp_points;

		struct
		{
			BlockPositions* positions;
			ml::DSPVector* derivatives;
		} outputs;
	};

	void operator()(Config config, const BlockPositions& block_positions, int count)
	{
		if (!config.warp_points || config.warp_points->count < 1)
		{
			*config.outputs.positions = block_positions;

			if (config.outputs.derivatives) *config.outputs.derivatives = 1.0f;

			return;
		}

		traverser_.generate(config.unit_state_id, block_positions, count);

		const auto& resets { traverser_.get_resets() };

		config.outputs.positions->rotate_prev_pos();

		for (int i = 0; i < count; i++)
		{
			if (resets[i] > 0)
			{
				unit_calculator_.reset();
			}

			const auto out_derivative { config.outputs.derivatives ? &config.outputs.derivatives->getBuffer()[i] : nullptr };
			const auto position { unit_calculator_(config.warp_points, block_positions.positions[i], out_derivative) };

			config.outputs.positions->positions.set(i, position);
		}
	}

private:

	WarpUnit unit_calculator_;
	Traverser traverser_;
};

} // calculators
} // transform
} // blink
