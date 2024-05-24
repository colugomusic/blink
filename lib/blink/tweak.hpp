#pragma once

#include <blink.h>
#include <math.hpp>
#include <cmath>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>

namespace blink {
namespace tweak {

template <class T> [[nodiscard]] auto find_number(std::string str) -> std::optional<T>;
template <class T> [[nodiscard]] auto find_positive_number(std::string str) -> std::optional<T>;

template <> [[nodiscard]] inline
auto find_number<float>(std::string str) -> std::optional<float> {
	std::regex r("(\\-?\\s*[\\.\\d]+)");
	std::smatch match; 
	if (!std::regex_search(str, match, r)) {
		return std::nullopt;
	}
	try {
		return std::stof(match[0].str());
	}
	catch (const std::exception&) {
		return std::nullopt;
	}
}

template <> [[nodiscard]] inline
auto find_number<int64_t>(std::string str) -> std::optional<int64_t> {
	std::regex r("(\\-?\\s*[\\d]+)");
	std::smatch match; 
	if (!std::regex_search(str, match, r)) {
		return std::nullopt;
	}
	try {
		return std::stoi(match[0].str());
	}
	catch (const std::exception&) {
		return std::nullopt;
	}
}

template <> [[nodiscard]] inline
auto find_positive_number<float>(std::string str) -> std::optional<float> {
	std::regex r("([\\.\\d]+)");
	std::smatch match; 
	if (!std::regex_search(str, match, r)) {
		return std::nullopt;
	}
	try {
		return std::stof(match[0].str());
	}
	catch (const std::exception&) {
		return std::nullopt;
	}
}

template <> [[nodiscard]] inline
auto find_positive_number<int64_t>(std::string str) -> std::optional<int64_t> {
	std::regex r("([\\d]+)");
	std::smatch match; 
	if (!std::regex_search(str, match, r)) {
		return std::nullopt;
	}
	try {
		return std::stoi(match[0].str());
	}
	catch (const std::exception&) {
		return std::nullopt;
	}
}

[[nodiscard]] inline
auto from_string(const char* str, int64_t* out) -> blink_Bool {
	auto value = tweak::find_number<int64_t>(str);
	if (!value) {
		return {false};
	}
	*out = *value;
	return {true};
}

template <int Normal, int Precise> [[nodiscard]] constexpr
auto increment(float v, bool precise) -> float {
	return v + 1.0f / (precise ? Precise : Normal);
}

template <int Normal, int Precise> [[nodiscard]] constexpr
auto decrement(float v, bool precise) -> float {
	return v - 1.0f / (precise ? Precise : Normal);
}

template <int Normal, int Precise> [[nodiscard]] constexpr
auto increment(int64_t v, bool precise) -> int64_t {
	return v + (precise ? Precise : Normal);
}

template <int Normal, int Precise> [[nodiscard]] constexpr
auto decrement(int64_t v, bool precise) -> int64_t {
	return v - (precise ? Precise : Normal);
}

template <int Normal> [[nodiscard]] constexpr
auto increment(float v) -> float {
	return v + 1.0f / (Normal);
}

template <int Normal> [[nodiscard]] constexpr
auto decrement(float v) -> float {
	return v - 1.0f / (Normal);
}

template <int Normal> [[nodiscard]] constexpr
auto increment(int64_t v) -> int64_t {
	return v + Normal;
}

template <int Normal> [[nodiscard]] constexpr
auto decrement(int64_t v) -> int64_t {
	return v - Normal;
}

template <class T, int Normal, int Precise> [[nodiscard]] constexpr
auto drag(T v, int amount, bool precise) -> T {
	return v + T(float(amount) / (precise ? Precise : Normal));
}

template <class T, int Normal> [[nodiscard]] constexpr
auto drag(T v, int amount) -> T {
	return v + T(float(amount) / Normal);
}

[[nodiscard]] inline
auto write_string(std::string_view str, char buffer[BLINK_STRING_MAX]) -> void {
	std::strncpy(buffer, str.data(), BLINK_STRING_MAX - 1);
	buffer[BLINK_STRING_MAX - 1] = '\0';
}

template <class T> [[nodiscard]] constexpr
auto stepify(T value, T step) -> T {
	if (step != 0) {
		value = std::floor(value / step + T(0.5)) * step;
	}
	return value;
}

template <int N, class T> [[nodiscard]] constexpr
auto stepify(T v) -> T {
	return stepify(v, T(1) / N);
}

inline
auto to_string(int64_t v, char buffer[BLINK_STRING_MAX]) -> void {
	std::stringstream ss;
	ss << v;
	write_string(ss.str(), buffer);
}

inline
auto to_string(float v, char buffer[BLINK_STRING_MAX]) -> void {
	std::stringstream ss;
	ss << v;
	write_string(ss.str(), buffer);
}

namespace amp {

static constexpr auto SILENT = 0.0f;

[[nodiscard]] inline
auto stepify(float v) -> float {
	static constexpr auto MIN_THRESHOLD = 0.00001f;
	if (v <= MIN_THRESHOLD) {
		return SILENT;
	}
	return math::convert::db_to_linear(blink::tweak::stepify(math::convert::linear_to_db(v), 0.1f));
}

[[nodiscard]] inline
auto constrain(float v) -> float {
	const auto db = math::convert::linear_to_db(v);
	if (db < -60.0f) return SILENT;
	if (db > 12.0f) return math::convert::db_to_linear(12.0f);
	return v;
}

[[nodiscard]] inline
auto from_string(const char* str, float* out) -> blink_Bool {
	auto db = tweak::find_number<float>(str);
	if (!db) {
		return {false};
	}
	*out = math::convert::db_to_linear(*db);
	return {true};
}

[[nodiscard]] inline
auto db_to_string(float db) -> std::string {
	std::stringstream ss;
	ss << db << " dB";
	return ss.str();
}

inline
auto to_string(float v, char buffer[BLINK_STRING_MAX]) -> void {
	std::stringstream ss;
	if (v <= SILENT) {
		ss << "Silent";
	}
	else {
		ss << db_to_string(tweak::stepify(math::convert::linear_to_db(v), 0.1f));
	}
	write_string(ss.str(), buffer);
}

[[nodiscard]] inline
auto increment(float v, bool precise) -> float {
	if (v <= SILENT) {
		return math::convert::db_to_linear(-60.0f);
	}
	return math::convert::db_to_linear(tweak::increment<1, 10>(math::convert::linear_to_db(v), precise));
};

[[nodiscard]] inline
auto decrement(float v, bool precise) -> float {
	if (v <= SILENT) {
		return v;
	}
	return math::convert::db_to_linear(tweak::decrement<1, 10>(math::convert::linear_to_db(v), precise));
};

[[nodiscard]] inline
auto drag(float v, int amount, bool precise) -> float {
	if (v <= SILENT) {
		v = math::convert::db_to_linear(-61.0f);
	}
	return math::convert::db_to_linear(tweak::drag<float, 1, 10>(math::convert::linear_to_db(v), amount / 5, precise));
};

[[nodiscard]] inline
auto tweaker() -> blink_TweakerReal {
	blink_TweakerReal out;
	out.constrain   = tweak::amp::constrain;
	out.decrement   = tweak::amp::decrement;
	out.drag        = tweak::amp::drag;
	out.from_string = tweak::amp::from_string;
	out.increment   = tweak::amp::increment;
	out.stepify     = tweak::amp::stepify;
	out.to_string   = tweak::amp::to_string;
	return out;
}

} // amp

namespace percentage {

[[nodiscard]] inline
auto constrain(float v) -> float {
	return std::clamp(v, 0.0f, 1.0f);
}

[[nodiscard]] inline
auto stepify(float v) -> float {
	return tweak::stepify<1000>(v);
}

[[nodiscard]] inline
auto to_string(float v, char buffer[BLINK_STRING_MAX]) -> void {
	std::stringstream ss;
	ss << stepify(v * 100.0f) << "%";
	write_string(ss.str(), buffer);
}

[[nodiscard]] inline
auto increment(float v, bool precise) -> float {
	return tweak::increment<100, 1000>(v, precise);
};

[[nodiscard]] inline
auto decrement(float v, bool precise) -> float {
	return tweak::decrement<100, 1000>(v, precise);
};

[[nodiscard]] inline
auto drag(float v, int amount, bool precise) -> float {
	return tweak::drag<float, 100, 1000>(v, amount / 5, precise);
};

[[nodiscard]] inline
auto from_string(const char* str, float* out) -> blink_Bool {
	auto value = tweak::find_number<float>(str);
	if (!value) return {false};
	*out = (*value / 100.0f);
	return {true};
};

template <int MIN = 0, int MAX = 100> [[nodiscard]]
auto tweaker() -> blink_TweakerReal {
	blink_TweakerReal out;
	out.constrain   = [](float v) { return std::clamp(v, float(MIN) / 100.0f, float(MAX) / 100.0f); };
	out.decrement   = tweak::percentage::decrement;
	out.drag        = tweak::percentage::drag;
	out.from_string = tweak::percentage::from_string;
	out.increment   = tweak::percentage::increment;
	out.stepify     = tweak::percentage::stepify;
	out.to_string   = tweak::percentage::to_string;
	return out;
}
namespace bipolar {

[[nodiscard]] inline
auto constrain(float v) -> float {
	return std::clamp(v, -1.0f, 1.0f);
};

template <int MIN = 0, int MAX = 100> [[nodiscard]]
auto tweaker() -> blink_TweakerReal {
	blink_TweakerReal out;
	out.constrain   = tweak::percentage::bipolar::constrain;
	out.increment   = tweak::percentage::increment;
	out.decrement   = tweak::percentage::decrement;
	out.drag        = tweak::percentage::drag;
	out.to_string   = tweak::percentage::to_string;
	out.from_string = tweak::percentage::from_string;
	return out;
}

} // bipolar
} // namespace percentage

namespace sample_offset {

[[nodiscard]] inline
auto decrement(int64_t v, bool precise) -> int64_t {
	return v - 1;
}

[[nodiscard]] inline
auto drag(int64_t v, int amount, bool precise) -> int64_t {
	return v + (amount / (precise ? 50 : 1));
}

[[nodiscard]] inline
auto increment(int64_t v, bool precise) -> int64_t {
	return v + 1;
}

[[nodiscard]] inline
auto tweaker() -> blink_TweakerInt {
	blink_TweakerInt out;
	out.constrain   = nullptr;
	out.increment   = tweak::sample_offset::increment;
	out.decrement   = tweak::sample_offset::decrement;
	out.drag        = tweak::sample_offset::drag;
	out.to_string   = blink::tweak::to_string;
	out.from_string = blink::tweak::from_string;
	return out;
}

} // sample_offset

/*
[[nodiscard]] inline
auto from_string(const Host& host, blink_SliderIntIdx sld_idx, std::string_view str, int64_t* out) -> blink_Bool {
	if (const auto value = tweak::from_string(get_tweaker(plugin, sld_idx), str.data())) {
		*out = value.value();
		return {true};
	}
	return {false};
}

[[nodiscard]] inline
auto to_string(const Host& host, blink_SliderIntIdx sld_idx, int64_t value) -> blink_TempString {
	static auto str = tweak::to_string(get_tweaker(plugin, sld_idx), value);
	return {str.c_str()};
}

[[nodiscard]] inline 
auto constrain(const Host& host, blink_SliderIntIdx sld_idx, int64_t value) -> int64_t {
	return tweak::constrain(get_tweaker(plugin, sld_idx), value);
}

[[nodiscard]] inline 
auto decrement(const Host& host, blink_SliderIntIdx sld_idx, int64_t value, blink_Bool precise) -> int64_t {
	return tweak::decrement(get_tweaker(plugin, sld_idx), value, precise.value > 0);
}

[[nodiscard]] inline 
auto drag(const Host& host, blink_SliderIntIdx sld_idx, int64_t start_value, int64_t amount, blink_Bool precise) -> int64_t {
	return tweak::drag(get_tweaker(plugin, sld_idx), start_value, amount, precise.value > 0);
}

[[nodiscard]] inline 
auto get_default_value(const Host& host, blink_SliderIntIdx sld_idx) -> int64_t {
	return host.slider_int.get<DefaultValue<int64_t>>(sld_idx.value).value;
}

[[nodiscard]] inline 
auto increment(const Host& host, blink_SliderIntIdx sld_idx, int64_t value, blink_Bool precise) -> int64_t {
	return tweak::increment(get_tweaker(plugin, sld_idx), value, precise.value > 0);
}
*/

} // namespace tweak
} // namespace blink