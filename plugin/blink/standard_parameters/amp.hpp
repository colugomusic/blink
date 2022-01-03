#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std.hpp>
#include <blink/math.hpp>
#include <blink/parameters/envelope_parameter_spec.hpp>
#include <blink/parameters/envelope_manipulator_target_spec.hpp>
#include <blink/parameters/slider_parameter_spec.hpp>
#include "percentage.hpp"
#include "search.hpp"

namespace blink {
namespace std_params {
namespace amp {

inline auto stepify(float v) { return tweak::std::amp::stepify(v); }
inline auto display(float v) { return tweak::std::amp::to_string(v); }
inline auto constrain(float v) { return tweak::std::amp::constrain(v); }
inline auto from_string(const std::string& str) { return tweak::std::amp::from_string(str); }
inline auto increment(float v, bool precise) { return tweak::std::amp::increment(v, precise); }
inline auto decrement(float v, bool precise) { return tweak::std::amp::decrement(v, precise); }
inline auto drag(float v, int amount, bool precise) { return tweak::std::amp::drag(v, amount, precise); }

inline SliderSpec<float> slider()
{
	SliderSpec<float> out;

	out.searcher.binary = search::float_points_binary;
	out.searcher.forward = search::float_points_forward;
	out.constrain = amp::constrain;
	out.increment = amp::increment;
	out.decrement = amp::decrement;
	out.drag = amp::drag;
	out.from_string = amp::from_string;
	out.to_string = amp::display;
	out.stepify = amp::stepify;
	out.default_value = 1.0f;

	return out;
}

inline EnvelopeSpec envelope()
{
	EnvelopeSpec out;

	out.get_gridline = [](int index) -> float
	{
		return math::convert::linear_to_speed(float(index));
	};

	out.default_value = 1.0f;
	out.searcher.binary = search::float_points_binary;
	out.searcher.forward = search::float_points_forward;
	out.to_string = amp::display;
	out.stepify = amp::stepify;

	out.value_slider = slider();

	out.range.min.default_value = 0.0f;
	out.range.min.to_string = amp::display;

	out.range.max.default_value = 1.0f;
	out.range.max.to_string = amp::display;
	out.range.max.from_string = amp::from_string;
	out.range.max.constrain = amp::constrain;
	out.range.max.increment = amp::increment;
	out.range.max.decrement = amp::decrement;
	out.range.max.drag = amp::drag;

	return out;
}

inline SliderParameterSpec<float> slider_parameter()
{
	SliderParameterSpec<float> out;

	out.uuid = BLINK_STD_UUID_AMP;
	out.name = "Amp";

	out.slider = slider();
	out.icon = blink_StdIcon_Amp;
	out.flags = blink_SliderFlags_MovesDisplay;

	return out;
}

inline EnvelopeParameterSpec envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = BLINK_STD_UUID_AMP;
	out.name = "Amp";

    out.envelope = envelope();
	out.flags = blink_EnvelopeFlags_MovesDisplay;

	return out;
}

inline EnvelopeManipulatorTargetSpec envelope_manipulator_target()
{
	EnvelopeManipulatorTargetSpec out;

	out.offset_envelope = envelope();
	out.override_envelope = envelope();

	return out;
}

} // amp
} // blink
} // std_params