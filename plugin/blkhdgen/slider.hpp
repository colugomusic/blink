#pragma once

#include "slider_spec.hpp"

namespace blkhdgen {

template <class T>
class Slider
{
public:

	Slider(SliderSpec<T> spec);

	const char* display_value(T value) const;

	const SliderSpec<T>& spec() const { return spec_; }

private:

	SliderSpec<T> spec_;
	mutable std::string display_value_buffer_;
};

template <class T>
Slider<T>::Slider(SliderSpec<T> spec)
	: spec_(spec)
{
}

template <class T>
const char* Slider<T>::display_value(T value) const
{
	return (display_value_buffer_ = spec_.display_value(value)).c_str();
}

}