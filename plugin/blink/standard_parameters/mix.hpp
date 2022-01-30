#pragma once

#include <tweak/tweak.hpp>
#include <blink/math.hpp>
#include "percentage.hpp"

namespace blink {
namespace std_params {
namespace mix {

static constexpr auto UUID { BLINK_STD_UUID_MIX };

inline EnvelopeSpec envelope()
{
	EnvelopeSpec out;

	out.default_value = 1.0f;
	out.searcher.binary = search::float_points_binary;
	out.searcher.forward = search::float_points_forward;
	out.stepify = tweak::std::percentage::stepify;
	out.value_slider = percentage::slider();
	out.range.min.default_value = 0.0f;
	out.range.min.to_string = tweak::std::percentage::to_string;
	out.range.max.default_value = 1.0f;
	out.range.max.to_string = tweak::std::percentage::to_string;
	out.to_string = tweak::std::percentage::to_string;

	return out;
}

inline EnvelopeParameterSpec envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = UUID;
	out.name = "Mix";

	out.envelope = envelope();
	out.clamp_range = { 0.0f, 1.0f };

	out.flags |= blink_EnvelopeFlags_HostClamp;;
	out.flags |= blink_EnvelopeFlags_CanManipulate;
	out.flags |= blink_EnvelopeFlags_IsManipulatorTarget;

	return out;
}

inline EnvelopeManipulatorTargetSpec envelope_manipulator_target()
{
	EnvelopeManipulatorTargetSpec out;

	out.offset_envelope = percentage::bipolar::envelope();
	out.override_envelope = envelope();

	return out;
}

} // mix
} // blink
} // std_params