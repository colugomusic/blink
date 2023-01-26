#pragma once

#include <tweak/tweak.hpp>
#include <blink/math.hpp>
#include <blink/slider_spec.hpp>
#include <blink/parameters/slider_parameter_spec.hpp>
#include <blink/parameters/envelope_manipulator_target_spec.hpp>

namespace blink {
namespace std_params {
namespace pitch {

static constexpr auto UUID { BLINK_STD_UUID_PITCH };

inline auto stepify(float v) -> float
{
	return tweak::math::stepify<10>(v);
}

inline float constrain(float v)
{
	if (v < -60.0f) return -60.0f;
	if (v > 60.0f) return 60.0f;

	return v;
};

inline auto increment(float v, bool precise)
{
	return tweak::increment<1, 10>(v, precise);
};

inline auto decrement(float v, bool precise)
{
	return tweak::decrement<1, 10>(v, precise);
};

inline auto drag(float v, int amount, bool precise) -> float
{
	return tweak::drag<float, 1, 10>(v, amount / 5, precise);
};

inline SliderSpec<float> slider()
{
	SliderSpec<float> out;

	out.searcher.binary = search::float_points_binary;
	out.searcher.forward = search::float_points_forward;
	out.constrain = pitch::constrain;
	out.increment = pitch::increment;
	out.decrement = pitch::decrement;
	out.drag = pitch::drag;
	out.to_string = [](float v) { return tweak::to_string(v); };
	out.from_string = [](std::string str) { return tweak::find_number<float>(str); };
	out.stepify = pitch::stepify;
	out.default_value = 0.0f;

	return out;
}

inline SliderParameterSpec<float> slider_parameter()
{
	SliderParameterSpec<float> out;

	out.uuid = BLINK_STD_UUID_PITCH;
	out.name = "Pitch";

	out.slider = slider();
	out.icon = blink_StdIcon_Pitch;
	out.flags = blink_SliderFlags_MovesDisplay;

	return out;
}

inline EnvelopeSpec envelope()
{
	EnvelopeSpec out;

	out.default_value = 0.0f;
	out.searcher.binary = search::float_points_binary;
	out.searcher.forward = search::float_points_forward;
	out.stepify = pitch::stepify;
	out.snap_value = tweak::snap_value;

	out.value_slider = slider();

	out.to_string = [](float v) { return tweak::to_string(v); };

	out.get_gridline = [](int index) -> float
	{
		return float(index * 12);
	};

	out.get_stepline = [](int index, float step_size) -> float
	{
		return step_size * index;
	};

	out.range.min.constrain = pitch::constrain;
	out.range.min.decrement = pitch::decrement;
	out.range.min.increment = pitch::increment;
	out.range.min.default_value = -24.0f;
	out.range.min.to_string = [](float v) { return tweak::to_string(v); };
	out.range.min.drag = pitch::drag;
	out.range.min.from_string = [](std::string str) { return tweak::find_number<float>(str); };

	out.range.max.constrain = pitch::constrain;
	out.range.max.decrement = pitch::decrement;
	out.range.max.increment = pitch::increment;
	out.range.max.default_value = 24.0f;
	out.range.max.to_string = [](float v) { return tweak::to_string(v); };
	out.range.max.drag = pitch::drag;
	out.range.max.from_string = [](std::string str) { return tweak::find_number<float>(str); };

	out.step_size.constrain = [](float v) { return tweak::constrain(v, 0.0f, 60.0f); };
	out.step_size.decrement = [out](float v, bool precise) { return out.step_size.constrain(tweak::decrement<1, 10>(v, precise)); };
	out.step_size.increment = [out](float v, bool precise) { return out.step_size.constrain(tweak::increment<1, 10>(v, precise)); };
	out.step_size.default_value = 1.0f;
	out.step_size.to_string = [](float v) { return tweak::to_string(v); };
	out.step_size.drag = [out](float v, int amount, bool precise) { return out.step_size.constrain(pitch::drag(v, amount, precise)); };
	out.step_size.from_string = [](std::string str) { return tweak::find_number<float>(str); };

	out.default_snap_amount = 1.0f;

	return out;
}

inline EnvelopeParameterSpec envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = BLINK_STD_UUID_PITCH;
	out.name = "Pitch";
	out.envelope = envelope();
	out.flags |= blink_EnvelopeFlags_MovesDisplay;
	out.flags |= blink_EnvelopeFlags_CanManipulate;

	return out;
}

inline EnvelopeManipulatorTargetSpec envelope_manipulator_target()
{
	EnvelopeManipulatorTargetSpec out;

	out.offset_envelope = envelope();
	out.override_envelope = envelope();

	return out;
}

} // pitch
} // blink
} // std_params