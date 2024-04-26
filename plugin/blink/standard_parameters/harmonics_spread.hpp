#pragma once

#include <blink/standard_parameters/percentage.hpp>
#include <blink/parameters/envelope_parameter_spec.hpp>
#include <blink/parameters/envelope_manipulator_target_spec.hpp>

namespace blink {
namespace std_params {
namespace harmonics_spread {

static constexpr auto UUID { "f04c18fd-0341-4398-a02b-f3e253f658c1" };

auto stepify(float v) -> float
{
	return tweak::std::percentage::stepify(v);
}

auto snap_value(float v, float step_size, float snap_amount)
{
	return stepify(tweak::snap_value(v, step_size, snap_amount));
}

float constrain(float v)
{
	return std::clamp(v, 0.0f, 4.0f);
};

auto increment(float v, bool precise)
{
	return constrain(stepify(tweak::increment<100, 1000>(v, precise)));
};

auto decrement(float v, bool precise)
{
	return constrain(stepify(tweak::decrement<100, 1000>(v, precise)));
};

auto drag(float v, int amount, bool precise) -> float
{
	return constrain(stepify(tweak::drag<float, 100, 1000>(v, amount / 5, precise)));
}

auto inline envelope()
{
	auto out { ::blink::std_params::percentage::envelope() };

	out.default_value = 1.0f;
	out.get_gridline = [](int index) { return float(index) / 4.0f; };
    out.stepify = tweak::math::stepify<100, float>;

	out.value_slider.constrain = constrain;
	out.value_slider.stepify = stepify;
	out.value_slider.decrement = decrement;
	out.value_slider.increment = increment;
	out.value_slider.drag = drag;

	out.range.max = out.value_slider;
	out.range.max.default_value = 2.0f;

	return out;
}

auto inline envelope_parameter()
{
	blink::EnvelopeParameterSpec out;

	out.uuid = UUID;
	out.name = "Harmonics Spread";
	out.short_name = "Spread";

	out.envelope = envelope();

	out.clamp_range = { 0.0f, 10.0f };

	out.flags |= blink_ParamFlags_HostClamp;
	out.flags |= blink_ParamFlags_CanManipulate;
	out.offset_envelope = ::blink::std_params::percentage::bipolar::envelope();
	out.override_envelope = envelope();

	return out;
}

} // harmonics_spread
} // params
} // fudge
