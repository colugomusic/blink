#pragma once

#include <optional>
#include <regex>
#include <sstream>
#include "envelope_spec.hpp"
#include "slider_parameter_spec.hpp"

namespace blkhdgen {
namespace std_params {

template <class T> std::optional<T> find_number(const std::string& str);
template <class T> std::optional<T> find_positive_number(const std::string& str);

template <>
std::optional<float> find_number<float>(const std::string& str)
{
	std::regex r("(\\-?\\s*[\\.\\d]+)");
	std::smatch match;

	if (!std::regex_search(str, match, r)) return std::optional<float>();

	return std::stof(match[0].str());
}

template <>
std::optional<int> find_number<int>(const std::string& str)
{
	std::regex r("(\\-?\\s*[\\d]+)");
	std::smatch match;

	if (!std::regex_search(str, match, r)) return std::optional<int>();

	return std::stoi(match[0].str());
}

template <>
std::optional<int> find_positive_number<int>(const std::string& str)
{
	std::regex r("([\\d]+)");
	std::smatch match;

	if (!std::regex_search(str, match, r)) return std::optional<int>();

	return std::stoi(match[0].str());
}

template <int Normal, int Precise>
inline float increment(float v, bool precise)
{
	return v + 1.0f / (precise ? Precise : Normal);
}

template <int Normal, int Precise>
inline float decrement(float v, bool precise)
{
	return v - 1.0f / (precise ? Precise : Normal);
}

template <int Normal>
inline float increment(float v)
{
	return v + 1.0f / (Normal);
}

template <int Normal>
inline float decrement(float v)
{
	return v - 1.0f / (Normal);
}

template <int Normal, int Precise>
inline float drag(float v, int amount, bool precise)
{
	return v + (float(amount) / (precise ? Precise : Normal));
}

inline float constrain(float v, float min, float max)
{
	if (v < min) return min;
	if (v > max) return max;

	return v;
}

inline auto display_number(float v)
{
	std::stringstream ss;

	ss << v;

	return ss.str();
};

namespace amp
{
	inline auto constrain(float v)
	{
		const auto db = math::linear2db(v);

		if (db < -60.0f) return 0.0f;
		if (db > 12.0f) return math::db2linear(12.0f);

		return v;
	};

	inline auto from_string(const std::string& str) -> std::optional<float>
	{
		auto db = find_number<float>(str);

		if (!db) return db;

		return math::db2linear(*db);
	};

	inline auto display(float v)
	{
		std::stringstream ss;

		ss << math::stepify(float(math::linear2db(v)), 0.01f) << " dB";

		return ss.str();
	}

	inline auto increment(float v, bool precise)
	{
		if (v <= 0.0f) return math::db2linear(-60.0f);

		return constrain(math::db2linear(std_params::increment<1, 10>(math::linear2db(v), precise)));
	};

	inline auto decrement(float v, bool precise)
	{
		return constrain(math::db2linear(std_params::decrement<1, 10>(math::linear2db(v), precise)));
	};

	inline auto drag(float v, int amount, bool precise) -> float
	{
		if (v <= 0.0f) v = math::db2linear(-61.0f);

		return constrain(math::db2linear(math::stepify(std_params::drag<1, 10>(math::linear2db(v), amount / 5, precise), 0.01f)));
	};
}

namespace pan
{
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
			ss << std::abs(v * 100) << "% L";
		}
		else if (v > 0.0f)
		{
			ss << v * 100 << "% R";
		}
		else
		{
			ss << "Center";
		}

		return ss.str();
	}
	
	inline auto increment(float v, bool precise)
	{
		return constrain(math::stepify(std_params::increment<100>(v), 0.01f));
	};

	inline auto decrement(float v, bool precise)
	{
		return constrain(math::stepify(std_params::decrement<100>(v), 0.01f));
	};

	inline auto drag(float v, int amount, bool precise) -> float
	{
		return constrain(math::stepify(std_params::drag<500, 5000>(v, amount, precise), 0.01f));
	};

	inline auto from_string(const std::string& str) -> std::optional<float>
	{
		std::string uppercase = str;

		std::transform(str.begin(), str.end(), uppercase.begin(), ::toupper);

		if (uppercase.find("CENTER") != std::string::npos) return 0.0f;

		const auto negative = uppercase.find('L') != std::string::npos || uppercase.find('-') != std::string::npos;

		auto value = find_positive_number<int>(str);

		if (!value) return std::optional<float>();

		return (float(*value) / 100) * (negative ? -1 : 1);
	};
}

