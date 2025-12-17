#pragma once

#include "blink.h"
#include "calculators/pitch.hpp"
#include "calculators/reverse.hpp"
#include "calculators/warp.hpp"
#include "correction_grains.hpp"
#include "blink/traverser.hpp"
#include <snd/frame-pos.hpp>

namespace blink {
namespace transform {

struct Tape {
	struct Config {
		uint64_t unit_state_id;
		float transpose {};
		int64_t sample_offset {};
		struct {
			const blink_UniformEnvData* pitch {};
		} env; 
		struct
		{
			const blink_UniformOptionData* reverse {};
		} option; 
		const blink_WarpPoints* warp_points {};
		struct {
			struct {
				bool pitch {};
				bool warped {};
			} derivatives; 
			bool correction_grains {};
		} outputs;
	}; 
	void xform(Config config, const BlockPositions& block_positions, int count); 
	auto& get_pitched_positions() const { return stage_.positions.pitched; }
	auto& get_warped_positions() const { return stage_.positions.warped; }
	auto& get_reversed_positions() const { return stage_.positions.reversed; }
	auto& get_pitched_derivatives() const { return stage_.derivatives.pitched; }
	auto& get_warped_derivatives() const { return stage_.derivatives.warped; }
	auto& get_correction_grains() const { return stage_.correction_grains; } 
private: 
	void apply_pitch(const Config& config, const BlockPositions& block_positions, int count);
	void apply_sample_offset(const Config& config);
	void apply_warp(const Config& config, int count);
	void apply_reverse(const Config& config, int count); 
	struct {
		struct {
			BlockPositions pitched;
			BlockPositions warped;
			BlockPositions reversed;
		} positions; 
		struct {
			ml::DSPVector pitched;
			ml::DSPVector warped;
		} derivatives; 
		CorrectionGrains correction_grains;
	} stage_; 
	struct {
		calculators::Pitch pitch;
		calculators::Warp warp;
		calculators::Reverse reverse;
	} calculators_;
};

inline void Tape::xform(Config config, const BlockPositions& block_positions, int count) {
	apply_pitch(config, block_positions, count);
	apply_sample_offset(config);
	apply_warp(config, count);
	apply_reverse(config, count);
}

inline void Tape::apply_pitch(const Config& config, const BlockPositions& block_positions, int count) {
	calculators::Pitch::Config calculator_config; 
	calculator_config.unit_state_id = config.unit_state_id;
	calculator_config.transpose = config.transpose;
	calculator_config.pitch = config.env.pitch;
	calculator_config.outputs.positions = &stage_.positions.pitched;
	calculator_config.outputs.derivatives = config.outputs.derivatives.pitch ? &stage_.derivatives.pitched : nullptr; 
	calculators_.pitch.xform(calculator_config, block_positions, count);
}

inline void Tape::apply_sample_offset(const Config& config) {
	snd::operator-=(stage_.positions.pitched.positions, config.sample_offset);
}

inline void Tape::apply_warp(const Config& config, int count) {
	calculators::Warp::Config calculator_config; 
	calculator_config.unit_state_id = config.unit_state_id;
	calculator_config.warp_points = config.warp_points;
	calculator_config.outputs.positions = &stage_.positions.warped;
	calculator_config.outputs.derivatives = config.outputs.derivatives.warped ? &stage_.derivatives.warped : nullptr; 
	calculators_.warp(calculator_config, stage_.positions.pitched, count);
}

inline void Tape::apply_reverse(const Config& config, int count) {
	// These sub-calculators are used to transform reverse
	// modulation points into "post-warp-space" by applying
	// the pitch and warp calculations to each point
	struct {
		calculators::PitchUnit pitch;
		calculators::PitchUnit::Config pitch_config {};
		calculators::WarpUnit warp; 
		struct {
			blink_Position pre_pitch { std::numeric_limits<std::int32_t>::max() };
			blink_Position pre_warp { std::numeric_limits<std::int32_t>::max() };
		} prev_positions;
	} sub_calculators;

	sub_calculators.pitch_config.pitch = config.env.pitch;
	sub_calculators.pitch_config.transpose = config.transpose;

	const auto transform_position { [&sub_calculators, &config](blink_Position p, float* derivative)
	{
		auto x { static_cast<blink_Position>(p) };

		if (config.env.pitch && config.env.pitch->points.count > 0)
		{
			if (x < sub_calculators.prev_positions.pre_pitch)
			{
				sub_calculators.pitch.reset();
			}

			sub_calculators.prev_positions.pre_pitch = x;

			x = sub_calculators.pitch.xform(sub_calculators.pitch_config, x, derivative);
		}
		else
		{
			*derivative = snd::convert::P2FF(config.transpose);

			x *= *derivative;
		}

		x -= config.sample_offset;

		if (config.warp_points && config.warp_points->count > 0)
		{
			if (x < sub_calculators.prev_positions.pre_warp)
			{
				sub_calculators.warp.reset();
			}

			sub_calculators.prev_positions.pre_warp = x;

			float ff;

			x = sub_calculators.warp(config.warp_points, x, &ff);

			*derivative *= ff;
		}

		p = static_cast<blink_Position>(x);

		return p;
	}};

	calculators::Reverse::Config calculator_config;

	calculator_config.option.reverse = config.option.reverse;
	calculator_config.outputs.positions = &stage_.positions.reversed;
	calculator_config.outputs.correction_grains = config.outputs.correction_grains ? &stage_.correction_grains : nullptr;
	calculator_config.unit_state_id = config.unit_state_id;
	calculator_config.transform_position = transform_position;

	calculators_.reverse(calculator_config, stage_.positions.warped, count);
}

} // transform
} // blink
