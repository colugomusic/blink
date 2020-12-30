#pragma once

#include "range.hpp"

namespace blkhdgen {

template <class T>
struct RangeSpec
{
	Range<T> min_range;
	Range<T> max_range;

	T min_default_value;
	T max_default_value;
	T step_size;
};

}