namespace pitch
{
	inline float constrain(float v)
	{
		if (v < -60.0f) return -60.0f;
		if (v > 60.0f) return 60.0f;

		return v;
	};

	inline auto increment(float v, bool precise)
	{
		return constrain(math::stepify(std_params::increment<1, 10>(v, precise), 0.1f));
	};

	inline auto decrement(float v, bool precise)
	{
		return constrain(math::stepify(std_params::decrement<1, 10>(v, precise), 0.1f));
	};

	inline auto drag(float v, int amount, bool precise) -> float
	{
		return constrain(math::stepify(std_params::drag<1, 10>(v, amount / 5, precise), 0.1f));
	};
}

namespace speed
{
	constexpr auto FREEZE = -10.0f;
	constexpr auto HALF = -1.0f;
	constexpr auto NORMAL = 0.0f;
	constexpr auto DOUBLE = 1.0f;
	static auto TRIPLE = math::speed2linear(3.0f);

	inline auto constrain(float v)
	{
		if (v < -5.0f) return FREEZE;
		if (v > 5.0f) return 5.0f;
	};

	inline auto increment(float v, bool precise)
	{
		if (v <= 5.0f) return -5.0f;

		return constrain(std_params::increment<1, 10>(v, precise));
	};

	inline auto decrement(float v, bool precise)
	{
		return constrain(std_params::decrement<1, 10>(v, precise));
	};

	inline auto drag(float v, int amount, bool precise) -> float
	{
		if (v <= 5.0f) v = FREEZE;

		return constrain(std_params::drag<1, 10>(v, amount / 5, precise));
	};

	inline auto from_string(const std::string& str) -> std::optional<float>
	{
		std::string uppercase = str;

		std::transform(str.begin(), str.end(), uppercase.begin(), ::toupper);

		if (uppercase.find("FREEZE") != std::string::npos) return FREEZE;
		if (uppercase.find("1/8") != std::string::npos) return -3.0f;
		if (uppercase.find("1/4") != std::string::npos) return -2.0f;
		if (uppercase.find("1/2") != std::string::npos) return HALF;
		if (uppercase.find("NORMAL") != std::string::npos) return NORMAL;
		if (uppercase.find("DOUBLE") != std::string::npos) return DOUBLE;

		auto ff = find_number<float>(str);

		if (!ff) return ff;

		return math::speed2linear(*ff);
	};

	inline auto display(float v)
	{
		std::stringstream ss;

		if (v < -0.5f)
		{
			ss << "Freeze";
		}
		else if (v == -3.0f)
		{
			ss << "1/8";
		}
		else if (v == -2.0f)
		{
			ss << v << "1/4";
		}
		else if (v == HALF)
		{
			ss << v << "1/2";
		}
		else if (v == NORMAL)
		{
			ss << v << "Normal";
		}
		else if (v == DOUBLE)
		{
			ss << v << "Double";
		}
		else
		{
			ss << "x" << math::linear2speed(v);
		}

		return ss.str();
	}
}

