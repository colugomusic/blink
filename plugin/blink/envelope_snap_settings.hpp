/* TODELETE:
#pragma once

#include "range_value.hpp"
#include "slider.hpp"

namespace blink {

struct EnvelopeSnapSettings
{
	Slider<float> step_size;
	float default_snap_amount = 0.0f;

	blink_EnvelopeSnapSettings bind() const
	{
		blink_EnvelopeSnapSettings out;

		out.step_size = Slider<float>::bind(step_size);
		out.default_snap_amount = default_snap_amount;

		return out;
	}
};

}
*/