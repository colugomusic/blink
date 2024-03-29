#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std/percentage.hpp>
#include <blink/math.hpp>
#include "percentage.hpp"

namespace blink {
namespace std_params {
namespace noise_color {

static constexpr auto UUID { BLINK_STD_UUID_NOISE_COLOR };

inline EnvelopeParameterSpec envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = BLINK_STD_UUID_NOISE_COLOR;
	out.name = "Noise Color";
	out.short_name = "Color";
	out.envelope = percentage::bipolar::envelope();
	out.envelope.show_grid_labels = false;
	out.clamp_range = { -1.0f, 1.0f };

	out.flags |= blink_EnvelopeFlags_HostClamp;
	out.flags |= blink_EnvelopeFlags_CanManipulate;
	out.offset_envelope = percentage::bipolar::envelope();
	out.override_envelope = percentage::bipolar::envelope();

	return out;
}

} // noise_color
} // blink
} // std_params