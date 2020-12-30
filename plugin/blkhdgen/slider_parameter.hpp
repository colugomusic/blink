#pragma once

#include "parameter.hpp"

namespace blkhdgen {

class SliderParameter : public Parameter
{
public:
	blkhdgen_ParameterType get_type() const override { return blkhdgen_ParameterType_Slider; }

	float normalize(float value) const;
	float inverse_normalize(float value) const;
	const char* display_value(float value) const;
	blkhdgen_Error set(float value);

	const RangeValue<float>& range();
};

}