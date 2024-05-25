#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include <snd/transport/frame_position.hpp>

#pragma warning(push, 0)
#include <DSP/MLDSPOps.h>
#pragma warning(pop)

namespace blink {
namespace math {

template <class T>
constexpr T midpoint(T a, T b)
{
	return (a + b) / T(2);
}

template <class T>
constexpr T lerp(T a, T b, T x)
{
	return (x * (b - a)) + a;
}

template <class T>
constexpr T inverse_lerp(T a, T b, T x)
{
	return (x - a) / (b - a);
}

template <class T>
constexpr T stepify(T value, T step)
{
	if (step != 0)
	{
		value = std::floor(value / step + T(0.5)) * step;
	}

	return value;
}

template <size_t ROWS>
constexpr ml::DSPVectorArray<ROWS> stepify(const ml::DSPVectorArray<ROWS>& value, const ml::DSPVectorArray<ROWS>& step)
{
	return ml::select(ml::intToFloat(ml::truncateFloatToInt((value / step) + 0.5f)) * step, value, ml::greaterThan(step, ml::DSPVectorArray<ROWS>(0.0f)));
}

namespace convert {

template <class T>
constexpr T linear_to_delay_time_ms(T linear)
{
	return T(2000) * (linear * linear * linear * linear);
}

template <class T>
constexpr T delay_time_ms_to_linear(T ms)
{
	return const_math::sqrt(const_math::sqrt(ms / T(2000)));
}

template <class T>
inline T bi_to_uni(T bi)
{
	return (bi + T(1)) / T(2);
}

template <class T>
inline T uni_to_bi(T uni)
{
	return (uni * T(2)) - T(1);
}

inline float pitch_to_frequency(float pitch)
{
	return 8.1758f * std::pow(2.0f, pitch / 12.0f);
}

inline ml::DSPVector pitch_to_frequency(const ml::DSPVector& pitch)
{
	return ml::DSPVector(8.1758f) * ml::pow(ml::DSPVector(2.0f), pitch / 12.0f);
}

inline float frequency_to_pitch(float frequency)
{
	return 12.0f * (std::log(frequency / 8.1758f) / std::log(2.0f));
}

template <int FREQ_MIN, int FREQ_MAX>
inline float linear_to_filter_hz(float linear)
{
	return pitch_to_frequency(lerp(frequency_to_pitch(float(FREQ_MIN)), frequency_to_pitch(float(FREQ_MAX)), linear));
}

template <int FREQ_MIN, int FREQ_MAX>
inline ml::DSPVector linear_to_filter_hz(const ml::DSPVector& linear)
{
	return pitch_to_frequency(lerp({frequency_to_pitch(float(FREQ_MIN))}, {frequency_to_pitch(float(FREQ_MAX))}, linear));
}

inline float linear_to_filter_hz(float linear)
{
	return pitch_to_frequency(lerp(-8.513f, 135.076f, linear));
}

inline ml::DSPVector linear_to_filter_hz(const ml::DSPVector& linear)
{
	return pitch_to_frequency(ml::lerp(ml::DSPVector(-8.513f), ml::DSPVector(135.076f), linear));
}

inline float filter_hz_to_linear(float hz)
{
	return inverse_lerp(-8.513f, 135.076f, frequency_to_pitch(hz));
}

inline double linear_to_db(double linear)
{
	return std::log(linear) * 8.6858896380650365530225783783321;
}

inline float linear_to_db(float linear)
{
	return std::log(linear) * 8.6858896380650365530225783783321f;
}

inline double db_to_linear(double db)
{
	return std::exp(db * 0.11512925464970228420089957273422);
}

inline float db_to_linear(float db)
{
	return std::exp(db * 0.11512925464970228420089957273422f);
}

template <class T>
constexpr inline T linear_to_speed(T linear)
{
	return std::pow(T(0.5), -linear);
}

template <class T>
constexpr inline T speed_to_linear(T speed)
{
	return std::log(speed) / std::log(T(2));
}

template <class T>
T p_to_ff(T p)
{
	return std::pow(T(2), p / T(12));
}

template <size_t ROWS>
ml::DSPVectorArray<ROWS> p_to_ff(const ml::DSPVectorArray<ROWS>& p)
{
	return ml::pow(ml::DSPVectorArray < ROWS>(2.0f), p / 12.0f);
}

template <class T>
T ff_to_p(T ff)
{
	return (std::log(ff) / std::log(T(2))) * T(12);
}

template <size_t ROWS>
ml::DSPVectorArray<ROWS> ff_to_p(const ml::DSPVectorArray<ROWS>& ff)
{
	return (ml::log(ff) / ml::log(ml::DSPVectorArray<ROWS>(2.0f))) * ml::DSPVectorArray<ROWS>(12.0f);
}

template <size_t ROWS>
ml::DSPVectorArray<ROWS> p_to_t(int SR, const ml::DSPVectorArray<ROWS>& p)
{
	return float(SR) / pitch_to_frequency(p + 60.0f);
}

} // convert

// Apply curve and return normalized value
template <class T, class Curve>
constexpr T transform_and_normalize(Curve curve, T min, T max, T value)
{
	return curve(inverse_lerp(min, max, value));
}

// Apply curve
template <class T, class Curve>
constexpr T transform(Curve curve, T min, T max, T value)
{
	return lerp(min, max, transform_and_normalize(curve, min, max, value));
}

// Apply curve and return value in range [min..max]
template <class T, class Curve>
constexpr T transform_and_denormalize(Curve curve, T min, T max, T value)
{
	return lerp(min, max, curve(value));
}

// Apply inverse curve and return normalized value
template <class T,class InverseCurve>
constexpr T inverse_transform_and_normalize(InverseCurve inverse_curve, T min, T max, T value)
{
	return inverse_curve(inverse_lerp(min, max, value));
}

template <class T>
inline T wrap(T x, T y)
{
	x = std::fmod(x, y);

	if (x < T(0)) x += y;

	return x;
}

inline int wrap(int x, int y)
{
	x = x % y;

	if (x < 0) x += y;

	return x;
}

inline std::int64_t wrap(std::int64_t x, std::int64_t y)
{
	x = x % y;

	if (x < 0) x += y;

	return x;
}

template <size_t ROWS>
snd::transport::DSPVectorArrayFramePosition<ROWS> wrap(const snd::transport::DSPVectorArrayFramePosition<ROWS>& x, float y)
{
	snd::transport::DSPVectorArrayFramePosition<ROWS> out;

	for (int i = 0; i < kFloatsPerDSPVector; i++)
	{
		out.set(i, std::fmod(double(x[i]), double(y)));

		if (out[i] < 0) out.set(i, out[i] + y);
	}

	return out;
}

template <size_t ROWS>
ml::DSPVectorArrayInt<ROWS> ceil(const snd::transport::DSPVectorArrayFramePosition<ROWS>& in)
{
	return (in + 1).pos;
}

template <size_t ROWS>
ml::DSPVectorArrayInt<ROWS> floor(const snd::transport::DSPVectorArrayFramePosition<ROWS>& in)
{
	return in.pos;
}

template <size_t ROWS>
ml::DSPVectorArray<ROWS> wrap(const ml::DSPVectorArray<ROWS>& x, float y)
{
	ml::DSPVectorArray<ROWS> out;

	for (int i = 0; i < kFloatsPerDSPVector; i++)
	{
		out[i] = std::fmod(x[i], y);

		if (out[i] < 0.0f) out[i] += y;
	}

	return out;
}

template <size_t ROWS>
ml::DSPVectorArrayInt<ROWS> ceil(const ml::DSPVectorArray<ROWS>& in)
{
	return ml::truncateFloatToInt(in + 1);
}

template <size_t ROWS>
ml::DSPVectorArrayInt<ROWS> floor(const ml::DSPVectorArray<ROWS>& in)
{
	return ml::truncateFloatToInt(in);
}

namespace window {

template <class T>
inline T tukey(T x, T r)
{
	const auto p0 = r / T(2);
	const auto p1 = T(1) - (r / T(2));

	if (x < p0)
	{
		return T(0.5) * (T(1) + T(std::cos(M_PI * (((T(2) * x) / r) - T(1)))));
	}
	else if (x < p1)
	{
		return T(1);
	}
	else
	{
		return T(0.5) * (T(1) + T(std::cos(M_PI * (((T(2) * x) / r) - (T(2) / r) + T(1)))));
	}
}

}

namespace ease {
namespace exponential {

template <size_t ROWS>
ml::DSPVectorArray<ROWS> in(const ml::DSPVectorArray<ROWS>& x)
{
	return ml::pow({2.0f}, ml::DSPVectorArray<ROWS>{10.0f} * (x - 1.0f));
}

template <size_t ROWS>
ml::DSPVectorArray<ROWS> out(const ml::DSPVectorArray<ROWS>& x)
{
	return ml::DSPVectorArray<ROWS>{1.0f} - (ml::pow({2.0f}, ml::DSPVectorArray<ROWS>{-10.0f} * x));
}

} // exponential

namespace quadratic {

template <class T> T in(T x)
{
	return x * x;
}

template <class T> T out(T x)
{
	return (x * (x - T(2))) * T(-1);
}

template <size_t ROWS>
ml::DSPVectorArray<ROWS> out(const ml::DSPVectorArray<ROWS>& x)
{
	return (x * (x - 2.0f)) * -1.0f;
}

template <class T> T in_out(T x)
{
	x /= T(0.5);

	if (x < T(1)) return x * x * T(0.5);

	x--;

	return (x * (x - T(2)) - T(1)) * T(-0.5);
}

template <class T> T out_in(T x)
{
	if (x < T(0.5))
	{
		x /= T(0.5);

		return T(-0.5) * (x * (x - T(2)));
	}
	else
	{
		return (T(2) * std::pow(x - T(0.5), T(2))) + T(0.5);
	}
}

} // quadratic

namespace parametric {

template <class T> T in_out(T x)
{
	auto sqr = x * x;

	return sqr / (T(2) * (sqr - x) + T(1));
}

} // parametric
} // ease
} // math
} // blink