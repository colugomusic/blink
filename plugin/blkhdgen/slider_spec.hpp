#pragma once

#include <functional>
#include <blkhdgen/draft.h>
#include "range_spec.hpp"

namespace blkhdgen {

struct SliderSpec
{
	blkhdgen_UUID uuid;
	const char* name;
	std::function<float(float)> transform;
	std::function<float(float)> inverse_transform;
	std::function<std::string(float)> display_value;
	RangeValue<float> range;
};

}