#pragma once

#include "parameter_spec.hpp"

namespace blkhdgen {

struct ToggleSpec : public ParameterSpec
{
	bool default_value = false;
	blkhdgen_StdIcon icon = blkhdgen_StdIcon_None;
	int flags = 0;
};

}