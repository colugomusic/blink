#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <limits>

namespace blink {
namespace const_math {

template <typename T>
constexpr auto EPSILON = T(0.001);

template <typename T> [[nodiscard]] constexpr
auto abs(T x) -> T {
	return x < 0.0 ? -x : x;
}

template <typename T> [[nodiscard]] constexpr
auto square(T x) -> T {
	return x * x;
}

template <typename T> [[nodiscard]] constexpr
auto cube(T x) -> T {
	return x * x * x;
}

template <typename T> [[nodiscard]] constexpr
auto sqrt_helper(T x, T g) -> T {
	return abs(g - x / g) < EPSILON<T> ? g : sqrt_helper(x, (g + x / g) / 2);
}

template <typename T> [[nodiscard]] constexpr
auto sqrt(T x) -> T {
	return sqrt_helper(x, T(1));
}

template <typename T> [[nodiscard]] constexpr
auto sin_helper(T x) -> T {
	return x < EPSILON<T> ? x : 3 * sin_helper(x / 3) - 4 * cube(sin_helper(x / 3));
}

template <typename T> [[nodiscard]] constexpr
auto sin(T x) -> T {
	return sin_helper(x < 0 ? -x + M_PI : x);
}

template <typename T> [[nodiscard]] constexpr
auto sinh_helper(T x) -> T {
	return x < EPSILON<T> ? x : 3 * sinh_helper(x / 3) + 4 * cube(sinh_helper(x / 3));
}

template <typename T> [[nodiscard]] constexpr
auto sinh(T x) -> T {
	return x < 0 ? -sinh_helper(-x) : sinh_helper(x);
}

[[nodiscard]] constexpr
auto cos (double x) -> double {
	return sin(M_PI_2 - x);
}

[[nodiscard]] constexpr
auto cosh(double x) -> double {
	return sqrt(1.0 + square(sinh(x)));
}

[[nodiscard]] constexpr
auto pow(double base, int exponent) -> double {
	return exponent < 0
		? 1.0 / pow(base, -exponent)
		: exponent == 0
			? 1.0
			: exponent == 1
				? base
				: base * pow(base, exponent-1);
}

template <typename T> [[nodiscard]] constexpr
auto atan_poly_helper(T res, T num1, T den1, T delta) -> T {
	return res < EPSILON<T> ? res : res + atan_poly_helper((num1 * delta) / (den1 + 2) - num1 / den1, num1 * delta * delta, den1 + 4, delta);
}

template <typename T> [[nodiscard]] constexpr
auto atan_poly(T x) -> T {
	return x + atan_poly_helper(pow(x, 5) / 5 - pow(x, 3) / 3, pow(x, 7), T(7), x * x);
}

[[nodiscard]] constexpr
auto atan_identity(double x) -> double {
	return x <= (2.0 - sqrt(3.0)) ? atan_poly(x) : (M_PI_2 / 3) + atan_poly((sqrt(3) * x - 1) / (sqrt(3) + x));
}

[[nodiscard]] constexpr
auto atan_cmplmntry(double x) -> double {
	return x < 1 ? atan_identity(x) : M_PI_2 - atan_identity(1 / x);
}

[[nodiscard]] constexpr
auto atan(double x) -> double {
	return x >= 0 ? atan_cmplmntry(x) : -atan_cmplmntry(-x);
}

[[nodiscard]] constexpr
auto atan2(double y, double x) -> double {
	return x > 0
		? atan(y / x)
		: y >= 0 && x < 0
			? atan(y / x) + M_PI
			: y < 0 && x < 0
				? atan(y / x) - M_PI
				: y > 0 && x == 0
					? M_PI_2
					: y < 0 && x == 0
						? -M_PI_2
						: 0;
}

[[nodiscard]] constexpr
auto nearest(double x) -> double {
	return (x - 0.5) > (int)(x) ? (int)(x + 0.5) : (int)(x);
}

[[nodiscard]] constexpr
auto fraction(double x) -> double {
	return (x - 0.5) > (int)(x) ? -(((double)(int)(x + 0.5)) - x) : x - ((double)(int)(x));
}

[[nodiscard]] constexpr
auto exp_helper(double r) -> double {
	return 1.0 + r + pow(r, 2) / 2 + pow(r, 3) / 6 + pow(r, 4) / 24 + pow(r, 5) / 120 + pow(r, 6) / 720 + pow(r, 7) / 5040;
}

[[nodiscard]] constexpr
auto exp(double x) -> double {
	return pow(M_E, static_cast<int>(nearest(x))) * exp_helper(fraction(x));
}

[[nodiscard]] constexpr
auto mantissa(double x) -> double {
	return x >= 10 ? mantissa(x / 10) : x < 1 ? mantissa(x * 10) : x;
}

[[nodiscard]] constexpr
auto exponent_helper(double x, int e) -> int {
	return x >= 10 ? exponent_helper(x / 10, e + 1) : x < 1 ? exponent_helper(x * 10, e - 1) : e;
}

[[nodiscard]] constexpr
auto exponent(double x) -> int {
	return exponent_helper(x, 0);
}

[[nodiscard]] constexpr
auto log_helper2(double y) -> double {
	return 2.0 * (y + pow(y, 3) / 3 + pow(y, 5) / 5 + pow(y, 7) / 7 + pow(y, 9) / 9 + pow(y, 11) / 11);
}

[[nodiscard]] constexpr
auto log_helper(double x) -> double {
	return log_helper2((x - 1) / (x + 1));
}

[[nodiscard]] constexpr
auto log(double x) -> double {
	return x == 0
		? -std::numeric_limits<double>::infinity()
		: x < 0
			? std::numeric_limits<double>::quiet_NaN()
			: 2.0 * log_helper(sqrt(mantissa(x))) + 2.3025851 * exponent(x);
}

} // const_math
} // blink