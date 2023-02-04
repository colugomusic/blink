#pragma once

#include <tweak/std/speed.hpp>
#include <blink/slider_spec.hpp>
#include <blink/parameters/slider_parameter_spec.hpp>

namespace blink {
namespace std_params {
namespace speed {

static constexpr auto UUID { BLINK_STD_UUID_SPEED };

inline SliderSpec<float> slider()
{
	SliderSpec<float> out;

	out.searcher.binary = search::float_points_binary;
	out.searcher.forward = search::float_points_forward;
	out.constrain = tweak::std::speed::constrain;
	out.increment = tweak::std::speed::increment;
	out.decrement = tweak::std::speed::decrement;
	out.drag = tweak::std::speed::drag;
	out.to_string = tweak::std::speed::to_string;
	out.from_string = tweak::std::speed::from_string;
	out.default_value = 1.0f;

	return out;
}

inline SliderParameterSpec<float> slider_parameter()
{
	SliderParameterSpec<float> out;

	out.uuid = UUID;
	out.name = "Speed";

	out.slider = slider();
	out.flags |= blink_SliderFlags_MovesDisplay;
	out.flags |= blink_SliderFlags_CanManipulate;

	return out;
}

inline EnvelopeSpec envelope()
{
	EnvelopeSpec out;

	out.default_value = tweak::std::speed::NORMAL;
	out.searcher.binary = search::float_points_binary;
	out.searcher.forward = search::float_points_forward;
	out.to_string = tweak::std::speed::to_string;

	out.value_slider = slider();

	out.get_gridline = [](int index) -> float
	{
		return tweak::convert::linear_to_speed(float(index));
	};

	out.range.min.constrain = tweak::std::speed::constrain;
	out.range.min.increment = tweak::std::speed::increment;
	out.range.min.decrement = tweak::std::speed::decrement;
	out.range.min.to_string = tweak::std::speed::to_string;
	out.range.min.drag = tweak::std::speed::drag;
	out.range.min.from_string = tweak::std::speed::from_string;
	out.range.min.default_value = tweak::std::speed::FREEZE;

	out.range.max.constrain = tweak::std::speed::constrain;
	out.range.max.increment = tweak::std::speed::increment;
	out.range.max.decrement = tweak::std::speed::decrement;
	out.range.max.to_string = tweak::std::speed::to_string;
	out.range.max.drag = tweak::std::speed::drag;
	out.range.max.from_string = tweak::std::speed::from_string;
	out.range.max.default_value = 2.0f;

	return out;
}

inline EnvelopeParameterSpec envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = BLINK_STD_UUID_SPEED;
	out.name = "Speed";
	out.envelope = envelope();
	out.flags |= blink_EnvelopeFlags_MovesDisplay;
	out.flags |= blink_EnvelopeFlags_CanManipulate;

	out.offset_envelope = envelope();
	out.override_envelope = envelope();

	out.apply_offset = [](float value, float offset) -> float
	{
		return value * offset;
	};

	return out;
}

} // speed
} // blink
} // std_params