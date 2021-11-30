#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std.hpp>
#include "math.hpp"
#include "chord_spec.hpp"
#include "envelope_spec.hpp"
#include "slider_parameter_spec.hpp"
#include "toggle_spec.hpp"

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

}

namespace amp {

inline auto stepify(float v) { return tweak::std::amp::stepify(v); }
inline auto display(float v) { return tweak::std::amp::to_string(v); }
inline auto constrain(float v) { return tweak::std::amp::constrain(v); }
inline auto from_string(const std::string& str) { return tweak::std::amp::from_string(str); }
inline auto increment(float v, bool precise) { return tweak::std::amp::increment(v, precise); }
inline auto decrement(float v, bool precise) { return tweak::std::amp::decrement(v, precise); }
inline auto drag(float v, int amount, bool precise) { return tweak::std::amp::drag(v, amount, precise); }

} // amp

namespace pan {

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

} // pan

namespace pitch {

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

} // pitch

namespace speed
{
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
}

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

}

namespace sliders
{

inline SliderSpec<float> amp()
{
	SliderSpec<float> out;

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

inline SliderSpec<float> pan()
{
	SliderSpec<float> out;

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

inline SliderSpec<float> pitch()
{
	SliderSpec<float> out;

	out.constrain = pitch::constrain;
	out.increment = pitch::increment;
	out.decrement = pitch::decrement;
	out.drag = pitch::drag;
	out.to_string = [](float v) { return tweak::to_string(v); };
	out.from_string = [](const std::string& str) { return tweak::find_number<float>(str); };
	out.stepify = pitch::stepify;
	out.default_value = 0.0f;

	return out;
}

inline SliderSpec<float> speed()
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

inline SliderSpec<int> sample_offset()
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

template <int MIN = 0, int MAX = 100>
inline SliderSpec<float> percentage()
{
	SliderSpec<float> out;

	out.constrain = [](float v) { return std::clamp(v, float(MIN) / 100.0f, float(MAX) / 100.0f); };
	out.increment = tweak::std::percentage::increment;
	out.decrement = tweak::std::percentage::decrement;
	out.drag = tweak::std::percentage::drag;
	out.to_string = tweak::std::percentage::to_string;
	out.from_string = tweak::std::percentage::from_string;
	out.default_value = 0;

	return out;
}

inline SliderSpec<float> percentage_bipolar()
{
	SliderSpec<float> out;

	out.constrain = tweak::std::percentage::constrain;
	out.increment = tweak::std::percentage_bipolar::increment;
	out.decrement = tweak::std::percentage_bipolar::decrement;
	out.drag = tweak::std::percentage_bipolar::drag;
	out.to_string = tweak::std::percentage_bipolar::to_string;
	out.from_string = tweak::std::percentage_bipolar::from_string;
	out.default_value = 0.5;

	return out;
}

inline SliderSpec<float> filter_frequency()
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

namespace generic {

inline SliderSpec<float> linear(float min, float max, float default_value, float precision)
{
	SliderSpec<float> out;

	out.constrain = [min, max](float v) { return std::clamp(v, min, max); };
	out.increment = [min, max, precision](float v, bool precise) { return v + (precise ? precision * 0.1f : precision); };
	out.decrement = [min, max, precision](float v, bool precise) { return v - (precise ? precision * 0.1f : precision); };
	out.drag = [min, max, precision](float v, int amount, bool precise) { return v + (amount * (precise ? precision * 0.1f : precision)); };
	out.to_string = [](float v) { return tweak::to_string(v); };
	out.from_string = [](const std::string& s) { return tweak::find_number<float>(s); };
	out.default_value = default_value;

	return out;
}

inline SliderSpec<float> ms(float min, float max, float default_value, float precision)
{
	SliderSpec<float> out = linear(min, max, default_value, precision);

	out.stepify = [](float v) { return tweak::std::ms::stepify(v); };
	out.to_string = [](float v) { return tweak::std::ms::to_string(v); };
	out.from_string = [](const std::string& s) { return tweak::std::ms::from_string(s); };

	return out;
}

}  // generic

namespace parameters {

inline SliderParameterSpec<float> amp()
{
	SliderParameterSpec<float> out;

	out.uuid = BLINK_STD_UUID_SLIDER_AMP;
	out.name = "Amp";

	out.slider = sliders::amp();
	out.icon = blink_StdIcon_Amp;
	out.flags = blink_SliderFlags_MovesDisplay;

	return out;
}

inline SliderParameterSpec<float> pan()
{
	SliderParameterSpec<float> out;

	out.uuid = BLINK_STD_UUID_SLIDER_PAN;
	out.name = "Pan";

	out.slider = sliders::pan();
	out.icon = blink_StdIcon_Pan;

	return out;
}

inline SliderParameterSpec<float> pitch()
{
	SliderParameterSpec<float> out;

	out.uuid = BLINK_STD_UUID_SLIDER_PITCH;
	out.name = "Pitch";

	out.slider = sliders::pitch();
	out.icon = blink_StdIcon_Pitch;
	out.flags = blink_SliderFlags_MovesDisplay;

	return out;
}

inline SliderParameterSpec<float> speed()
{
	SliderParameterSpec<float> out;

	out.uuid = BLINK_STD_UUID_SLIDER_SPEED;
	out.name = "Speed";

	out.slider = sliders::speed();
	out.flags = blink_SliderFlags_MovesDisplay;

	return out;
}

inline SliderParameterSpec<int> sample_offset()
{
	SliderParameterSpec<int> out;

	out.uuid = BLINK_STD_UUID_SLIDER_SAMPLE_OFFSET;
	out.name = "Sample Offset";

	out.slider = sliders::sample_offset();

	out.icon = blink_StdIcon_SampleOffset;
	out.flags = blink_SliderFlags_MovesDisplay;

	return out;
}

inline SliderParameterSpec<float> noise_width()
{
	SliderParameterSpec<float> out;

	out.uuid = BLINK_STD_UUID_SLIDER_NOISE_WIDTH;
	out.name = "Noise Width";

	out.slider = std_params::sliders::percentage();

	return out;
}

}

} // sliders

namespace toggles {

inline ToggleSpec loop()
{
	ToggleSpec out;

	out.uuid = BLINK_STD_UUID_TOGGLE_LOOP;
	out.name = "Loop";
	out.flags = blink_ToggleFlags_ShowInContextMenu | blink_ToggleFlags_ShowButton | blink_ToggleFlags_MovesDisplay;
	out.default_value = false;

	return out;
}

inline ToggleSpec reverse()
{
	ToggleSpec out;

	out.uuid = BLINK_STD_UUID_TOGGLE_REVERSE;
	out.name = "Reverse";
	out.flags = blink_ToggleFlags_ShowInContextMenu | blink_ToggleFlags_ShowButton | blink_ToggleFlags_MovesDisplay;
	out.default_value = false;

	return out;
}

} // toggles

namespace options {

inline OptionSpec noise_mode()
{
	OptionSpec out;

	out.uuid = BLINK_STD_UUID_OPTION_NOISE_MODE;
	out.name = "Noise Mode";
	out.default_index = 0;
	out.options = {
		"Multiply",
		"Mix",
	};

	return out;
}

}

namespace envelopes {

// returns the y value at the given block position
// [search_beg_index] is the index of the point to begin searching from
// [left] returns the index of the point to the left of the block position,
//        or zero if there isn't one.
//        in some scenarios this can be passed as search_beg_index to
//        speed up the search in the next iteration
template <class SearchFunc>
inline float generic_search(const blink_EnvelopeData* data, float default_value, blink_Position block_position, int search_beg_index, int* left, SearchFunc search)
{
	*left = 0;

	const auto clamp = [data](float value)
	{
		return std::clamp(value, data->min, data->max);
	};

	if (data->points.count < 1) return clamp(default_value);
	if (data->points.count == 1) return clamp(data->points.points[0].position.y);

	auto search_beg = data->points.points + search_beg_index;
	auto search_end = data->points.points + data->points.count;
	const auto pos = search(search_beg, search_end);

	if (pos == search_beg)
	{
		// It's the first point
		return clamp(pos->position.y);
	}

	if (pos == search_end)
	{
		// No points to the right so we're at the end of the envelope
		*left = int(std::distance<const blink_EnvelopePoint*>(data->points.points, (pos - 1)));

		return clamp((pos - 1)->position.y);
	}

	// We're somewhere in between two envelope points. Linear interpolate
	// between them.
	const auto p0 = (pos - 1)->position;
	const auto p1 = pos->position;

	const auto segment_size = p1.x - p0.x;	// Should never be zero
	const auto r = (block_position - p0.x) / segment_size;

	*left = int(std::distance<const blink_EnvelopePoint*>(data->points.points, (pos - 1)));

	return math::lerp(clamp(p0.y), clamp(p1.y), float(r));
}

// Use a binary search to locate the envelope position
inline float generic_search_binary(const blink_EnvelopeData* data, float default_value, blink_Position block_position, int search_beg_index, int* left)
{
	const auto find = [block_position](const blink_EnvelopePoint* beg, const blink_EnvelopePoint* end)
	{
		const auto less = [](blink_Position position, const blink_EnvelopePoint& point)
		{
			return position < point.position.x;
		};

		return std::upper_bound(beg, end, block_position, less);
	};

	return generic_search(data, default_value, block_position, search_beg_index, left, find);
}

// Use a forward search to locate the envelope position (can be
// faster when envelope is being traversed forwards)
inline float generic_search_forward(const blink_EnvelopeData* data, float default_value, blink_Position block_position, int search_beg_index, int* left)
{
	const auto find = [block_position](const blink_EnvelopePoint* beg, const blink_EnvelopePoint* end)
	{
		const auto greater = [block_position](const blink_EnvelopePoint& point)
		{
			return point.position.x > block_position;
		};

		return std::find_if(beg, end, greater);
	};

	return generic_search(data, default_value, block_position, search_beg_index, left, find);
}

inline EnvelopeSpec amp()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_AMP;
	out.name = "Amp";

	out.get_gridline = [](int index) -> float
	{
		return math::convert::linear_to_speed(float(index));
	};

	out.default_value = 1.0f;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;
	out.to_string = amp::display;
	out.stepify = amp::stepify;

	out.value_slider = sliders::amp();

	out.range.min.default_value = 0.0f;
	out.range.min.to_string = amp::display;

	out.range.max.default_value = 1.0f;
	out.range.max.to_string = amp::display;
	out.range.max.from_string = amp::from_string;
	out.range.max.constrain = amp::constrain;
	out.range.max.increment = amp::increment;
	out.range.max.decrement = amp::decrement;
	out.range.max.drag = amp::drag;

	out.flags = blink_EnvelopeFlags_MovesDisplay;

	return out;
}

inline EnvelopeSpec pan()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_PAN;
	out.name = "Pan";

	out.default_value = 0.0f;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;
	out.stepify = pan::stepify;

	out.to_string = pan::display;
	out.value_slider = sliders::pan();

	out.range.min.default_value = -1.0f;
	out.range.min.to_string = pan::display;

	out.range.max.default_value = 1.0f;
	out.range.max.to_string = pan::display;

	out.flags = blink_EnvelopeFlags_NoGridLabels;

	return out;
}

inline EnvelopeSpec pitch()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_PITCH;
	out.name = "Pitch";

