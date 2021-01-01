#pragma once

#include <functional>
#include "parameter_spec.hpp"

namespace blkhdgen {

template <class T>
struct SliderSpec : public ParameterSpec
{
	std::function<T(T)> curve;
	std::function<T(T)> inverse_curve;
	std::function<std::string(T)> display_value;
	RangeValue<T> range;
};

}