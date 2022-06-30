#pragma once

#include <tweak/tweak.hpp>
#include <blink/math.hpp>
#include "percentage.hpp"

namespace blink {
namespace std_params {
namespace mix {

static constexpr auto UUID { BLINK_STD_UUID_MIX };

inline auto envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = UUID;
	out.name = "Mix";

	out.envelope = percentage::envelope();
	out.clamp_range = { 0.0f, 1.0f };

	out.flags |= blink_EnvelopeFlags_HostClamp;;
	out.flags |= blink_EnvelopeFlags_CanManipulate;
	out.flags |= blink_EnvelopeFlags_IsManipulatorTarget;

	return out;
}

inline auto slider_parameter()
{
	SliderParameterSpec<float> out;

	out.uuid = UUID;
	out.name = "Mix";

	out.slider = percentage::slider();

	out.flags |= blink_SliderFlags_CanManipulate;

	return out;
}

inline auto envelope_manipulator_target()
{
	EnvelopeManipulatorTargetSpec out;

	out.offset_envelope = percentage::bipolar::envelope();
	out.override_envelope = percentage::envelope();

	return out;
}

} // mix
} // blink
} // std_params