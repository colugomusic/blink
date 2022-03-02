#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std/percentage.hpp>
#include <blink/math.hpp>
#include <blink/parameters/envelope_parameter_spec.hpp>
#include "percentage.hpp"

namespace blink {
namespace std_params {
namespace resonance {

static constexpr auto UUID { BLINK_STD_UUID_FILTER_RESONANCE };

inline EnvelopeParameterSpec envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = UUID;
	out.name = "Resonance";
	out.envelope = percentage::envelope();

	out.flags |= blink_EnvelopeFlags_CanManipulate;
	out.flags |= blink_EnvelopeFlags_IsManipulatorTarget;

	return out;
}

inline EnvelopeManipulatorTargetSpec envelope_manipulator_target()
{
	EnvelopeManipulatorTargetSpec out;

	out.offset_envelope = percentage::bipolar::envelope();
	out.override_envelope = percentage::envelope();

	return out;
}

} // resonance
} // blink
} // std_params