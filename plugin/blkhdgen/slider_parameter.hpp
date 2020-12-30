#pragma once

#include "parameter.hpp"
#include "slider_spec.hpp"

namespace blkhdgen {

class SliderParameter : public Parameter
{
public:

	SliderParameter(SliderSpec spec);

	blkhdgen_ParameterType get_type() const override { return blkhdgen_ParameterType_Slider; }

	float transform(float value) const;
	float inverse_transform(float value) const;
	const char* display_value(float value) const;
	blkhdgen_Error set(float value);

	const RangeValue<float>& range();

private:

	RangeValue<float> range_;
	std::function<float(float)> transform_;
	std::function<float(float)> inverse_transform_;
	std::function<std::string(float)> display_value_;
	mutable std::string display_value_buffer_;
};

SliderParameter::SliderParameter(SliderSpec spec)
	: Parameter(spec)
	, range_(spec.range)
	, transform_(spec.transform)
	, inverse_transform_(spec.inverse_transform)
	, display_value_(spec.display_value)
{
}

float SliderParameter::transform(float value) const
{
	return math::inverse_lerp(transform_(range_.range.min), transform_(range_.range.max), transform_(value));
}

float SliderParameter::inverse_transform(float value) const
{
	return inverse_transform_(math::lerp(transform_(range_.range.min), transform_(range_.range.max), value));
}

const char* SliderParameter::display_value(float value) const
{
	return (display_value_buffer_ = display_value_(value)).c_str();
}

const RangeValue<float>& SliderParameter::range()
{
	return range_;
}

}