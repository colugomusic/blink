#pragma once

#include "range.hpp"

namespace blkhdgen {

template <class T>
struct RangeValue
{
	Range<T> range;
	T value;
	T step_size;
};

}