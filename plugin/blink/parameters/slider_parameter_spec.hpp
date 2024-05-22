/* TODELETE:
#pragma once

#include <functional>
#include <blink/envelope_spec.hpp>
#include <blink/parameters/parameter_spec.hpp>
#include <blink/slider_spec.hpp>

namespace blink {

template <class T>
struct SliderParameterSpec : public ParameterSpec
{
	SliderSpec<T> slider;
	blink_StdIcon icon = blink_StdIcon_None;
	blink_Range clamp_range;
	std::optional<EnvelopeSpec> offset_envelope;
	std::optional<EnvelopeSpec> override_envelope;
	std::function<float(float, float)> apply_offset;
};

}
*/
