#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std.hpp>
#include <blink/math.hpp>
#include <blink/slider_spec.hpp>

namespace blink {
namespace std_params {
namespace percentage {

template <int MIN = 0, int MAX = 100>
inline SliderSpec<float> slider()
{
	SliderSpec<float> out;

	out.constrain = [](float v) { return std::clamp(v, float(MIN) / 100.0f, float(MAX) / 100.0f); };
	out.increment = tweak::std::percentage::increment;
	out.decrement = tweak::std::percentage::decrement;
	out.drag = tweak::std::percentage::drag;
	out.to_string = tweak::std::percentage::to_string;
	out.from_string = tweak::std::percentage::from_string;
	out.default_value = 0;

	return out;
}

template <int MIN = 0, int MAX = 100>
inline EnvelopeSpec envelope()
{
    EnvelopeSpec out;

    out.default_value = 0.0f;
    out.search_binary = search::envelope_binary;
    out.search_forward = search::envelope_forward;
    out.stepify = tweak::std::percentage::stepify;
    out.value_slider = percentage::slider<MIN, MAX>();
    out.range.min.default_value = float(MIN) / 100.0f;
    out.range.min.to_string = tweak::std::percentage::to_string;
    out.range.max.default_value = float(MAX) / 100.0f;
    out.range.max.to_string = tweak::std::percentage::to_string;
    out.to_string = tweak::std::percentage::to_string;

    return out;
}

namespace bipolar {

inline SliderSpec<float> slider()
{
	SliderSpec<float> out;

	out.constrain = tweak::std::percentage::constrain;
	out.increment = tweak::std::percentage_bipolar::increment;
	out.decrement = tweak::std::percentage_bipolar::decrement;
	out.drag = tweak::std::percentage_bipolar::drag;
	out.to_string = tweak::std::percentage_bipolar::to_string;
	out.from_string = tweak::std::percentage_bipolar::from_string;
	out.default_value = 0.5;

	return out;
}

inline EnvelopeSpec envelope()
{
    EnvelopeSpec out;

    out.default_value = 0.5f;
    out.search_binary = search::envelope_binary;
    out.search_forward = search::envelope_forward;
    out.stepify = tweak::std::percentage_bipolar::stepify;
    out.value_slider = slider();
    out.range.min.default_value = 0.0f;
    out.range.min.to_string = tweak::std::percentage_bipolar::to_string;
    out.range.max.default_value = 1.0f;
    out.range.max.to_string = tweak::std::percentage_bipolar::to_string;
    out.to_string = tweak::std::percentage_bipolar::to_string;

    return out;
}

} // bipolar

} // percentage
} // blink
} // std_params