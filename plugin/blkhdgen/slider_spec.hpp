#pragma once

#include <functional>
#include "parameter_spec.hpp"

namespace blkhdgen {

struct SliderSpec : public ParameterSpec
{
	std::function<float(float)> curve;
	std::function<float(float)> inverse_curve;
	std::function<std::string(float)> display_value;
	RangeValue<float> range;
};

}