namespace envelopes {

inline EnvelopeSpec amp()
{
	EnvelopeSpec out;

	out.uuid = "273e7c30-404b-4db6-ba97-20f33d49fe51";
	out.name = "Amp";

	out.get_grid_line = [](int index) -> float
	{
		return math::linear2db(math::linear2speed(float(index)));
	};

	out.display_value = amp::display;

	out.range.min.default_value = 0.0f;
	out.range.min.display_value = amp::display;

	out.range.max.default_value = 1.0f;
	out.range.max.display_value = amp::display;
	out.range.max.from_string = amp::from_string;
	out.range.max.constrain = amp::constrain;
	out.range.max.increment = amp::increment;
	out.range.max.decrement = amp::decrement;
	out.range.max.drag = amp::drag;

	out.default_value = 1.0f;
	out.flags = blkhdgen_EnvelopeFlags_DefaultEnabled | blkhdgen_EnvelopeFlags_SnapToDefaultOnly;

	return out;
}

inline EnvelopeSpec pan()
{
	EnvelopeSpec out;

	out.uuid = "9c312a2c-a1b4-4a8d-ab68-07ea157c4574";
	out.name = "Pan";

	out.display_value = pan::display;

	out.range.min.default_value = -1.0f;
	out.range.min.display_value = pan::display;

	out.range.max.default_value = 1.0f;
	out.range.max.display_value = pan::display;

	out.default_value = 0.0f;
	out.flags = blkhdgen_EnvelopeFlags_DefaultEnabled | blkhdgen_EnvelopeFlags_SnapToDefaultOnly;

	return out;
}

inline EnvelopeSpec pitch()
{
	EnvelopeSpec out;

	out.uuid = "ca2529db-e7bd-4019-9a07-22aee24526d1";
	out.name = "Pitch";

	out.display_value = [](float v)
	{
		std::stringstream ss;

		ss << v;

		return ss.str();
	};

	out.get_grid_line = [](int index) -> float
	{
		return float(index * 12);
	};

	out.range.min.constrain = pitch::constrain;
	out.range.min.decrement = pitch::decrement;
	out.range.min.increment = pitch::increment;
	out.range.min.default_value = -24.0f;
	out.range.min.display_value = display_number;
	out.range.min.drag = pitch::drag;
	out.range.min.from_string = find_number<float>;

	out.range.max.constrain = pitch::constrain;
	out.range.max.decrement = pitch::decrement;
	out.range.max.increment = pitch::increment;
	out.range.max.default_value = 24.0f;
	out.range.max.display_value = display_number;
	out.range.max.drag = pitch::drag;
	out.range.max.from_string = find_number<float>;

	out.step_size.constrain = [](float v) { return constrain(v, 0.0f, 60.0f); };
	out.step_size.decrement = decrement<1, 10>;
	out.step_size.increment = increment<1, 10>;
	out.step_size.default_value = 1.0f;
	out.step_size.display_value = display_number;
	out.step_size.drag = pitch::drag;
	out.step_size.from_string = find_number<float>;

	out.default_value = 0.0f;
	out.default_snap_amount = 1.0f;

	return out;
}

inline EnvelopeSpec speed()
{
	EnvelopeSpec out;

	out.uuid = "02f68738-f54a-4f35-947b-c30e73896aa4";
	out.name = "Speed";

	out.display_value = speed::display;

	out.get_grid_line = [](int index) -> float
	{
		return math::linear2speed(float(index));
	};

	out.range.min.constrain = speed::constrain;
	out.range.min.increment = speed::increment;
	out.range.min.decrement = speed::decrement;
	out.range.min.display_value = speed::display;
	out.range.min.drag = speed::drag;
	out.range.min.from_string = speed::from_string;
	out.range.min.default_value = speed::FREEZE;

	out.range.min.constrain = speed::constrain;
	out.range.min.increment = speed::increment;
	out.range.min.decrement = speed::decrement;
	out.range.min.display_value = speed::display;
	out.range.min.drag = speed::drag;
	out.range.min.from_string = speed::from_string;
	out.range.min.default_value = 2.0f;

	out.default_value = speed::NORMAL;

	return out;
}

inline EnvelopeSpec formant()
{
	EnvelopeSpec out;

	out.uuid = "7b72dbef-e36d-4dce-958b-b0fa498ae41e";
	out.name = "Formant";

	// TODO:

	//out.display_value = [](float v)
	//{
	//	std::stringstream ss;

	//	ss << v;

	//	return ss.str();
	//};

	//out.range.min_range.range.min = -60.0f;
	//out.range.min_range.range.max = 0.0f;
	//out.range.min_range.value = -24.0f;
	//out.range.min_range.step_size = 1.0f;

	//out.range.max_range.range.min = -60.0f;
	//out.range.max_range.range.max = 60.0f;
	//out.range.max_range.value = 24.0f;
	//out.range.max_range.step_size = 1.0;

	//out.step_size.range.min = 0.0f;
	//out.step_size.range.max = 60.0f;
	//out.step_size.value = 1.0f;
	//out.step_size.step_size = 1.0f;

	//out.default_value = 0.0f;
	//out.default_snap_amount = 1.0f;

	return out;
}

inline EnvelopeSpec noise_amount()
{
	EnvelopeSpec out;

	out.uuid = "29d5ecb5-cb5d-4f19-afd3-835dd805682a";
	out.name = "Noise Amount";

	// TODO:

	//out.display_value = [](float v)
	//{
	//	std::stringstream ss;

	//	ss << v;

	//	return ss.str();
	//};

	//out.range.min_range.range.min = 0.0f;
	//out.range.min_range.range.max = 0.0f;
	//out.range.min_range.value = 0.0f;
	//out.range.min_range.step_size = 0.0f;

	//out.range.max_range.range.min = 1.0f;
	//out.range.max_range.range.max = 1.0f;
	//out.range.max_range.value = 1.0f;
	//out.range.max_range.step_size = 0.0;

	//out.step_size.range.min = 0.0f;
	//out.step_size.range.max = 0.0f;
	//out.step_size.value = 0.0f;
	//out.step_size.step_size = 0.0f;

	//out.default_value = 0.0f;
	//out.default_snap_amount = 0.0f;

	return out;
}

inline EnvelopeSpec noise_color()
{
	EnvelopeSpec out;

	out.uuid = "30100123-7343-4386-9ed2-f913b9e1e571";
	out.name = "Noise Color";

	// TODO:

	//out.step_size.range.min = 1.0f;
	//out.step_size.range.max = 1.0f;
	//out.step_size.value = 1.0f;
	//out.step_size.step_size = 0.0f;

	//out.default_value = 0.0f;
	//out.default_snap_amount = 0.0f;

	return out;
}

} // envelopes

