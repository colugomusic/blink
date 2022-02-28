#pragma once

#include "blink.h"
#include "blink/traverser.hpp"
#include "calculators/pitch.hpp"
#include "calculators/reverse.hpp"
#include "calculators/warp.hpp"

namespace blink {
namespace transform {

class Tape
{
public:

	struct Config
	{
		uint64_t unit_state_id;
		float transpose;
		int64_t sample_offset;

		struct
		{
			const blink_EnvelopeData* pitch;
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
				bool pitch;
				bool warped;
			} derivatives;
		} outputs;
	};
	
	void operator()(Config config, const BlockPositions& block_positions, int count);

	auto& get_pitched_positions() const { return stage_.positions.pitched; }
	auto& get_warped_positions() const { return stage_.positions.warped; }
	auto& get_pitched_derivatives() const { return stage_.derivatives.pitched; }
	auto& get_warped_derivatives() const { return stage_.derivatives.warped; }

private:

	void apply_pitch(const Config& config, const BlockPositions& block_positions, int count);
	void apply_sample_offset(const Config& config);
	void apply_warp(const Config& config, int count);
	void apply_reverse(const Config& config, int count);

	struct
	{
		struct
		{
			BlockPositions pitched;
			BlockPositions warped;
			BlockPositions reversed;
		} positions;

		struct
		{
			ml::DSPVector pitched;
			ml::DSPVector warped;
		} derivatives;
	} stage_;

	struct
	{
		calculators::Pitch pitch;
		calculators::Warp warp;
		calculators::Reverse reverse;
	} calculators_;
};

inline void Tape::operator()(Config config, const BlockPositions& block_positions, int count)
{
	apply_pitch(config, block_positions, count);
	apply_sample_offset(config);
	apply_warp(config, count);
	apply_reverse(config, count);
}

inline void Tape::apply_pitch(const Config& config, const BlockPositions& block_positions, int count)
{
	calculators::Pitch::Config calculator_config;

	calculator_config.unit_state_id = config.unit_state_id;
	calculator_config.transpose = config.transpose;
	calculator_config.pitch = config.env.pitch;
	calculator_config.outputs.positions = &stage_.positions.pitched;
	calculator_config.outputs.derivatives = config.outputs.derivatives.pitch ? &stage_.derivatives.pitched : nullptr;

	calculators_.pitch(calculator_config, block_positions, count);
}

inline void Tape::apply_sample_offset(const Config& config)
{
	stage_.positions.pitched.positions -= config.sample_offset;
}

inline void Tape::apply_warp(const Config& config, int count)
{
	calculators::Warp::Config calculator_config;

	calculator_config.unit_state_id = config.unit_state_id;
	calculator_config.warp_points = config.warp_points;
	calculator_config.outputs.positions = &stage_.positions.warped;
	calculator_config.outputs.derivatives = config.outputs.derivatives.warped ? &stage_.derivatives.warped : nullptr;

	calculators_.warp(calculator_config, stage_.positions.pitched, count);
}

inline void Tape::apply_reverse(const Config& config, int count)
{
	calculators::Reverse::Config calculator_config;

	calculator_config.env.pitch = config.env.pitch;
	calculator_config.option.reverse = config.option.reverse;
	calculator_config.outputs.positions = &stage_.positions.reversed;
	calculator_config.sample_offset = config.sample_offset;
	calculator_config.transpose = config.transpose;
	calculator_config.unit_state_id = config.unit_state_id;
	calculator_config.warp_points = config.warp_points;

	calculators_.reverse(calculator_config, stage_.positions.warped, count);
}

} // transform
} // blink
