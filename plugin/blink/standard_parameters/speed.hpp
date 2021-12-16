#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std.hpp>
#include <blink/math.hpp>
#include <blink/slider_spec.hpp>
#include <blink/parameters/slider_parameter_spec.hpp>

namespace blink {
namespace std_params {
namespace speed {

constexpr auto FREEZE = 0.0f;
constexpr auto THIRTYSECOND = 0.03125f;
constexpr auto SIXTEENTH = 0.0625f;
constexpr auto EIGHTH = 0.125f;
constexpr auto QUARTER = 0.25f;
constexpr auto HALF = 0.5f;
constexpr auto NORMAL = 1.0f;
constexpr auto DOUBLE = 2.0f;
constexpr auto TRIPLE = 3.0f;

inline auto constrain(float v)
{
    if (v < math::convert::linear_to_speed(-32.0f)) return FREEZE;
    if (v > 32.0f) return 32.0f;

    return v;
};

inline auto increment(float v, bool precise)
{
    if (v <= FREEZE) return math::convert::linear_to_speed(-32.0f);

    return constrain(math::convert::linear_to_speed(tweak::increment<1, 10>(math::convert::speed_to_linear(v), precise)));
};

inline auto decrement(float v, bool precise)
{
    return constrain(math::convert::linear_to_speed(tweak::decrement<1, 10>(math::convert::speed_to_linear(v), precise)));
};

inline auto drag(float v, int amount, bool precise) -> float
{
    if (v <= FREEZE) v = math::convert::linear_to_speed(-32.0f);

    return constrain(math::convert::linear_to_speed(tweak::drag<float, 1, 10>(math::convert::speed_to_linear(v), amount / 5, precise)));
};

inline auto from_string(const std::string& str) -> std::optional<float>
{
    std::string uppercase = str;

    std::transform(str.begin(), str.end(), uppercase.begin(), ::toupper);

    if (uppercase.find("FREEZE") != std::string::npos) return FREEZE;
    if (uppercase.find("NORMAL") != std::string::npos) return NORMAL;
    if (uppercase.find("DOUBLE") != std::string::npos) return DOUBLE;
    if (uppercase.find("TRIPLE") != std::string::npos) return TRIPLE;

    std::regex r("1/([\\d]+)");
    std::smatch match;

    if (std::regex_search(str, match, r))
    {
        return 1.0f / std::stoi(match[1]);
    }

    auto ff = tweak::find_number<float>(str);

    if (!ff) return ff;

    return *ff;
};

inline auto display(float v)
{
    constexpr auto threshold = 0.001f;

    const auto milestone_hit = [threshold](float value, float milestone)
    {
        return value > milestone - threshold && value < milestone + threshold;
    };

    std::stringstream ss;

    if (v <= FREEZE)
    {
        ss << "Freeze";
    }
    else if (v < 1.0f - threshold)
    {
        const auto recip = 1.0f / v;
        const auto rounded_recip = std::round(recip);

        if (std::abs(recip - rounded_recip) < threshold)
        {
            ss << "1/" << rounded_recip;
        }
        else
        {
            ss << "x" << v;
        }
    }
    else if (milestone_hit(v, NORMAL))
    {
        ss << "Normal";
    }
    else if (milestone_hit(v, DOUBLE))
    {
        ss << "Double";
    }
    else if (milestone_hit(v, TRIPLE))
    {
        ss << "Triple";
    }
    else
    {
        ss << "x" << v;
    }

    return ss.str();
}

inline SliderSpec<float> slider()
{
	SliderSpec<float> out;

	out.constrain = speed::constrain;
	out.increment = speed::increment;
	out.decrement = speed::decrement;
	out.drag = speed::drag;
	out.to_string = speed::display;
	out.from_string = speed::from_string;
	out.default_value = 1.0f;

	return out;
}

inline SliderParameterSpec<float> slider_parameter()
{
	SliderParameterSpec<float> out;

	out.uuid = BLINK_STD_UUID_SPEED;
	out.name = "Speed";

	out.slider = slider();
	out.flags = blink_SliderFlags_MovesDisplay;

	return out;
}

inline EnvelopeSpec envelope()
{
	EnvelopeSpec out;

	out.default_value = speed::NORMAL;
	out.searcher.binary = search::float_points_binary;
	out.searcher.forward = search::float_points_forward;
	out.to_string = speed::display;

	out.value_slider = slider();

	out.get_gridline = [](int index) -> float
	{
		return math::convert::linear_to_speed(float(index));
	};

	out.range.min.constrain = speed::constrain;
	out.range.min.increment = speed::increment;
	out.range.min.decrement = speed::decrement;
	out.range.min.to_string = speed::display;
	out.range.min.drag = speed::drag;
	out.range.min.from_string = speed::from_string;
	out.range.min.default_value = speed::FREEZE;

	out.range.max.constrain = speed::constrain;
	out.range.max.increment = speed::increment;
	out.range.max.decrement = speed::decrement;
	out.range.max.to_string = speed::display;
	out.range.max.drag = speed::drag;
	out.range.max.from_string = speed::from_string;
	out.range.max.default_value = 2.0f;

	return out;
}

inline EnvelopeParameterSpec envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = BLINK_STD_UUID_SPEED;
	out.name = "Speed";
	out.envelope = envelope();
	out.flags = blink_EnvelopeFlags_MovesDisplay;

	return out;
}

} // speed
} // blink
} // std_params