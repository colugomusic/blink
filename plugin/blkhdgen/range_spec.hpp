#pragma once

#include "slider_spec.hpp"

namespace blkhdgen {

template <class T>
struct RangeSpec
{
	SliderSpec<T> min;
	SliderSpec<T> max;
};

}