#pragma once

#include <blink.h>
#include <cmath>

namespace blink {
namespace tweak {

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

namespace percentage {

[[nodiscard]] inline
auto stepify(float v) -> float {
	return stepify<1000>(v);
}

} // namespace percentage

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

} // namespace tweak
} // namespace blink