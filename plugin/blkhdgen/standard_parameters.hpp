#pragma once

#include <optional>
#include <regex>
#include <sstream>
#include "envelope_spec.hpp"
#include "slider_spec.hpp"

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

inline std::string amp_display(float v)
{
	std::stringstream ss;

	ss << math::stepify(float(math::linear2db(v)), 0.01f) << " dB";

	return ss.str();
}

inline std::string pan_display(float v)
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

inline float amp_constrain(float v)
{
	const auto db = math::linear2db(v);

	if (db < -60.0f) return 0.0f;
	if (db > 12.0f) return math::db2linear(12.0f);

	return v;
};

inline float pan_constrain(float v)
{
	if (v < -1.0f) return -1.0f;
	if (v > 1.0f) return 1.0f;

	return v;
};

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

inline std::string speed_display(float v)
{
	std::stringstream ss;

	if (v == 0.0f)
	{
		ss << "Freeze";
	}
	else if (v == 0.25f)
	{
		ss << v << "Quarter";
	}
	else if (v == 0.5f)
	{
		ss << v << "Half";
	}
	else if (v == 1.0f)
	{
		ss << v << "Normal";
	}
	else if (v == 2.0f)
	{
		ss << v << "Double";
	}
	else
	{
		ss << v;
	}

	return ss.str();
}

