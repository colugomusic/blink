#pragma once

#include "parameter.hpp"
#include "slider_spec.hpp"

namespace blkhdgen {

class SliderParameter : public Parameter
{
public:

	SliderParameter(SliderSpec spec);

	blkhdgen_ParameterType get_type() const override { return blkhdgen_ParameterType_Slider; }

	float curve(float value) const;
	float inverse_curve(float value) const;
	const char* display_value(float value) const;
	blkhdgen_Error set(float value);
	float get() const;

	const RangeValue<float>& range();

private:

	RangeValue<float> range_;
	std::atomic<float> current_value_;
	std::function<float(float)> curve_;
	std::function<float(float)> inverse_curve_;
	std::function<std::string(float)> display_value_;
	mutable std::string display_value_buffer_;
};

SliderParameter::SliderParameter(SliderSpec spec)
	: Parameter(spec)
	, range_(spec.range)
	, curve_(spec.curve)
	, inverse_curve_(spec.inverse_curve)
	, display_value_(spec.display_value)
{
}

float SliderParameter::curve(float value) const
{
	return curve_(value);
}

float SliderParameter::inverse_curve(float value) const
{
	return inverse_curve_(value);
}

const char* SliderParameter::display_value(float value) const
{
	return (display_value_buffer_ = display_value_(value)).c_str();
}

const RangeValue<float>& SliderParameter::range()
{
	return range_;
}

blkhdgen_Error SliderParameter::set(float value)
{
	current_value_ = value;

	return BLKHDGEN_OK;
}

float SliderParameter::get() const
{
	return current_value_;
}

}