	out.default_value = 0.0f;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;
	out.stepify = pitch::stepify;
	out.snap_value = tweak::snap_value;

	out.value_slider = sliders::pitch();

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
	out.range.min.from_string = [](const std::string& str) { return tweak::find_number<float>(str); };

	out.range.max.constrain = pitch::constrain;
	out.range.max.decrement = pitch::decrement;
	out.range.max.increment = pitch::increment;
	out.range.max.default_value = 24.0f;
	out.range.max.to_string = [](float v) { return tweak::to_string(v); };
	out.range.max.drag = pitch::drag;
	out.range.max.from_string = [](const std::string& str) { return tweak::find_number<float>(str); };

	out.step_size.constrain = [](float v) { return tweak::constrain(v, 0.0f, 60.0f); };
	out.step_size.decrement = [out](float v, bool precise) { return out.step_size.constrain(tweak::decrement<1, 10>(v, precise)); };
	out.step_size.increment = [out](float v, bool precise) { return out.step_size.constrain(tweak::increment<1, 10>(v, precise)); };
	out.step_size.default_value = 1.0f;
	out.step_size.to_string = [](float v) { return tweak::to_string(v); };
	out.step_size.drag = [out](float v, int amount, bool precise) { return out.step_size.constrain(pitch::drag(v, amount, precise)); };
	out.step_size.from_string = [](const std::string& str) { return tweak::find_number<float>(str); };

