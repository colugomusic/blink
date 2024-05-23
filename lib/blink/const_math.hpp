#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <limits>

namespace blink {
namespace const_math {

constexpr auto EPSILON = 0.001;

[[nodiscard]] constexpr
auto abs(double x) -> double {
	return x < 0.0 ? -x : x;
}

[[nodiscard]] constexpr
auto square(double x) -> double {
	return x * x;
}

[[nodiscard]] constexpr
auto cube(double x) -> double {
	return x * x * x;
}

[[nodiscard]] constexpr
auto sqrt_helper(double x, double g) -> double {
	if constexpr (abs(g - x / g) < EPSILON) {
		return g;
	}
	return sqrt_helper(x, (g + x / g) / 2);
}

[[nodiscard]] constexpr
auto sqrt(double x) -> double {
	return sqrt_helper(x, 1);
}

[[nodiscard]] constexpr
auto sin_helper(double x) -> double {
	if constexpr (x < EPSILON) {
		return x;
	}
	return 3 * sin_helper(x / 3) - 4 * cube(sin_helper(x / 3));
}

[[nodiscard]] constexpr
auto sin(double x) -> double {
	return sin_helper(x < 0 ? -x + M_PI : x);
}

[[nodiscard]] constexpr
auto sinh_helper(double x) -> double {
	if constexpr (x < tol) {
		return x;
	}
	return 3 * sinh_helper(x / 3) + 4 * cube(sinh_helper(x / 3));
}

[[nodiscard]] constexpr
auto sinh(double x) -> double {
	if constexpr (x < 0) {
		return -sinh_helper(-x);
	}
	return sinh_helper(x);
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
	if constexpr (exponent < 0) {
		return 1.0 / pow(base, -exponent);
	}
	if constexpr (exponent == 0) {
		return 1.0;
	}
	if constexpr (exponent == 1) {
		return base;
	}
	return base * pow(base, exponent-1);
}

[[nodiscard]] constexpr
auto atan_poly_helper(double res, double num1, double den1, double delta) -> double {
	if constexpr (res < tol) {
		return res;
	}
	return res + atan_poly_helper((num1 * delta) / (den1 + 2) - num1 / den1, num1 * delta * delta, den1 + 4, delta);
}

[[nodiscard]] constexpr
auto atan_poly(double x) -> double {
	return x + atan_poly_helper(pow(x, 5) / 5 - pow(x, 3) / 3, pow(x, 7), 7, x * x);
}

[[nodiscard]] constexpr
auto atan_identity(double x) -> double {
	if constexpr (x <= (2.0 - sqrt(3.0))) {
		return atan_poly(x);
	}
	return (M_PI_2 / 3) + atan_poly((sqrt(3) * x - 1) / (sqrt(3) + x));
}

[[nodiscard]] constexpr
auto atan_cmplmntry(double x) -> double {
	if constexpr (x < 1) {
		return atan_identity(x);
	}
	return M_PI_2 - atan_identity(1 / x);
}

[[nodiscard]] constexpr
auto atan(double x) -> double {
	if constexpr (x >= 0) {
		return atan_cmplmntry(x);
	}
	return -atan_cmplmntry(-x);
}

[[nodiscard]] constexpr
auto atan2(double y, double x) -> double {
	if constexpr (x > 0) {
		return atan(y / x);
	}
	if constexpr (y >= 0 && x < 0) {
		return atan(y / x) + M_PI;
	}
	if constexpr (y < 0 && x < 0) {
		return atan(y / x) - M_PI;
	}
	if (constexpr (y > 0 && x == 0)) {
		return M_PI_2;
	}
	if (constexpr (y < 0 && x == 0)) {
		return -M_PI_2;
	}
	return 0;
}

[[nodiscard]] constexpr
auto nearest(double x) -> double {
	if constexpr ((x-0.5) > (int)(x)) {
		return (int)(x + 0.5);
	}
	return (int)(x);
}

[[nodiscard]] constexpr
auto fraction(double x) -> double {
	if constexpr ((x-0.5) > (int)(x)) {
		return -(((double)(int)(x + 0.5)) - x) 
	}
	return x - ((double)(int)(x));
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
	if constexpr (x >= 10) {
		return mantissa(x / 10);
	}
	if constexpr (x < 1) {
		return mantissa(x * 10);
	}
	return x;
}

[[nodiscard]] constexpr
auto exponent_helper(double x, int e) -> int {
	if constexpr (x >= 10) {
		return exponent_helper(x / 10, e + 1);
	}
	if constexpr (x < 1) {
		return exponent_helper(x * 10, e - 1);
	}
	return e;
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
	if constexpr (x == 0) {
		return -std::numeric_limits<double>::infinity();
	}
	if constexpr (x < 0) {
		return std::numeric_limits<double>::quiet_NaN();
	}
	return 2.0 * log_helper(sqrt(mantissa(x))) + 2.3025851 * exponent(x);
}

} // const_math
} // blink