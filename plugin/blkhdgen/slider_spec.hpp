#pragma once

#include <functional>
#include "parameter_spec.hpp"

namespace blkhdgen {

struct SliderSpec : public ParameterSpec
{
	std::function<float(float)> transform;
	std::function<float(float)> inverse_transform;
	std::function<std::string(float)> display_value;
	RangeValue<float> range;
};

}