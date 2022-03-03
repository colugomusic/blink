#pragma once

#include "parameter.hpp"
#include <blink/parameters/slider_parameter_spec.hpp>

namespace blink {

template <class T>
class SliderParameter : public Parameter
{
public:

	const SliderParameterSpec<T> spec;
	const Slider<T> slider;
	const blink_Range clamp_range;

	SliderParameter(SliderParameterSpec<T> spec_);

	blink_ParameterType get_type() const override;

	const char* display_value(T value) const;

private:

	mutable std::string display_value_buffer_;
};

template <class T>
SliderParameter<T>::SliderParameter(SliderParameterSpec<T> spec_)
	: Parameter(spec_)
	, spec(spec_)
	, slider(spec.slider)
	, clamp_range(spec.clamp_range)
{
}

template <>
inline blink_ParameterType SliderParameter<float>::get_type() const
{
	return blink_ParameterType_Slider;
}

template <>
inline blink_ParameterType SliderParameter<int>::get_type() const
{
	return blink_ParameterType_IntSlider;
}

template <class T>
const char* SliderParameter<T>::display_value(T value) const
{
	return (display_value_buffer_ = spec.slider.display_value(value)).c_str();
}

}