	out.default_snap_amount = 1.0f;

	out.flags = blink_EnvelopeFlags_MovesDisplay;

	return out;
}

inline EnvelopeSpec speed()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_SPEED;
	out.name = "Speed";

	out.default_value = speed::NORMAL;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;
	out.to_string = speed::display;

	out.value_slider = sliders::speed();

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

	out.flags = blink_EnvelopeFlags_MovesDisplay;

	return out;
}

inline EnvelopeSpec formant()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_FORMANT;
	out.name = "Formant";

	out.default_value = 0.5f;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;
	out.stepify = tweak::std::percentage_bipolar::stepify;

	out.value_slider = sliders::percentage_bipolar();

	out.range.min.default_value = 0.0f;
	out.range.min.to_string = tweak::std::percentage_bipolar::to_string;
	out.range.max.default_value = 1.0f;
	out.range.max.to_string = tweak::std::percentage_bipolar::to_string;
	out.to_string = tweak::std::percentage_bipolar::to_string;
	out.flags = blink_EnvelopeFlags_NoGridLabels;

	return out;
}

inline EnvelopeSpec noise_amount()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_NOISE_AMOUNT;
	out.name = "Noise Amount";
	out.short_name = "Amount";

	out.default_value = 0.0f;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;
	out.stepify = tweak::std::percentage::stepify;

	out.value_slider = sliders::percentage();

	out.range.min.default_value = 0.0f;
	out.range.min.to_string = tweak::std::percentage::to_string;
	out.range.max.default_value = 1.0f;
	out.range.max.to_string = tweak::std::percentage::to_string;
	out.to_string = tweak::std::percentage::to_string;

	return out;
}

