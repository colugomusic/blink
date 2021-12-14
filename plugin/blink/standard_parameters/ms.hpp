#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std.hpp>
#include <blink/math.hpp>
#include "linear.hpp"

namespace blink {
namespace std_params {
namespace ms {

inline SliderSpec<float> slider(float min, float max, float default_value, float precision)
{
	SliderSpec<float> out = linear::slider(min, max, default_value, precision);

	out.stepify = [](float v) { return tweak::std::ms::stepify(v); };
	out.to_string = [](float v) { return tweak::std::ms::to_string(v); };
	out.from_string = [](const std::string& s) { return tweak::std::ms::from_string(s); };

	return out;
}

inline EnvelopeSpec envelope(float min, float max, float default_value)
{
    EnvelopeSpec out;

    out.default_value = default_value;
    out.search_binary = search::envelope_binary;
    out.search_forward = search::envelope_forward;
    out.stepify = tweak::std::ms::stepify;
    out.value_slider = slider(min, max, default_value, 1.0f);
    out.range.min.default_value = min;
    out.range.min.to_string = [](float v){ return tweak::std::ms::to_string(v); };
    out.range.max.default_value = max;
    out.range.max.to_string = [](float v) { return tweak::std::ms::to_string(v); };
    out.to_string = [](float v) { return tweak::std::ms::to_string(v); };

    return out;
}

} // ms
} // blink
} // std_params