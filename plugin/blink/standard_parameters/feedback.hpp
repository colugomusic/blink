#pragma once

#include <tweak/std/percentage.hpp>
#include "percentage.hpp"
#include <blink/parameters/envelope_parameter_spec.hpp>
#include <blink/parameters/envelope_manipulator_target_spec.hpp>
#include <blink/parameters/slider_parameter_spec.hpp>

namespace blink {
namespace std_params {
namespace feedback {

static constexpr auto UUID { BLINK_STD_UUID_FEEDBACK };

inline auto envelope()
{
	auto out { percentage::envelope() };

	out.default_value = 0.5f;

	return out;
}

inline auto envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = UUID;
	out.name = "Feedback";
	out.envelope = envelope();
	out.clamp_range = { 0.0f, 1.0f };

	out.flags |= blink_EnvelopeFlags_HostClamp;
	out.flags |= blink_EnvelopeFlags_CanManipulate;
	out.flags |= blink_EnvelopeFlags_IsManipulatorTarget;

	return out;
}

inline auto slider_parameter()
{
	SliderParameterSpec<float> out;

	out.uuid = UUID;
	out.clamp_range = { 0.0f, 1.0f };
	out.name = "Feedback";
	out.slider = percentage::slider();

	out.flags |= blink_SliderFlags_HostClamp;
	out.flags |= blink_SliderFlags_CanManipulate;

	return out;
}

inline EnvelopeManipulatorTargetSpec envelope_manipulator_target()
{
	EnvelopeManipulatorTargetSpec out;

	out.offset_envelope = percentage::bipolar::envelope();
	out.override_envelope = envelope();

	return out;
}

} // feedback
} // std_params
} // blink