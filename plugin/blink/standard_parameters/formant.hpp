#pragma once

#include <tweak/std/percentage.hpp>
#include "percentage.hpp"
#include <blink/parameters/envelope_parameter_spec.hpp>

namespace blink {
namespace std_params {
namespace formant {

inline EnvelopeSpec envelope()
{
	EnvelopeSpec out;

	out.searcher.binary = search::float_points_binary;
	out.searcher.forward = search::float_points_forward;
	out.default_value = 0.0f;
	out.searcher.binary = search::float_points_binary;
	out.searcher.forward = search::float_points_forward;
	out.stepify = tweak::std::percentage::stepify;

	out.value_slider = percentage::bipolar::slider();

	out.range.min.default_value = -1.0f;
	out.range.min.to_string = tweak::std::percentage::to_string;
	out.range.max.default_value = 1.0f;
	out.range.max.to_string = tweak::std::percentage::to_string;
	out.to_string = tweak::std::percentage::to_string;
	out.show_grid_labels = false;

	return out;
}

inline EnvelopeParameterSpec envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = BLINK_STD_UUID_FORMANT;
	out.name = "Formant";

	out.envelope = envelope();

	out.flags |= blink_ParamFlags_CanManipulate;
	out.offset_envelope = envelope();
	out.override_envelope = envelope();

	return out;
}

} // formant
} // blink
} // std_params