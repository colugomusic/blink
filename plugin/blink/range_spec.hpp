#pragma once

#include "slider_spec.hpp"

namespace blink {

template <class T>
struct RangeSpec
{
	SliderSpec<T> min;
	SliderSpec<T> max;
};

}