namespace envelopes {

inline EnvelopeSpec amp()
{
	EnvelopeSpec out;

	out.uuid = "273e7c30-404b-4db6-ba97-20f33d49fe51";
	out.name = "Amp";

	out.curve = [](float v)
	{
		return math::db2linear(v);
	};

	out.inverse_curve = [](float v)
	{
		return math::linear2db(v);
	};

	out.display_value = amp_display;

	out.range.min_range.range.min = -60.0f;
	out.range.min_range.range.max = -60.0f;
	out.range.min_range.value = -60.0f;
	out.range.min_range.step_size = 0.0f;

	out.range.max_range.range.min = -60.0f;
	out.range.max_range.range.max = 12.0f;
	out.range.max_range.value = 0.0f;
	out.range.max_range.step_size = 0.0;

	out.step_size.range.min = 0.0f;
	out.step_size.range.max = 0.0f;
	out.step_size.value = 0.0f;
	out.step_size.step_size = 0.0f;

	out.default_value = 0.0f;
	out.default_snap_amount = 0.0f;

	return out;
}

inline EnvelopeSpec pan()
{
	EnvelopeSpec out;

	out.uuid = "9c312a2c-a1b4-4a8d-ab68-07ea157c4574";
	out.name = "Pan";

	out.curve = [](float v)
	{
		return v;
	};

	out.inverse_curve = [](float v)
	{
		return v;
	};

	out.display_value = pan_display;

	out.range.min_range.range.min = -1.0f;
	out.range.min_range.range.max = -1.0f;
	out.range.min_range.value = -1.0f;
	out.range.min_range.step_size = 0.0f;

	out.range.max_range.range.min = -1.0f;
	out.range.max_range.range.max = 1.0f;
	out.range.max_range.value = 1.0f;
	out.range.max_range.step_size = 0.0;

	out.step_size.range.min = 1.0f;
	out.step_size.range.max = 1.0f;
	out.step_size.value = 1.0f;
	out.step_size.step_size = 0.0f;

	out.default_value = 0.0f;
	out.default_snap_amount = 0.0f;

	return out;
}

inline EnvelopeSpec pitch()
{
	EnvelopeSpec out;

	out.uuid = "ca2529db-e7bd-4019-9a07-22aee24526d1";
	out.name = "Pitch";

	out.curve = [](float v)
	{
		return v;
	};

	out.inverse_curve = [](float v)
	{
		return v;
	};

	out.display_value = [](float v)
	{
		std::stringstream ss;

		ss << v;

		return ss.str();
	};

	out.range.min_range.range.min = -60.0f;
	out.range.min_range.range.max = 0.0f;
	out.range.min_range.value = -24.0f;
	out.range.min_range.step_size = 1.0f;

	out.range.max_range.range.min = -60.0f;
	out.range.max_range.range.max = 60.0f;
	out.range.max_range.value = 24.0f;
	out.range.max_range.step_size = 1.0;

	out.step_size.range.min = 0.0f;
	out.step_size.range.max = 60.0f;
	out.step_size.value = 1.0f;
	out.step_size.step_size = 1.0f;

	out.default_value = 0.0f;
	out.default_snap_amount = 1.0f;

	return out;
}

inline EnvelopeSpec speed()
{
	EnvelopeSpec out;

	out.uuid = "02f68738-f54a-4f35-947b-c30e73896aa4";
	out.name = "Speed";

	out.curve = [](float v)
	{
		return std::pow(v, 2.0f);
	};

	out.inverse_curve = [](float v)
	{
		return std::sqrt(v);
	};

	out.display_value = speed_display;

	out.range.min_range.range.min = 0.0f;
	out.range.min_range.range.max = 1.0f;
	out.range.min_range.value = 0.0f;
	out.range.min_range.step_size = 0.0f;

	out.range.max_range.range.min = 0.5f;
	out.range.max_range.range.max = 32.0f;
	out.range.max_range.value = 2.0f;
	out.range.max_range.step_size = 0.0f;

	out.step_size.range.min = 0.0f;
	out.step_size.range.max = 32.0f;
	out.step_size.value = 1.0f;
	out.step_size.step_size = 0.25f;

	out.default_value = 1.0f;
	out.default_snap_amount = 0.0f;

	return out;
}

inline EnvelopeSpec formant()
{
	EnvelopeSpec out;

	out.uuid = "7b72dbef-e36d-4dce-958b-b0fa498ae41e";
	out.name = "Formant";

	out.curve = [](float v)
	{
		return v;
	};

	out.inverse_curve = [](float v)
	{
		return v;
	};

	out.display_value = [](float v)
	{
		std::stringstream ss;

		ss << v;

		return ss.str();
	};

	out.range.min_range.range.min = -60.0f;
	out.range.min_range.range.max = 0.0f;
	out.range.min_range.value = -24.0f;
	out.range.min_range.step_size = 1.0f;

	out.range.max_range.range.min = -60.0f;
	out.range.max_range.range.max = 60.0f;
	out.range.max_range.value = 24.0f;
	out.range.max_range.step_size = 1.0;

	out.step_size.range.min = 0.0f;
	out.step_size.range.max = 60.0f;
	out.step_size.value = 1.0f;
	out.step_size.step_size = 1.0f;

	out.default_value = 0.0f;
	out.default_snap_amount = 1.0f;

	return out;
}

inline EnvelopeSpec noise_amount()
{
	EnvelopeSpec out;

	out.uuid = "29d5ecb5-cb5d-4f19-afd3-835dd805682a";
	out.name = "Noise Amount";

	out.curve = [](float v)
	{
		return v;
	};

	out.inverse_curve = [](float v)
	{
		return v;
	};

	out.display_value = [](float v)
	{
		std::stringstream ss;

		ss << v;

		return ss.str();
	};

	out.range.min_range.range.min = 0.0f;
	out.range.min_range.range.max = 0.0f;
	out.range.min_range.value = 0.0f;
	out.range.min_range.step_size = 0.0f;

	out.range.max_range.range.min = 1.0f;
	out.range.max_range.range.max = 1.0f;
	out.range.max_range.value = 1.0f;
	out.range.max_range.step_size = 0.0;

	out.step_size.range.min = 0.0f;
	out.step_size.range.max = 0.0f;
	out.step_size.value = 0.0f;
	out.step_size.step_size = 0.0f;

	out.default_value = 0.0f;
	out.default_snap_amount = 0.0f;

	return out;
}

inline EnvelopeSpec noise_color()
{
	EnvelopeSpec out;

	out.uuid = "30100123-7343-4386-9ed2-f913b9e1e571";
	out.name = "Noise Color";

	out.curve = [](float v)
	{
		return v;
	};

	out.inverse_curve = [](float v)
	{
		return v;
	};

	out.display_value = [](float v)
	{
		std::stringstream ss;

		ss << v;

		return ss.str();
	};

	out.range.min_range.range.min = -1.0f;
	out.range.min_range.range.max = -1.0f;
	out.range.min_range.value = -1.0f;
	out.range.min_range.step_size = 0.0f;

	out.range.max_range.range.min = 1.0f;
	out.range.max_range.range.max = 1.0f;
	out.range.max_range.value = 1.0f;
	out.range.max_range.step_size = 0.0;

	out.step_size.range.min = 1.0f;
	out.step_size.range.max = 1.0f;
	out.step_size.value = 1.0f;
	out.step_size.step_size = 0.0f;

	out.default_value = 0.0f;
	out.default_snap_amount = 0.0f;

	return out;
}

} // envelopes

