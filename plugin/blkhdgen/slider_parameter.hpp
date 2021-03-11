#pragma once

#include "parameter.hpp"
#include "slider_spec.hpp"

namespace blkhdgen {

template <class T>
class SliderParameter : public Parameter
{
public:

	SliderParameter(SliderSpec<T> spec);

	blkhdgen_ParameterType get_type() const override;

	T constrain(T value) const;
	T increment(T value, bool precise) const;
	T decrement(T value, bool precise) const;
	T drag(T value, int amount, bool precise) const;
	const char* display_value(T value) const;
	blkhdgen_Error set(T value);
	T get() const;

	T get_default_value() { return default_value_; }

private:

	T default_value_;
	std::atomic<T> current_value_;
	std::function<T(T)> constrain_;
	std::function<T(T, bool)> increment_;
	std::function<T(T, bool)> decrement_;
	std::function<T(T, int, bool)> drag_;
	std::function<std::string(T)> display_value_;
	mutable std::string display_value_buffer_;
};

template <class T>
SliderParameter<T>::SliderParameter(SliderSpec<T> spec)
	: Parameter(spec)
	, default_value_(spec.default_value)
	, constrain_(spec.constrain)
	, increment_(spec.increment)
	, decrement_(spec.decrement)
	, drag_(spec.drag)
	, display_value_(spec.display_value)
{
}

inline blkhdgen_ParameterType SliderParameter<float>::get_type() const
{
	return blkhdgen_ParameterType_Slider;
}

inline blkhdgen_ParameterType SliderParameter<int>::get_type() const
{
	return blkhdgen_ParameterType_IntSlider;
}

template <class T>
T SliderParameter<T>::constrain(T value) const
{
	return constrain_(value);
}

template <class T>
T SliderParameter<T>::increment(T value, bool precise) const
{
	return increment_(value, precise);
}

template <class T>
T SliderParameter<T>::decrement(T value, bool precise) const
{
	return decrement_(value, precise);
}

template <class T>
T SliderParameter<T>::drag(T value, int amount, bool precise) const
{
	return drag_(value, amount, precise);
}

template <class T>
const char* SliderParameter<T>::display_value(T value) const
{
	return (display_value_buffer_ = display_value_(value)).c_str();
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