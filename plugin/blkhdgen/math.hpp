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

}
}