inline EnvelopeSpec noise_color()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_NOISE_COLOR;
	out.name = "Noise Color";
	out.short_name = "Color";

	out.default_value = 0.5f;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;
	out.stepify = tweak::std::percentage_bipolar::stepify;

	out.value_slider = sliders::percentage_bipolar();

	out.range.min.default_value = 0.0f;
	out.range.min.to_string = tweak::std::percentage_bipolar::to_string;
	out.range.max.default_value = 1.0f;
	out.range.max.to_string = tweak::std::percentage_bipolar::to_string;
	out.to_string = tweak::std::percentage_bipolar::to_string;
	out.flags = blink_EnvelopeFlags_NoGridLabels;

	return out;
}

inline EnvelopeSpec filter_frequency()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_FILTER_FREQUENCY;
	out.name = "Frequency";

	out.default_value = 0.52833f;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;

	out.value_slider = sliders::filter_frequency();

	out.range.min.default_value = 0.0f;
	out.range.min.to_string = filter_frequency::display;
	out.range.max.default_value = 1.0f;
	out.range.max.to_string = filter_frequency::display;
	out.to_string = filter_frequency::display;

	return out;
}

inline EnvelopeSpec resonance()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_FILTER_RESONANCE;
	out.name = "Resonance";

	out.default_value = 0.0f;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;
	out.stepify = tweak::std::percentage::stepify;

	out.value_slider = sliders::percentage();

	out.range.min.default_value = 0.0f;
	out.range.min.to_string = tweak::std::percentage::to_string;
	out.range.max.default_value = 1.0f;
	out.range.max.to_string = tweak::std::percentage::to_string;
	out.to_string = tweak::std::percentage::to_string;

	return out;
}

inline EnvelopeSpec mix()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_MIX;
	out.name = "Mix";

	out.default_value = 1.0f;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;
	out.stepify = tweak::std::percentage::stepify;
	out.value_slider = sliders::percentage();
	out.range.min.default_value = 0.0f;
	out.range.min.to_string = tweak::std::percentage::to_string;
	out.range.max.default_value = 1.0f;
	out.range.max.to_string = tweak::std::percentage::to_string;
	out.to_string = tweak::std::percentage::to_string;

	return out;
}

namespace generic
{
	template <int MIN = 0, int MAX = 100>
	inline EnvelopeSpec percentage()
	{
		EnvelopeSpec out;

		out.default_value = 0.0f;
		out.search_binary = generic_search_binary;
		out.search_forward = generic_search_forward;
		out.stepify = tweak::std::percentage::stepify;
		out.value_slider = sliders::percentage<MIN, MAX>();
		out.range.min.default_value = float(MIN) / 100.0f;
		out.range.min.to_string = tweak::std::percentage::to_string;
		out.range.max.default_value = float(MAX) / 100.0f;
		out.range.max.to_string = tweak::std::percentage::to_string;
		out.to_string = tweak::std::percentage::to_string;

		return out;
	}

