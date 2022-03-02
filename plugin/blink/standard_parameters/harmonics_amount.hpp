#pragma once

#include <blink/parameters/envelope_parameter_spec.hpp>
#include <blink/parameters/envelope_manipulator_target_spec.hpp>
#include <blink/standard_parameters/percentage.hpp>

namespace blink {
namespace std_params {
namespace harmonics_amount {

static constexpr auto UUID { "a8a6e4fa-6713-48bb-a888-65ac437384b7" };

inline auto slider()
{
	blink::SliderSpec<float> out;

	out.constrain = [](float v) { return tweak::constrain(v, 0.0f, 3.0f); };
    out.stepify = tweak::math::stepify<100, float>;
	out.increment = [out](float v, bool precise) { return out.constrain(out.stepify(tweak::increment<1, 10>(v, precise))); };
	out.decrement = [out](float v, bool precise) { return out.constrain(out.stepify(tweak::decrement<1, 10>(v, precise))); };
	out.drag = [out](float v, int amount, bool precise) { return out.constrain(out.stepify(tweak::drag<float, 10, 100>(v, amount / 5, precise))); };
    out.to_string = [](float v) { return tweak::to_string(v); };
	out.from_string = tweak::find_positive_number<float>;
	out.default_value = 0.0f;

	return out;
}

inline auto offset_slider()
{
	auto out { slider() };

	out.constrain = [](float v) { return tweak::constrain(v, -3.0f, 3.0f); };
	out.from_string = tweak::find_number<float>;

	return out;
}

inline auto envelope()
{
	blink::EnvelopeSpec out;

	out.get_gridline = [](int index) -> float
	{
		return float(index);
	};

	out.default_value = 0.0f;
	out.searcher.binary = blink::std_params::search::float_points_binary;
	out.searcher.forward = blink::std_params::search::float_points_forward;
    out.stepify = tweak::math::stepify<100, float>;

	out.value_slider = slider();

	out.range.min.default_value = 0.0f;
    out.range.min.to_string = [](float v) { return std::to_string(int(v)); };
	out.range.max.default_value = 3.0f;
    out.range.max.to_string = [](float v) { return std::to_string(int(v)); };
    out.to_string = [](float v) { return std::to_string(int(v)); };

	return out;
}

inline auto offset_envelope()
{
	blink::EnvelopeSpec out { envelope() };

	out.value_slider = offset_slider();
	out.range.min.default_value = -3.0f;

	return out;
}

inline auto envelope_parameter()
{
	blink::EnvelopeParameterSpec out;

	out.uuid = UUID;
	out.name = "Harmonics Amount";
	out.short_name = "Amount";
	out.envelope = envelope();
	out.clamp_range = { 0.0f, 3.0f };

	out.flags |= blink_EnvelopeFlags_HostClamp;
	out.flags |= blink_EnvelopeFlags_CanManipulate;
	out.flags |= blink_EnvelopeFlags_IsManipulatorTarget;

	return out;
}

auto inline envelope_manipulator_target()
{
	blink::EnvelopeManipulatorTargetSpec out;

	out.offset_envelope = offset_envelope();
	out.override_envelope = envelope();

	return out;
}

} // harmonics_amount
} // params
} // fudge
