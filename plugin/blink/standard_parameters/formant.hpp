#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std.hpp>
#include "percentage.hpp"
#include <blink/parameters/envelope_parameter_spec.hpp>

namespace blink {
namespace std_params {
namespace formant {

inline EnvelopeSpec envelope()
{
	EnvelopeSpec out;

	out.default_value = 0.5f;
	out.searcher.binary = search::float_points_binary;
	out.searcher.forward = search::float_points_forward;
	out.stepify = tweak::std::percentage_bipolar::stepify;

	out.value_slider = percentage::bipolar::slider();

	out.range.min.default_value = 0.0f;
	out.range.min.to_string = tweak::std::percentage_bipolar::to_string;
	out.range.max.default_value = 1.0f;
	out.range.max.to_string = tweak::std::percentage_bipolar::to_string;
	out.to_string = tweak::std::percentage_bipolar::to_string;

	return out;
}

inline EnvelopeParameterSpec envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = BLINK_STD_UUID_FORMANT;
	out.name = "Formant";

	out.envelope = envelope();
	out.flags = blink_EnvelopeFlags_NoGridLabels;

	return out;
}

} // formant
} // blink
} // std_params