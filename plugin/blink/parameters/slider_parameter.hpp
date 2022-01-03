#pragma once

#include "parameter.hpp"
#include <blink/parameters/slider_parameter_spec.hpp>

namespace blink {

template <class T>
class SliderParameter : public Parameter
{
public:

	SliderParameter(SliderParameterSpec<T> spec);

	blink_ParameterType get_type() const override;

	const char* display_value(T value) const;

	const SliderParameterSpec<T>& spec() const { return spec_; }
	const Slider<T>& slider() const { return slider_; }

private:

	SliderParameterSpec<T> spec_;
	Slider<T> slider_;
	mutable std::string display_value_buffer_;
};

template <class T>
SliderParameter<T>::SliderParameter(SliderParameterSpec<T> spec)
	: Parameter(spec)
	, spec_(spec)
	, slider_(spec.slider)
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
	return (display_value_buffer_ = spec_.slider.display_value(value)).c_str();
}

}
