#pragma once

#include <functional>
#include <blkhdgen/draft.h>
#include "range_spec.hpp"

namespace blkhdgen {

struct EnvelopeSpec
{
	blkhdgen_UUID uuid;
	const char* name;
	std::function<float(float, float, float)> normalize;
	std::function<float(float, float, float)> inverse_normalize;
	RangeSpec<float> range;
};

}