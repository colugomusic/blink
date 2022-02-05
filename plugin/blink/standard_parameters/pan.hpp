#pragma once

#include <tweak/tweak.hpp>
#include <blink/math.hpp>
#include <blink/slider_spec.hpp>
#include <blink/parameters/slider_parameter_spec.hpp>

namespace blink {
namespace std_params {
namespace pan {

static constexpr auto UUID { BLINK_STD_UUID_PAN };

inline auto stepify(float v) -> float
{
	return tweak::math::stepify<100>(v);
}

inline float constrain(float v)
{
	if (v < -1.0f) return -1.0f;
	if (v > 1.0f) return 1.0f;

	return v;
};

inline std::string display(float v)
{
	std::stringstream ss;

	if (v < 0.0f)
	{
		ss << stepify(std::abs(v * 100)) << "% L";
	}
	else if (v > 0.0f)
	{
		ss << stepify(v * 100) << "% R";
	}
	else
	{
		ss << "Center";
	}

	return ss.str();
}
	
inline auto increment(float v, bool precise)
{
	return tweak::increment<100>(v);
};

inline auto decrement(float v, bool precise)
{
	return tweak::decrement<100>(v);
};

inline auto drag(float v, int amount, bool precise) -> float
{
	return tweak::drag<float, 500, 5000>(v, amount, precise);
};

inline auto from_string(const std::string& str) -> std::optional<float>
{
	std::string uppercase = str;

	std::transform(str.begin(), str.end(), uppercase.begin(), ::toupper);

	if (uppercase.find("CENTER") != std::string::npos) return 0.0f;

	const auto negative = uppercase.find('L') != std::string::npos || uppercase.find('-') != std::string::npos;

	auto value = tweak::find_positive_number<int>(str);

	if (!value) return std::optional<float>();

	return (float(*value) / 100) * (negative ? -1 : 1);
};

inline SliderSpec<float> slider()
{
	SliderSpec<float> out;

	out.searcher.binary = search::float_points_binary;
	out.searcher.forward = search::float_points_forward;
	out.constrain = pan::constrain;
	out.increment = pan::increment;
	out.decrement = pan::decrement;
	out.drag = pan::drag;
	out.from_string = pan::from_string;
	out.to_string = pan::display;
	out.stepify = pan::stepify;
	out.default_value = 0.0f;

	return out;
}

inline SliderParameterSpec<float> slider_parameter()
{
	SliderParameterSpec<float> out;

	out.uuid = BLINK_STD_UUID_PAN;
	out.name = "Pan";

	out.slider = slider();
	out.icon = blink_StdIcon_Pan;
	out.flags |= blink_SliderFlags_CanManipulate;

	return out;
}

inline EnvelopeSpec envelope()
{
	EnvelopeSpec out;

	out.default_value = 0.0f;
	out.searcher.binary = search::float_points_binary;
	out.searcher.forward = search::float_points_forward;
	out.stepify = pan::stepify;

	out.show_grid_labels = false;
	out.to_string = pan::display;
	out.value_slider = slider();

	out.range.min.default_value = -1.0f;
	out.range.min.to_string = pan::display;

	out.range.max.default_value = 1.0f;
	out.range.max.to_string = pan::display;

	return out;
}

inline EnvelopeParameterSpec envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = BLINK_STD_UUID_PAN;
	out.name = "Pan";
	out.envelope = envelope();

	out.flags |= blink_EnvelopeFlags_CanManipulate;
	out.flags |= blink_EnvelopeFlags_IsManipulatorTarget;

	return out;
}

inline EnvelopeManipulatorTargetSpec envelope_manipulator_target()
{
	EnvelopeManipulatorTargetSpec out;

	out.offset_envelope = envelope();
	out.override_envelope = envelope();

	return out;
}

} // pan
} // blink
} // std_params