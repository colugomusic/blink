#pragma once

#include <blink/slider_spec.hpp>

namespace blink {

template <class T>
struct SliderParameterSpec : public ParameterSpec
{
	SliderSpec<T> slider;
	blink_StdIcon icon = blink_StdIcon_None;
	blink_Range clamp_range;
};

}