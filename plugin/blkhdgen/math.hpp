#pragma once

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
	return lerp(min, max, transform_normalize(value));
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

}
}