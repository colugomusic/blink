#pragma once

#include <algorithm>
#include <blink.h>
#include <cmath>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <tweak/std/amp.hpp>
#include <tweak/std/percentage.hpp>
#include <tweak/std/speed.hpp>
#include <tweak/math.hpp>
#include <tweak/tweak.hpp>
#include "math.hpp"

namespace blink::tweak {

[[nodiscard]] inline
auto positive_number_from_string(const char* str, float* out) -> blink_Bool {
	auto value = ::tweak::find_positive_number<float>(str);
	if (!value) {
		return {false};
	}
	*out = *value;
	return {true};
}

[[nodiscard]] inline
auto from_string(const char* str, int64_t* out) -> blink_Bool {
	auto value = ::tweak::find_number<int64_t>(str);
	if (!value) {
		return {false};
	}
	*out = *value;
	return {true};
}

inline
auto write_string(std::string_view str, char buffer[BLINK_STRING_MAX]) -> void {
	const auto n = std::min(size_t(BLINK_STRING_MAX - 1), str.size());
	str.copy(buffer, n);
	buffer[n] = '\0';
}

template <class T> [[nodiscard]] constexpr
auto identity(T v) -> T {
	return v;
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
	return ::tweak::std_::amp::stepify(v);
}

[[nodiscard]] inline
auto constrain(float v) -> float {
	return ::tweak::std_::amp::constrain(v);
}

[[nodiscard]] inline
auto from_string(const char* str, float* out) -> blink_Bool {
	if (const auto v = ::tweak::std_::amp::from_string(str)) {
		*out = *v;
		return {true};
	}
	return {false};
}

[[nodiscard]] inline
auto db_to_string(float db) -> std::string {
	return ::tweak::std_::amp::db_to_string(db);
}

inline
auto to_string(float v, char buffer[BLINK_STRING_MAX]) -> void {
	write_string(::tweak::std_::amp::to_string(v), buffer);
}

[[nodiscard]] inline
auto increment(float v, bool precise) -> float {
	return ::tweak::std_::amp::increment(v, precise);
};

[[nodiscard]] inline
auto decrement(float v, bool precise) -> float {
	return ::tweak::std_::amp::decrement(v, precise);
};

[[nodiscard]] inline
auto drag(float v, int amount, bool precise) -> float {
	return ::tweak::std_::amp::drag(v, amount, precise);
};

[[nodiscard]] inline
auto tweaker() -> blink_TweakerReal {
	blink_TweakerReal out = {0};
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

namespace delay_time {

[[nodiscard]] inline
auto stepify(float v) -> float {
	v = math::convert::linear_to_delay_time_ms(v);
	v = ::tweak::math::stepify<1000>(v);
	v = math::convert::delay_time_ms_to_linear(v);
	return v;
}

inline
auto to_string(float value, char buffer[BLINK_STRING_MAX]) -> void {
	std::stringstream ss;
	ss << math::convert::linear_to_delay_time_ms(value) << " ms";
	write_string(ss.str(), buffer);
}

[[nodiscard]] inline
auto constrain(float v) -> float {
	return std::clamp(v, 0.0f, 1.0f);
}

[[nodiscard]] inline
auto increment(float v, bool precise) -> float {
	v = math::convert::linear_to_delay_time_ms(v);
	v += (precise ? 1.0f : 10.0f);
	v = math::convert::delay_time_ms_to_linear(v);
	return v;
}

[[nodiscard]] inline
auto decrement(float v, bool precise) -> float {
	v = math::convert::linear_to_delay_time_ms(v);
	v -= (precise ? 1.0f : 10.0f);
	v = math::convert::delay_time_ms_to_linear(v);
	return v;
}

[[nodiscard]] inline
auto drag(float v, int amount, bool precise) -> float {
	v = math::convert::linear_to_delay_time_ms(v);
	v = ::tweak::drag<float, 100, 1000>(v, amount / 5, precise);
	v = math::convert::delay_time_ms_to_linear(v);
	return v;
}

[[nodiscard]] inline
auto from_string(const char* str, float* out) -> blink_Bool {
	auto number = ::tweak::find_positive_number<float>(str);
	if (!number) {
		return {false};
	}
	*out = math::convert::delay_time_ms_to_linear(*number);
	return {true};
}

[[nodiscard]] inline
auto tweaker() -> blink_TweakerReal {
	blink_TweakerReal out = {0};
	out.constrain   = tweak::delay_time::constrain;
	out.decrement   = tweak::delay_time::decrement;
	out.drag        = tweak::delay_time::drag;
	out.from_string = tweak::delay_time::from_string;
	out.increment   = tweak::delay_time::increment;
	out.stepify     = tweak::delay_time::stepify;
	out.to_string   = tweak::delay_time::to_string;
	return out;
}

} // delay_time

namespace linear {

template <int Resolution> [[nodiscard]] inline
auto fns() -> blink_EnvFns {
	blink_EnvFns fns = {0};
	fns.stepify   = ::tweak::math::stepify<Resolution>;
	fns.to_string = tweak::to_string;
	return fns;
}

template <int Min, int Max, int Resolution, int EditPrecision> [[nodiscard]] inline
auto tweaker() -> blink_TweakerReal {
	blink_TweakerReal out = {0};
	out.stepify   = ::tweak::math::stepify<Resolution>;
	out.constrain = [](float v) { return std::clamp(v, float(Min) / Resolution, float(Max) / Resolution); };
	out.increment = [](float v, bool precise) { return v + (precise ? 1.0f / EditPrecision : 1.0f / Resolution); };
	out.decrement = [](float v, bool precise) { return v - (precise ? 1.0f / EditPrecision : 1.0f / Resolution); };
	out.drag      = [](float v, int amount, bool precise) { return v + (amount * (precise ? 1.0f / EditPrecision : 1.0f / Resolution)); };
	out.to_string = tweak::to_string;
	out.from_string = [](const char* str, float* out) -> blink_Bool {
		auto number = ::tweak::find_number<float>(str);
		if (!number) {
			return {false};
		}
		*out = *number;
		return {true};
	};
	return out;
}

} // linear

namespace ms {

template <int Resolution> inline
auto to_string(float v, char buffer[BLINK_STRING_MAX]) -> void {
	std::stringstream ss;
	ss << v << " ms";
	tweak::write_string(ss.str(), buffer);
}

template <int Resolution> [[nodiscard]] inline
auto fns() -> blink_EnvFns {
	blink_EnvFns fns = {0};
	fns.stepify   = ::tweak::math::stepify<Resolution>;
	fns.to_string = tweak::ms::to_string<Resolution>;
	return fns;
}

template <int Min, int Max, int Resolution, int EditPrecision> [[nodiscard]] inline
auto tweaker() -> blink_TweakerReal {
	blink_TweakerReal out = linear::tweaker<Min, Max, Resolution, EditPrecision>();
	out.to_string = ms::to_string<Resolution>;
	out.from_string = tweak::positive_number_from_string;
	return out;
}

} // ms

namespace filter_frequency {

static constexpr auto DEFAULT_VALUE = 0.52833f;

[[nodiscard]] inline
auto constrain(float v) -> float {
	return std::clamp(v, 0.0f, 1.0f);
}

[[nodiscard]] inline
auto increment(float v, bool precise) -> float {
	return ::tweak::increment<100, 1000>(v, precise);
}

[[nodiscard]] inline
auto decrement(float v, bool precise) -> float {
	return ::tweak::decrement<100, 1000>(v, precise);
}

[[nodiscard]] inline
auto drag(float v, int amount, bool precise) -> float {
	return ::tweak::drag<float, 100, 1000>(v, amount / 5, precise);
}

[[nodiscard]] inline
auto from_string(const char* str, float* out) -> blink_Bool {
	auto value = ::tweak::find_number<float>(str);
	if (!value) {
		return {false};
	}
	std::string uppercase = str;
	std::string_view strv{str};
	std::transform(strv.begin(), strv.end(), uppercase.begin(), ::toupper);
	if (uppercase.find("MHZ") != std::string::npos) {
		*out = math::convert::filter_hz_to_linear((*value) * 1000.0f);
		return {true};
	}
	*out = math::convert::filter_hz_to_linear(*value);
	return {true};
}

inline
auto to_string(float v, char buffer[BLINK_STRING_MAX]) -> void {
	std::stringstream ss;
	const auto hz = math::convert::linear_to_filter_hz(v);
	if (hz >= 1000.0f) {
		ss << ::tweak::math::stepify(hz / 1000.0f, 0.1f) << " KHz";
	}
	else {
		ss << ::tweak::math::stepify(hz, 0.1f) << " Hz";
	}
	write_string(ss.str(), buffer);
}

[[nodiscard]] inline
auto tweaker() -> blink_TweakerReal {
	blink_TweakerReal out = {0};
	out.constrain   = tweak::filter_frequency::constrain;
	out.decrement   = tweak::filter_frequency::decrement;
	out.drag        = tweak::filter_frequency::drag;
	out.from_string = tweak::filter_frequency::from_string;
	out.increment   = tweak::filter_frequency::increment;
	out.to_string   = tweak::filter_frequency::to_string;
	return out;
}

} // filter_frequency

namespace pan {

[[nodiscard]] inline
auto stepify(float v) -> float {
	return ::tweak::math::stepify<100>(v);
}

[[nodiscard]] inline
auto constrain(float v) -> float {
	return std::clamp(v, -1.0f, 1.0f);
}

inline
auto to_string(float v, char buffer[BLINK_STRING_MAX]) -> void {
	std::stringstream ss;
	if (v < 0.0f) {
		ss << stepify(std::abs(v * 100)) << "% L";
	}
	else if (v > 0.0f) {
		ss << stepify(v * 100) << "% R";
	}
	else {
		ss << "Center";
	}
	write_string(ss.str(), buffer);
}

[[nodiscard]] inline
auto increment(float v, bool precise) -> float {
	return ::tweak::increment<100, 1000>(v, precise);
}

[[nodiscard]] inline
auto decrement(float v, bool precise) -> float {
	return ::tweak::decrement<100, 1000>(v, precise);
}

[[nodiscard]] inline
auto drag(float v, int amount, bool precise) -> float {
	return ::tweak::drag<float, 500, 5000>(v, amount, precise);
}

[[nodiscard]] inline
auto from_string(const char* str, float* out) -> blink_Bool {
	std::string uppercase = str;
	std::string_view strv{str};
	std::transform(strv.begin(), strv.end(), uppercase.begin(), ::toupper);
	if (uppercase.find("CENTER") != std::string::npos) {
		*out = 0.0f;
		return {true};
	}
	const auto negative = uppercase.find('L') != std::string::npos || uppercase.find('-') != std::string::npos;
	const auto value = ::tweak::find_positive_number<int64_t>(str);
	if (!value) return {false};
	*out = (float(*value) / 100) * (negative ? -1 : 1);
	return {true};
}

[[nodiscard]] inline
auto tweaker() -> blink_TweakerReal {
	blink_TweakerReal out = {0};
	out.constrain   = tweak::pan::constrain;
	out.decrement   = tweak::pan::decrement;
	out.drag        = tweak::pan::drag;
	out.from_string = tweak::pan::from_string;
	out.increment   = tweak::pan::increment;
	out.stepify     = tweak::pan::stepify;
	out.to_string   = tweak::pan::to_string;
	return out;
}

} // pan

namespace percentage {

[[nodiscard]] inline
auto constrain(float v) -> float {
	return ::tweak::std_::percentage::constrain(v);
}

[[nodiscard]] inline
auto stepify(float v) -> float {
	return ::tweak::std_::percentage::stepify(v);
}

inline
auto to_string(float v, char buffer[BLINK_STRING_MAX]) -> void {
	write_string(::tweak::std_::percentage::to_string(v), buffer);
}

[[nodiscard]] inline
auto increment(float v, bool precise) -> float {
	return ::tweak::std_::percentage::increment(v, precise);
};

[[nodiscard]] inline
auto decrement(float v, bool precise) -> float {
	return ::tweak::std_::percentage::decrement(v, precise);
};

[[nodiscard]] inline
auto drag(float v, int amount, bool precise) -> float {
	return ::tweak::std_::percentage::drag(v, amount, precise);
};

[[nodiscard]] inline
auto from_string(const char* str, float* out) -> blink_Bool {
	if (const auto v = ::tweak::std_::percentage::from_string(str)) {
		*out = *v;
		return {true};
	}
	return {false};
};

[[nodiscard]] inline
auto fns() -> blink_EnvFns {
	blink_EnvFns fns = {0};
	fns.stepify   = tweak::percentage::stepify;
	fns.to_string = tweak::percentage::to_string;
	return fns;
}

template <int MIN = 0, int MAX = 100> [[nodiscard]]
auto tweaker() -> blink_TweakerReal {
	blink_TweakerReal out = {0};
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
	return ::tweak::std_::percentage::bipolar::constrain(v);
};

template <int MIN = 0, int MAX = 100> [[nodiscard]]
auto tweaker() -> blink_TweakerReal {
	blink_TweakerReal out = {0};
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

namespace pitch {

[[nodiscard]] inline
auto constrain(float v) -> float {
	return std::clamp(v, -60.0f, 60.0f);
};

[[nodiscard]] inline
auto increment(float v, bool precise) -> float {
	return ::tweak::increment<1, 10>(v, precise);
};

[[nodiscard]] inline
auto decrement(float v, bool precise) -> float {
	return ::tweak::decrement<1, 10>(v, precise);
};

[[nodiscard]] inline
auto drag(float v, int amount, bool precise) -> float {
	return ::tweak::drag<float, 1, 10>(v, amount / 5, precise);
};

[[nodiscard]] inline
auto from_string(const char* str, float* out) -> blink_Bool {
	auto value = ::tweak::find_number<float>(str);
	if (!value) return {false};
	*out = *value;
	return {true};
};

[[nodiscard]] inline
auto stepify(float v) -> float {
	return ::tweak::math::stepify<10>(v);
};

[[nodiscard]] inline
auto tweaker() -> blink_TweakerReal {
	blink_TweakerReal out = {0};
	out.constrain   = pitch::constrain;
	out.decrement   = pitch::decrement;
	out.drag        = pitch::drag;
	out.from_string = pitch::from_string;
	out.increment   = pitch::increment;
	out.stepify     = pitch::stepify;
	out.to_string   = tweak::to_string;
	return out;
}

[[nodiscard]] inline
auto step_size_tweaker() -> blink_TweakerReal {
	blink_TweakerReal out = {0};
	out.to_string   = tweak::to_string;
	out.constrain   = [](float v) { return ::tweak::constrain(v, 0.0f, 60.0f); };
	out.decrement   = [](float v, bool precise) { return ::tweak::decrement<1, 10>(v, precise); };
	out.drag        = [](float v, int amount, bool precise) { return tweak::pitch::drag(v, amount, precise); };
	out.increment   = [](float v, bool precise) { return ::tweak::increment<1, 10>(v, precise); };
	out.from_string = [](const char* str, float* out) -> blink_Bool {
		if (const auto number = ::tweak::find_number<float>(str)) {
			*out = *number;
			return {true};
		}
		return {false};
	};
	return out;
}

} // pitch

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
	blink_TweakerInt out = {0};
	out.constrain   = nullptr;
	out.increment   = tweak::sample_offset::increment;
	out.decrement   = tweak::sample_offset::decrement;
	out.drag        = tweak::sample_offset::drag;
	out.to_string   = blink::tweak::to_string;
	out.from_string = blink::tweak::from_string;
	return out;
}

} // sample_offset

namespace speed {

[[nodiscard]] inline
auto constrain(float v) {
	return ::tweak::std_::speed::constrain(v);
}

[[nodiscard]] inline
auto increment(float v, bool precise) {
	return ::tweak::std_::speed::increment(v, precise);
};

[[nodiscard]] inline
auto decrement(float v, bool precise) {
	return ::tweak::std_::speed::decrement(v, precise);
};

[[nodiscard]] inline
auto drag(float v, int amount, bool precise) -> float {
	return ::tweak::std_::speed::drag(v, amount, precise);
};

[[nodiscard]] inline
auto from_string(const char* cstr, float* out) -> blink_Bool {
	if (const auto v = ::tweak::std_::speed::from_string(cstr)) {
		*out = *v;
		return {true};
	}
	return {false};
}

inline
auto to_string(float v, char buffer[BLINK_STRING_MAX]) -> void {
	write_string(::tweak::std_::speed::to_string(v), buffer);
}

[[nodiscard]] inline
auto tweaker() -> blink_TweakerReal {
	blink_TweakerReal out = {0};
	out.constrain   = speed::constrain;
	out.decrement   = speed::decrement;
	out.drag        = speed::drag;
	out.from_string = speed::from_string;
	out.increment   = speed::increment;
	out.to_string   = speed::to_string;
	return out;
}

} // speed

[[nodiscard]] inline
auto raw_decrement(const blink_TweakerInt& tweaker, int64_t value, bool precise) -> int64_t {
	if (tweaker.decrement) {
		return tweaker.decrement(value, precise);
	}
	return value;
}

[[nodiscard]] inline
auto raw_decrement(const blink_TweakerReal& tweaker, float value, bool precise) -> float {
	if (tweaker.decrement) {
		return tweaker.decrement(value, precise);
	}
	return value;
}

[[nodiscard]] inline
auto raw_drag(const blink_TweakerInt& tweaker, int64_t value, int amount, bool precise) -> int64_t {
	if (tweaker.drag) {
		return tweaker.drag(value, amount, precise);
	}
	return value;
}

[[nodiscard]] inline
auto raw_drag(const blink_TweakerReal& tweaker, float value, int amount, bool precise) -> float {
	if (tweaker.drag) {
		return tweaker.drag(value, amount, precise);
	}
	return value;
}

[[nodiscard]] inline
auto raw_increment(const blink_TweakerInt& tweaker, int64_t value, bool precise) -> int64_t {
	if (tweaker.increment) {
		return tweaker.increment(value, precise);
	}
	return value;
}

[[nodiscard]] inline
auto raw_increment(const blink_TweakerReal& tweaker, float value, bool precise) -> float {
	if (tweaker.increment) {
		return tweaker.increment(value, precise);
	}
	return value;
}

[[nodiscard]] inline
auto from_string(const blink_TweakerInt& tweaker, std::string_view str) -> std::optional<int64_t> {
	if (tweaker.from_string) {
		int64_t value;
		if (tweaker.from_string(str.data(), &value).value) {
			return value;
		}
	}
	return std::nullopt;
}

[[nodiscard]] inline
auto from_string(const blink_TweakerReal& tweaker, std::string_view str) -> std::optional<float> {
	if (tweaker.from_string) {
		float value;
		if (tweaker.from_string(str.data(), &value).value) {
			return value;
		}
	}
	return std::nullopt;
}

[[nodiscard]] inline
auto stepify(const blink_TweakerInt& tweaker, int64_t value) -> int64_t {
	if (tweaker.stepify) {
		return tweaker.stepify(value);
	}
	return value;
}

[[nodiscard]] inline
auto stepify(const blink_TweakerReal& tweaker, float value) -> float {
	if (tweaker.stepify) {
		return tweaker.stepify(value);
	}
	return value;
}

[[nodiscard]] inline
auto constrain(const blink_TweakerInt& tweaker, int64_t value) -> int64_t {
	if (tweaker.constrain) {
		return tweaker.constrain(value);
	}
	return value;
}

[[nodiscard]] inline
auto constrain(const blink_TweakerReal& tweaker, float value) -> float {
	if (tweaker.constrain) {
		return tweaker.constrain(value);
	}
	return value;
}

[[nodiscard]] inline
auto decrement(const blink_TweakerInt& tweaker, int64_t value, bool precise) -> int64_t {
	return constrain(tweaker, stepify(tweaker, raw_decrement(tweaker, value, precise)));
}

[[nodiscard]] inline
auto decrement(const blink_TweakerReal& tweaker, float value, bool precise) -> float {
	return constrain(tweaker, stepify(tweaker, raw_decrement(tweaker, value, precise)));
}

[[nodiscard]] inline
auto drag(const blink_TweakerInt& tweaker, int64_t value, int amount, bool precise) -> int64_t {
	return constrain(tweaker, stepify(tweaker, raw_drag(tweaker, value, amount, precise)));
}

[[nodiscard]] inline
auto drag(const blink_TweakerReal& tweaker, float value, int amount, bool precise) -> float {
	return constrain(tweaker, stepify(tweaker, raw_drag(tweaker, value, amount, precise)));
}

[[nodiscard]] inline
auto increment(const blink_TweakerInt& tweaker, int64_t value, bool precise) -> int64_t {
	return constrain(tweaker, stepify(tweaker, raw_increment(tweaker, value, precise)));
}

[[nodiscard]] inline
auto increment(const blink_TweakerReal& tweaker, float value, bool precise) -> float {
	return constrain(tweaker, stepify(tweaker, raw_increment(tweaker, value, precise)));
}

[[nodiscard]] inline
auto to_string(const blink_TweakerInt& tweaker, int64_t value) -> std::string {
	if (tweaker.to_string) {
		char buffer[BLINK_STRING_MAX];
		tweaker.to_string(value, buffer);
		return buffer;
	}
	return {};
}

[[nodiscard]] inline
auto to_string(const blink_TweakerReal& tweaker, float value) -> std::string {
	if (tweaker.to_string) {
		char buffer[BLINK_STRING_MAX];
		tweaker.to_string(value, buffer);
		return buffer;
	}
	return {};
}

[[nodiscard]] inline
auto snap_value(const blink_TweakerReal& tweaker, float v, float step_size, float snap_amount) -> float {
	return stepify(tweaker, ::tweak::snap_value(v, step_size, snap_amount));
}

} // namespace blink::tweak
