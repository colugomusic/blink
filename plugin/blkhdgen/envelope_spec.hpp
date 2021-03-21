#pragma once

#include <functional>
#include "parameter_spec.hpp"
#include "range_spec.hpp"

namespace blkhdgen {

struct EnvelopeSpec : public ParameterSpec
{
	//std::function<float(float)> curve;
	//std::function<float(float)> inverse_curve;
	std::function<std::string(float)> display_value;
	std::function<std::optional<float>(const std::string&)> from_string;
	std::function<float(int)> get_grid_line;
	RangeSpec<float> range;
	SliderSpec<float> step_size;
	float default_value = 0.0f;
	float default_snap_amount = 0.0f;
	int flags = 0;
};

}