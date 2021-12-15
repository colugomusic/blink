#pragma once

#include "parameter_spec.hpp"

namespace blink {

struct ToggleSpec : public ParameterSpec
{
	bool default_value = false;
	blink_StdIcon icon = blink_StdIcon_None;
	int flags = 0;
};

}