#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std.hpp>
#include <blink/envelope_parameter_spec.hpp>
#include <blink/math.hpp>

namespace blink {
namespace std_params {
namespace filter_frequency {

inline auto linear2hz(float v)
{
	return std::pow(v, 2.0f) * 20000.0f;
}

inline float constrain(float v)
{
	return std::clamp(v, 0.0f, 1.0f);
};

inline auto stepify(float v) -> float
{
	return v;
}

inline auto display(float v)
{
	const auto hz = math::convert::linear_to_filter_hz(v);

	std::stringstream ss;

	if (hz >= 1000.0f)
	{
		ss << math::stepify(hz / 1000.0f, 0.1f) << " MHz";
	}
	else
	{
		ss << math::stepify(hz, 0.1f) << " Hz";
	}

	return ss.str();
}

inline auto from_string(const std::string& str) -> std::optional<float>
{
	auto value = tweak::find_number<float>(str);

	if (!value) return std::optional<float>();

	std::string uppercase = str;

	std::transform(str.begin(), str.end(), uppercase.begin(), ::toupper);

	if (uppercase.find("MHZ") != std::string::npos)
	{
		return math::convert::filter_hz_to_linear((*value) * 1000.0f);
	}

	return math::convert::filter_hz_to_linear(*value);
};

inline auto increment(float v, bool precise)
{
	return tweak::increment<100, 1000>(v, precise);
};

inline auto decrement(float v, bool precise)
{
	return tweak::decrement<100, 1000>(v, precise);
};

inline auto drag(float v, int amount, bool precise) -> float
{
	return tweak::drag<float, 100, 1000>(v, amount / 5, precise);
};

inline SliderSpec<float> slider()
{
	SliderSpec<float> out;

	out.constrain = filter_frequency::constrain;
	out.increment = filter_frequency::increment;
	out.decrement = filter_frequency::decrement;
	out.drag = filter_frequency::drag;
	out.to_string = filter_frequency::display;
	out.from_string = filter_frequency::from_string;
	out.default_value = 0;

	return out;
}

inline EnvelopeSpec envelope()
{
	EnvelopeSpec out;

	out.default_value = 0.52833f;
	out.search_binary = search::envelope_binary;
	out.search_forward = search::envelope_forward;

	out.value_slider = slider();

	out.range.min.default_value = 0.0f;
	out.range.min.to_string = filter_frequency::display;
	out.range.max.default_value = 1.0f;
	out.range.max.to_string = filter_frequency::display;
	out.to_string = filter_frequency::display;

	return out;
}

inline EnvelopeParameterSpec envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_FILTER_FREQUENCY;
	out.name = "Frequency";

	out.envelope = envelope();

	return out;
}

} // filter_frequency
} // blink
} // std_params