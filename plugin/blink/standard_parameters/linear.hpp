#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std.hpp>
#include <blink/math.hpp>
#include <blink/envelope_spec.hpp>
#include <blink/slider_spec.hpp>
#include "search.hpp"

namespace blink {
namespace std_params {
namespace linear {

inline SliderSpec<float> slider(float min, float max, float default_value, float precision)
{
	SliderSpec<float> out;

	out.constrain = [min, max](float v) { return std::clamp(v, min, max); };
	out.increment = [min, max, precision](float v, bool precise) { return v + (precise ? precision * 0.1f : precision); };
	out.decrement = [min, max, precision](float v, bool precise) { return v - (precise ? precision * 0.1f : precision); };
	out.drag = [min, max, precision](float v, int amount, bool precise) { return v + (amount * (precise ? precision * 0.1f : precision)); };
	out.to_string = [](float v) { return tweak::to_string(v); };
	out.from_string = [](const std::string& s) { return tweak::find_number<float>(s); };
	out.default_value = default_value;

	return out;
}

inline EnvelopeSpec envelope(float min, float max, float default_value)
{
    EnvelopeSpec out;

    out.default_value = default_value;
    out.search_binary = search::envelope_binary;
    out.search_forward = search::envelope_forward;
    out.value_slider = slider(min, max, default_value, 1.0f);
    out.range.min.default_value = min;
    out.range.min.to_string = [](float v){ return tweak::to_string(v); };
    out.range.max.default_value = max;
    out.range.max.to_string = [](float v) { return tweak::to_string(v); };
    out.to_string = [](float v) { return tweak::to_string(v); };

    return out;
}

} // linear
} // blink
} // std_params