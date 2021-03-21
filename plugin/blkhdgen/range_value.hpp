#pragma once

#include "range.hpp"

namespace blkhdgen {

template <class T>
struct RangeValue
{
	Range<T> range;
	T value = T(0);
	T step_size = T(0);
};

}