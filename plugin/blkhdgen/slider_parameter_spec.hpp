#pragma once

#include "slider_spec.hpp"

namespace blkhdgen {

template <class T>
struct SliderParameterSpec : public ParameterSpec
{
	SliderSpec<T> slider;
	blkhdgen_StdIcon icon = blkhdgen_StdIcon_None;
};

}