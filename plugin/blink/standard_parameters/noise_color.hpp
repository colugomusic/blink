#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std/percentage.hpp>
#include <blink/math.hpp>
#include "percentage.hpp"

namespace blink {
namespace std_params {
namespace noise_color {

inline EnvelopeParameterSpec envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = BLINK_STD_UUID_NOISE_COLOR;
	out.name = "Noise Color";
	out.short_name = "Color";
	out.envelope = percentage::bipolar::envelope();
	out.envelope.show_grid_labels = false;

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

} // noise_color
} // blink
} // std_params