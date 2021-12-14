#pragma once

#include <tweak/tweak.hpp>
#include <blink/slider_spec.hpp>

namespace blink {

template <class T>
class Slider
{
public:

	Slider(SliderSpec<T> spec);

	const char* display_value(T value) const;

	const SliderSpec<T>& spec() const { return spec_; }
	const tweak::Tweaker<T>& tweaker() const { return tweaker_; }

private:

	SliderSpec<T> spec_;
	tweak::Tweaker<T> tweaker_;
	mutable std::string display_value_buffer_;
};

template <class T>
Slider<T>::Slider(SliderSpec<T> spec)
	: spec_(spec)
	, tweaker_(tweak::Spec<T>(spec))
{
}

template <class T>
const char* Slider<T>::display_value(T value) const
{
	display_value_buffer_ = tweaker_.to_string(value);

	return display_value_buffer_.c_str();
}

}
