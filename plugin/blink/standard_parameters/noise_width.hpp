#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std/percentage.hpp>
#include <blink/math.hpp>
#include "percentage.hpp"

namespace blink {
namespace std_params {
namespace noise_width {

static constexpr auto UUID { BLINK_STD_UUID_NOISE_WIDTH };

inline SliderParameterSpec<float> slider_parameter()
{
	SliderParameterSpec<float> out;

	out.uuid = BLINK_STD_UUID_NOISE_WIDTH;
	out.name = "Noise Width";
	out.short_name = "Width";
	out.slider = percentage::slider();

	out.clamp_range = { 0.0f, 1.0f };
	out.flags |= blink_SliderFlags_HostClamp;
	out.flags |= blink_SliderFlags_CanManipulate;
	out.offset_envelope = percentage::bipolar::envelope();
	out.override_envelope = percentage::envelope();

	return out;
}

} // noise_width
} // blink
} // std_params