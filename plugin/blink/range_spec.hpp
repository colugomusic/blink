#pragma once

#include <blink/slider_spec.hpp>

namespace blink {

template <class T>
struct RangeSpec
{
	SliderSpec<T> min;
	SliderSpec<T> max;
};

}