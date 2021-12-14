#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std.hpp>
#include <blink/math.hpp>
#include <blink/slider_spec.hpp>
#include <blink/slider_parameter_spec.hpp>

namespace blink {
namespace std_params {
namespace sample_offset {

inline auto constrain(int v)
{
	return v;
};

inline auto from_string(const std::string& str) -> std::optional<float>
{
	auto db = tweak::find_number<float>(str);

	if (!db) return db;

	return math::convert::db_to_linear(*db);
};

inline auto increment(int v, bool precise)
{
	return v + 1;
};

inline auto decrement(int v, bool precise)
{
	return v - 1;
};

inline auto drag(int v, int amount, bool precise) -> int
{
	return v + (amount / (precise ? 50 : 1));
};

inline SliderSpec<int> slider()
{
	SliderSpec<int> out;

	out.constrain = sample_offset::constrain;
	out.increment = sample_offset::increment;
	out.decrement = sample_offset::decrement;
	out.drag = sample_offset::drag;
	out.to_string = [](int v) { return tweak::to_string(v); };
	out.from_string = [](const std::string& str) { return tweak::find_number<int>(str); };
	out.default_value = 0;

	return out;
}

inline SliderParameterSpec<int> slider_parameter()
{
	SliderParameterSpec<int> out;

	out.uuid = BLINK_STD_UUID_SLIDER_SAMPLE_OFFSET;
	out.name = "Sample Offset";

	out.slider = slider();

	out.icon = blink_StdIcon_SampleOffset;
	out.flags = blink_SliderFlags_MovesDisplay;

	return out;
}

} // sample_offset
} // blink
} // std_params