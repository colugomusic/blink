#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std.hpp>
#include <blink/math.hpp>
#include "percentage.hpp"

namespace blink {
namespace std_params {
namespace noise_width {

inline SliderParameterSpec<float> slider_parameter()
{
	SliderParameterSpec<float> out;

	out.uuid = BLINK_STD_UUID_SLIDER_NOISE_WIDTH;
	out.name = "Noise Width";

	out.slider = percentage::slider();

	return out;
}

} // noise_width
} // blink
} // std_params