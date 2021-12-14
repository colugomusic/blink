#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std.hpp>
#include <blink/math.hpp>
#include "percentage.hpp"

namespace blink {
namespace std_params {
namespace mix {

inline EnvelopeSpec envelope()
{
	EnvelopeSpec out;

	out.default_value = 1.0f;
	out.search_binary = search::envelope_binary;
	out.search_forward = search::envelope_forward;
	out.stepify = tweak::std::percentage::stepify;
	out.value_slider = percentage::slider();
	out.range.min.default_value = 0.0f;
	out.range.min.to_string = tweak::std::percentage::to_string;
	out.range.max.default_value = 1.0f;
	out.range.max.to_string = tweak::std::percentage::to_string;
	out.to_string = tweak::std::percentage::to_string;

	return out;
}

inline EnvelopeParameterSpec envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_MIX;
	out.name = "Mix";

	out.envelope = envelope();

	return out;
}

} // mix
} // blink
} // std_params