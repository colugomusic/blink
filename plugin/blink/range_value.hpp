#pragma once

#include "range.hpp"

namespace blink {

template <class T>
struct RangeValue
{
	Range<T> range;
	T value = T(0);
	T step_size = T(0);
};

}