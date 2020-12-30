#pragma once

#include "range_value.hpp"

namespace blkhdgen {

struct EnvelopeSnapSettings
{
	RangeValue<float> step_size;
	float default_snap_amount;
};

}