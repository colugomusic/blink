#pragma once

#include "range_value.hpp"
#include "slider.hpp"

namespace blink {

struct EnvelopeSnapSettings
{
	Slider<float> step_size;
	float default_snap_amount = 0.0f;
};

}