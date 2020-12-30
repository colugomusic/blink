#pragma once

#include "parameter_spec.hpp"

namespace blkhdgen {

struct ToggleSpec : public ParameterSpec
{
	bool default_value = false;
	int flags = 0;
};

}