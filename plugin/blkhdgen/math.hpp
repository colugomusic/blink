#pragma once

#pragma warning(push, 0)
#include <DSP/MLDSPOps.h>
#pragma warning(pop)

namespace blkhdgen {
namespace math {

template <class T>
constexpr T af_to_db(T v)
{
	return std::log(v) / std::log(T(1.12202));
}

template <class T>
constexpr T db_to_af(T v)
{
	return std::pow(T(1.122018456459045), v);
}

template <class T>
T p_to_ff(T p)
{
	return std::pow(T(2), p / T(12));
}

template <size_t ROWS>
ml::DSPVectorArray<ROWS> p_to_ff(const ml::DSPVectorArray<ROWS>& p)
{
	return ml::pow(2.0f, p / 12.0f);
}

template <class T>
T ff_to_p(T ff)
{
	return (std::log(ff) / std::log(T(2))) * T(12);
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

//template <class T, class Curve>
//constexpr T mod_normalize(Curve curve, T min, T max, T value)
//{
//	return curve_(inverse_lerp(min, max, value));
//}
//
//template <class T, class Curve>
//constexpr T mod_transform(Curve curve, T min, T max, T value)
//{
//	return lerp(min, max, mod_normalize(curve, min, max, value));
//}
//
//template <class T, class Curve, class InverseCurve>
//constexpr T mod_inverse_normalize(Curve curve, InverseCurve inverse_curve, T min, T max, T value)
//{
//	return inverse_curve(lerp(curve(min), curve(max), value));
//}

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

}
}