namespace sliders
{

inline SliderSpec<float> amp()
{
	SliderSpec<float> out;

	out.uuid = BLKHDGEN_STD_UUID_SLIDER_AMP;
	out.name = "Amp";

	out.constrain = amp_constrain;

	out.increment = [](float v, bool precise)
	{
		if (v <= 0.0f) return math::db2linear(-60.0f);

		return amp_constrain(math::db2linear(increment<1, 10>(math::linear2db(v), precise)));
	};

	out.decrement = [](float v, bool precise)
	{
		return amp_constrain(math::db2linear(decrement<1, 10>(math::linear2db(v), precise)));
	};

	out.drag = [](float v, int amount, bool precise)
	{
		if (v <= 0.0f) v = math::db2linear(-61.0f);

		return amp_constrain(math::db2linear(math::stepify(drag<1, 10>(math::linear2db(v), amount / 5, precise), 0.01f)));
	};

	out.from_string = [](const std::string& str) -> std::optional<float>
	{
		auto db = find_number<float>(str);

		if (!db) return db;

		return math::db2linear(*db);
	};

	out.display_value = amp_display;
	out.default_value = 1.0f;

	return out;
}

inline SliderSpec<float> pan()
{
	SliderSpec<float> out;

	out.uuid = "b5bf03f3-17e2-4546-8cc2-e29790ea02a2";
	out.name = "Pan";

	out.constrain = pan_constrain;

	out.increment = [](float v, bool precise)
	{
		return pan_constrain(math::stepify(increment<100>(v), 0.01f));
	};
	
	out.decrement = [](float v, bool precise)
	{
		return pan_constrain(math::stepify(decrement<100>(v), 0.01f));
	};

	out.drag = [](float v, int amount, bool precise)
	{
		return pan_constrain(math::stepify(drag<500, 5000>(v, amount, precise), 0.01f));
	};

	out.from_string = [](const std::string& str)->std::optional<float>
	{
		std::string uppercase = str;

		std::transform(str.begin(), str.end(), uppercase.begin(), ::toupper);

		if (uppercase.find("CENTER") != std::string::npos) return 0.0f;

		const auto negative = uppercase.find('L') != std::string::npos || uppercase.find('-') != std::string::npos;
		
		auto value = find_positive_number<int>(str);

		if (!value) return std::optional<float>();

		return (float(*value) / 100) * (negative ? -1 : 1);
	};

	out.display_value = pan_display;
	out.default_value = 0.0f;

	return out;
}

inline SliderSpec<float> pitch()
{
	SliderSpec<float> out;

	out.uuid = "00859eeb-ce9e-43cd-9994-bff881a9d32d";
	out.name = "Pitch";

	out.constrain = [](float v)
	{
		if (v < -60.0f) return -60.0f;
		if (v > 60.0f) return 60.0f;

		return v;
	};

	out.increment = [out](float v, bool precise)
	{
		return out.constrain(math::stepify(increment<1, 10>(v, precise), 0.1f));
	};

	out.decrement = [out](float v, bool precise)
	{
		return out.constrain(math::stepify(decrement<1, 10>(v, precise), 0.1f));
	};

	out.drag = [out](float v, int amount, bool precise)
	{
		return out.constrain(math::stepify(drag<1, 10>(v, amount / 5, precise), 0.1f));
	};

	out.display_value = [](float v)
	{
		std::stringstream ss;

		ss << v;

		return ss.str();
	};

	out.from_string = find_number<float>;
	out.default_value = 0.0f;

	return out;
}

inline SliderSpec<float> speed()
{
	SliderSpec<float> out;

	out.uuid = "04293c38-3a64-42b2-80f0-43a4f8190ba7";
	out.name = "Speed";


	out.display_value = [](float v)
	{
		std::stringstream ss;

		ss << v;

		return ss.str();
	};

	out.from_string = find_number<float>;
	out.default_value = 1.0f;

	return out;
}

inline SliderSpec<int> sample_offset()
{
	SliderSpec<int> out;

	out.uuid = "88373752-7656-4d0e-8da2-a18c05af0106";
	out.name = "Sample Offset";

	out.constrain = [](int v)
	{
		return v;
	};

	out.increment = [](int v, bool precise)
	{
		return v + 1;
	};

	out.decrement = [](int v, bool precise)
	{
		return v - 1;
	};

	out.drag = [](int v, int amount, bool precise)
	{
		return v + (amount / (precise ? 50 : 5));
	};

	out.display_value = [](int v)
	{
		std::stringstream ss;

		ss << v;

		return ss.str();
	};

	out.from_string = find_number<int>;
	out.default_value = 0;

	return out;
}

} // sliders

namespace toggles {

inline ToggleSpec loop()
{
	ToggleSpec out;

	out.uuid = "dfa36d24-3c41-4a13-9b57-dc0116ef19f7";
	out.name = "Loop";
	out.flags = blkhdgen_ToggleFlags_ShowInContextMenu;
	out.default_value = false;

	return out;
}

inline ToggleSpec reverse()
{
	ToggleSpec out;

	out.uuid = "e7cacaf8-4afc-4e81-83de-50620fed4b13";
	out.name = "Reverse";
	out.flags = blkhdgen_ToggleFlags_ShowInContextMenu;
	out.default_value = false;

	return out;
}

} // toggles

}}