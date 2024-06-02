#pragma once

#include "blink.h"
#include "blink/traverser.hpp"
#include "calculators/speed.hpp"
#include "calculators/reverse.hpp"
#include "calculators/warp.hpp"

namespace blink {
namespace transform {

class Stretch
{
public:

	struct Config
	{
		uint64_t unit_state_id;
		float speed;
		int64_t sample_offset;

		struct
		{
			const blink_EnvData* speed;
		} env;

		struct
		{
			const blink_OptionData* reverse;
		} option;

		const blink_WarpPoints* warp_points;

		struct
		{
			struct
			{
				bool sped;
				bool warped;
			} derivatives;
		} outputs;
	};
	
	void operator()(Config config, const BlockPositions& block_positions, int count);

	auto& get_sped_positions() const { return stage_.positions.sped; }
	auto& get_warped_positions() const { return stage_.positions.warped; }
	auto& get_reversed_positions() const { return stage_.positions.reversed; }
	auto& get_sped_derivatives() const { return stage_.derivatives.sped; }
	auto& get_warped_derivatives() const { return stage_.derivatives.warped; }

private:

	void apply_speed(const Config& config, const BlockPositions& block_positions, int count);
	void apply_sample_offset(const Config& config);
	void apply_warp(const Config& config, int count);
	void apply_reverse(const Config& config, int count);

	struct
	{
		struct
		{
			BlockPositions sped;
			BlockPositions warped;
			BlockPositions reversed;
		} positions;

		struct
		{
			ml::DSPVector sped;
			ml::DSPVector warped;
		} derivatives;
	} stage_;

	struct
	{
		calculators::Speed speed;
		calculators::Warp warp;
		calculators::Reverse reverse;
	} calculators_;
};

inline void Stretch::operator()(Config config, const BlockPositions& block_positions, int count)
{
	apply_speed(config, block_positions, count);
	apply_sample_offset(config);
	apply_warp(config, count);
	apply_reverse(config, count);
}

inline void Stretch::apply_speed(const Config& config, const BlockPositions& block_positions, int count)
{
	calculators::Speed::Config calculator_config;

	calculator_config.unit_state_id = config.unit_state_id;
	calculator_config.speed = config.speed;
	calculator_config.env_speed = config.env.speed;
	calculator_config.outputs.positions = &stage_.positions.sped;
	calculator_config.outputs.derivatives = config.outputs.derivatives.sped ? &stage_.derivatives.sped : nullptr;

	calculators_.speed(calculator_config, block_positions, count);
}

inline void Stretch::apply_sample_offset(const Config& config)
{
	stage_.positions.sped.positions -= config.sample_offset;
}

inline void Stretch::apply_warp(const Config& config, int count)
{
	calculators::Warp::Config calculator_config;

	calculator_config.unit_state_id = config.unit_state_id;
	calculator_config.warp_points = config.warp_points;
	calculator_config.outputs.positions = &stage_.positions.warped;
	calculator_config.outputs.derivatives = config.outputs.derivatives.warped ? &stage_.derivatives.warped : nullptr;

	calculators_.warp(calculator_config, stage_.positions.sped, count);
}

inline void Stretch::apply_reverse(const Config& config, int count)
{
	// These sub-calculators are used to transform reverse
	// modulation points into "post-warp-space" by applying
	// the speed and warp calculations to each point
	struct
	{
		calculators::SpeedUnit speed;
		calculators::SpeedUnit::Config speed_config {};
		calculators::WarpUnit warp;

		struct
		{
			blink_Position pre_speed { std::numeric_limits<std::int32_t>::max() };
			blink_Position pre_warp { std::numeric_limits<std::int32_t>::max() };
		} prev_positions;
	} sub_calculators;

	sub_calculators.speed_config.env_speed = config.env.speed;
	sub_calculators.speed_config.speed = config.speed;

	const auto transform_position { [&sub_calculators, &config](blink_Position p, float* derivative)
	{
		auto x { static_cast<blink_Position>(p) };

		if (config.env.speed && config.env.speed->points.count > 0)
		{
			if (x < sub_calculators.prev_positions.pre_speed)
			{
				sub_calculators.speed.reset();
			}

			sub_calculators.prev_positions.pre_speed = x;

			x = sub_calculators.speed(sub_calculators.speed_config, x);
		}
		else
		{
			x *= config.speed;
		}

		x -= config.sample_offset;

		if (config.warp_points && config.warp_points->count > 0)
		{
			if (x < sub_calculators.prev_positions.pre_warp)
			{
				sub_calculators.warp.reset();
			}

			sub_calculators.prev_positions.pre_warp = x;

			x = sub_calculators.warp(config.warp_points, x);
		}

		p = static_cast<blink_Position>(x);

		return p;
	}};

	calculators::Reverse::Config calculator_config;

	calculator_config.option.reverse = config.option.reverse;
	calculator_config.outputs.positions = &stage_.positions.reversed;
	calculator_config.unit_state_id = config.unit_state_id;
	calculator_config.transform_position = transform_position;

	calculators_.reverse(calculator_config, stage_.positions.warped, count);
}

} // transform
} // blink
