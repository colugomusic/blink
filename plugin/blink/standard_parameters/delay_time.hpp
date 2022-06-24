#pragma once

#include <blink/parameters/envelope_parameter_spec.hpp>
#include <blink/parameters/envelope_manipulator_target_spec.hpp>
#include <blink/parameters/slider_parameter_spec.hpp>
#include <blink/math.hpp>

namespace blink {
namespace std_params {
namespace delay_time {

static constexpr auto UUID { BLINK_STD_UUID_DELAY_TIME };

auto inline slider()
{
	SliderSpec<float> out;

	out.default_value = math::convert::delay_time_ms_to_linear(100.0f);

	out.constrain = [](float v)
	{
		return std::clamp(v, 0.0f, 1.0f);
	};

	out.decrement = [](float v, bool precise)
	{
		v = math::convert::linear_to_delay_time_ms(v);
		v -= (precise ? 1.0f : 10.0f);
		v = math::convert::delay_time_ms_to_linear(v);

		return v;
	};

	out.drag = [](float v, int amount, bool precise)
	{
		v = math::convert::linear_to_delay_time_ms(v);
		v = tweak::drag<float, 100, 1000>(v, amount / 5, precise);
		v = math::convert::delay_time_ms_to_linear(v);

		return v;
	};

	out.from_string = [](const std::string& str)
	{
	};


	return out;
}

inline auto envelope()
{
	EnvelopeSpec out;

	return out;
}

inline auto envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = UUID;
	out.name = "Time";
	out.long_desc = "Delay Time";
	out.envelope = envelope();
	out.clamp_range = { 0.0f, 1.0f };

	out.flags |= blink_EnvelopeFlags_HostClamp;
	out.flags |= blink_EnvelopeFlags_CanManipulate;
	out.flags |= blink_EnvelopeFlags_IsManipulatorTarget;

	return out;
}

inline auto slider_parameter()
{
	SliderParameterSpec<float> out;

	out.uuid = UUID;
	out.clamp_range = { 0.0f, 1.0f };
	out.name = "Time";
	out.long_desc = "Delay Time";
	out.slider = percentage::slider();

	out.flags |= blink_SliderFlags_HostClamp;
	out.flags |= blink_SliderFlags_CanManipulate;

	return out;
}

inline EnvelopeManipulatorTargetSpec envelope_manipulator_target()
{
	EnvelopeManipulatorTargetSpec out;


	return out;
}

} // feedback
} // std_params
} // blink
