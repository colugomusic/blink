#pragma once

#include <functional>
#include "parameter_spec.hpp"
#include "range_spec.hpp"

namespace blkhdgen {

struct EnvelopeSpec : public ParameterSpec
{
	std::function<float(float)> transform;
	std::function<float(float)> inverse_transform;
	std::function<std::string(float)> display_value;
	RangeSpec<float> range;
	RangeValue<float> step_size;
	float default_value = 0.0f;
	float default_snap_amount = 0.0f;
	int flags = 0;
};

}