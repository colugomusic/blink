#pragma once

#include "range_value.hpp"

namespace blkhdgen {

template <class T>
struct RangeSpec
{
	RangeValue<T> min_range;
	RangeValue<T> max_range;
};

}