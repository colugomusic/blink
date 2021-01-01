#pragma once

#include "parameter.hpp"
#include "slider_spec.hpp"

namespace blkhdgen {

template <class T>
class SliderParameter : public Parameter
{
public:

	SliderParameter(SliderSpec<T> spec);

	blkhdgen_ParameterType get_type() const override { return blkhdgen_ParameterType_Slider; }

	T curve(T value) const;
	T inverse_curve(T value) const;
	const char* display_value(T value) const;
	blkhdgen_Error set(T value);
	T get() const;

	const RangeValue<T>& range();

private:

	RangeValue<T> range_;
	std::atomic<T> current_value_;
	std::function<T(T)> curve_;
	std::function<T(T)> inverse_curve_;
	std::function<std::string(T)> display_value_;
	mutable std::string display_value_buffer_;
};

template <class T>
SliderParameter<T>::SliderParameter(SliderSpec<T> spec)
	: Parameter(spec)
	, range_(spec.range)
	, curve_(spec.curve)
	, inverse_curve_(spec.inverse_curve)
	, display_value_(spec.display_value)
{
}

template <class T>
T SliderParameter<T>::curve(T value) const
{
	return curve_(value);
}

template <class T>
T SliderParameter<T>::inverse_curve(T value) const
{
	return inverse_curve_(value);
}

template <class T>
const char* SliderParameter<T>::display_value(T value) const
{
	return (display_value_buffer_ = display_value_(value)).c_str();
}

template <class T>
const RangeValue<T>& SliderParameter<T>::range()
{
	return range_;
}

template <class T>
blkhdgen_Error SliderParameter<T>::set(T value)
{
	current_value_ = value;

	return BLKHDGEN_OK;
}

template <class T>
T SliderParameter<T>::get() const
{
	return current_value_;
}

}