	inline EnvelopeSpec percentage_bipolar()
	{
		EnvelopeSpec out;

		out.default_value = 0.5f;
		out.search_binary = generic_search_binary;
		out.search_forward = generic_search_forward;
		out.stepify = tweak::std::percentage_bipolar::stepify;
		out.value_slider = sliders::percentage_bipolar();
		out.range.min.default_value = 0.0f;
		out.range.min.to_string = tweak::std::percentage_bipolar::to_string;
		out.range.max.default_value = 1.0f;
		out.range.max.to_string = tweak::std::percentage_bipolar::to_string;
		out.to_string = tweak::std::percentage_bipolar::to_string;

		return out;
	}

	inline EnvelopeSpec linear(float min, float max, float default_value)
	{
		EnvelopeSpec out;

		out.default_value = default_value;
		out.search_binary = generic_search_binary;
		out.search_forward = generic_search_forward;
		out.value_slider = sliders::generic::linear(min, max, default_value, 1.0f);
		out.range.min.default_value = min;
		out.range.min.to_string = [](float v){ return tweak::to_string(v); };
		out.range.max.default_value = max;
		out.range.max.to_string = [](float v) { return tweak::to_string(v); };
		out.to_string = [](float v) { return tweak::to_string(v); };

		return out;
	}

	inline EnvelopeSpec ms(float min, float max, float default_value)
	{
		EnvelopeSpec out;

		out.default_value = default_value;
		out.search_binary = generic_search_binary;
		out.search_forward = generic_search_forward;
		out.stepify = tweak::std::ms::stepify;
		out.value_slider = sliders::generic::ms(min, max, default_value, 1.0f);
		out.range.min.default_value = min;
		out.range.min.to_string = [](float v){ return tweak::std::ms::to_string(v); };
		out.range.max.default_value = max;
		out.range.max.to_string = [](float v) { return tweak::std::ms::to_string(v); };
		out.to_string = [](float v) { return tweak::std::ms::to_string(v); };

		return out;
	}
}

} // envelopes

namespace chords {

// returns the scale value at the given block position
// [search_beg_index] is the index of the scale transition to begin searching from
// [left] returns the index of the scale transition to the left of the block position,
//        or zero if there isn't one.
//        in some scenarios this can be passed as search_beg_index to
//        speed up the search in the next iteration
template <class SearchFunc>
inline int generic_search(const blink_ChordData* data, blink_Position block_position, int search_beg_index, int* left, SearchFunc search)
{
	*left = 0;

	if (data->blocks.count < 2) return 0;

	auto search_beg = data->blocks.blocks + search_beg_index;
	auto search_end = data->blocks.blocks + data->blocks.count;
	const auto pos = search(search_beg, search_end);

	if (pos == search_beg)
	{
		// The scale to the right is the first one
		return 0;
	}

	*left = int(std::distance<const blink_ChordBlock*>(data->blocks.blocks, (pos - 1)));

	if (pos == search_end)
	{
		// Nothing to the right so we're at the end
		return 0;
	}

	// We're somewhere in between two scale transitions.
	// Return the scale on the left
	return (pos - 1)->scale;
}

// Use a binary search to locate the envelope position
inline int generic_search_binary(const blink_ChordData* data, blink_Position block_position, int search_beg_index, int* left)
{
	const auto find = [block_position](const blink_ChordBlock* beg, const blink_ChordBlock* end)
	{
		const auto less = [](blink_Position position, const blink_ChordBlock& block)
		{
			return position < block.position;
		};

		return std::upper_bound(beg, end, block_position, less);
	};

	return generic_search(data, block_position, search_beg_index, left, find);
}

// Use a forward search to locate the envelope position (can be
// faster when envelope is being traversed forwards)
inline int generic_search_forward(const blink_ChordData* data, blink_Position block_position, int search_beg_index, int* left)
{
	const auto find = [block_position](const blink_ChordBlock* beg, const blink_ChordBlock* end)
	{
		const auto greater = [block_position](const blink_ChordBlock& block)
		{
			return block.position > block_position;
		};

		return std::find_if(beg, end, greater);
	};

	return generic_search(data, block_position, search_beg_index, left, find);
}


inline ChordSpec scale()
{
	ChordSpec out;

	out.uuid = BLINK_STD_UUID_CHORD_SCALE;
	out.name = "Scale";
	out.icon = blink_StdIcon_PianoRoll;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;

	return out;
}
}

}}