namespace sliders
{

inline SliderParameterSpec<float> amp()
{
	SliderParameterSpec<float> out;

	out.uuid = BLKHDGEN_STD_UUID_SLIDER_AMP;
	out.name = "Amp";

	out.slider.constrain = amp::constrain;
	out.slider.increment = amp::increment;
	out.slider.decrement = amp::decrement;
	out.slider.drag = amp::drag;
	out.slider.from_string = amp::from_string;
	out.slider.display_value = amp::display;
	out.slider.default_value = 1.0f;
	out.icon = blkhdgen_StdIcon_Amp;

	return out;
}

inline SliderParameterSpec<float> pan()
{
	SliderParameterSpec<float> out;

	out.uuid = "b5bf03f3-17e2-4546-8cc2-e29790ea02a2";
	out.name = "Pan";

	out.slider.constrain = pan::constrain;
	out.slider.increment  = pan::increment;
	out.slider.decrement = pan::decrement;
	out.slider.drag = pan::drag;
	out.slider.from_string = pan::from_string;
	out.slider.display_value = pan::display;
	out.slider.default_value = 0.0f;
	out.icon = blkhdgen_StdIcon_Pan;

	return out;
}

inline SliderParameterSpec<float> pitch()
{
	SliderParameterSpec<float> out;

	out.uuid = BLKHDGEN_STD_UUID_SLIDER_PITCH;
	out.name = "Pitch";

	out.slider.constrain = pitch::constrain;
	out.slider.increment = pitch::increment;
	out.slider.decrement = pitch::decrement;
	out.slider.drag = pitch::drag;
	out.slider.display_value = display_number;
	out.slider.from_string = find_number<float>;
	out.slider.default_value = 0.0f;
	out.icon = blkhdgen_StdIcon_Pitch;

	return out;
}

inline SliderParameterSpec<float> speed()
{
	SliderParameterSpec<float> out;

	out.uuid = "04293c38-3a64-42b2-80f0-43a4f8190ba7";
	out.name = "Speed";

	// TODO: finish implementing

	out.slider.display_value = speed::display;
	out.slider.from_string = find_number<float>;
	out.slider.default_value = 1.0f;

	return out;
}

inline SliderParameterSpec<int> sample_offset()
{
	SliderParameterSpec<int> out;

	out.uuid = BLKHDGEN_STD_UUID_SLIDER_SAMPLE_OFFSET;
	out.name = "Sample Offset";

	// TODO: cleanup

	out.slider.constrain = [](int v)
	{
		return v;
	};

	out.slider.increment = [](int v, bool precise)
	{
		return v + 1;
	};

	out.slider.decrement = [](int v, bool precise)
	{
		return v - 1;
	};

	out.slider.drag = [](int v, int amount, bool precise)
	{
		return v + (amount / (precise ? 50 : 1));
	};

	out.slider.display_value = [](int v)
	{
		std::stringstream ss;

		ss << v;

		return ss.str();
	};

	out.slider.from_string = find_number<int>;
	out.slider.default_value = 0;
	out.icon = blkhdgen_StdIcon_SampleOffset;

	return out;
}

} // sliders

namespace toggles {

inline ToggleSpec loop()
{
	ToggleSpec out;

	out.uuid = BLKHDGEN_STD_UUID_TOGGLE_LOOP;
	out.name = "Loop";
	out.flags = blkhdgen_ToggleFlags_ShowInContextMenu | blkhdgen_ToggleFlags_ShowButton;
	out.default_value = false;

	return out;
}

inline ToggleSpec reverse()
{
	ToggleSpec out;

	out.uuid = BLKHDGEN_STD_UUID_TOGGLE_REVERSE;
	out.name = "Reverse";
	out.flags = blkhdgen_ToggleFlags_ShowInContextMenu | blkhdgen_ToggleFlags_ShowButton;
	out.default_value = false;

	return out;
}

} // toggles

}}