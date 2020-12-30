#pragma once

#include <map>
#include "parameter_spec.hpp"

namespace blkhdgen {

struct OptionSpec : public ParameterSpec
{
	blkhdgen_Index default_value;
	std::map<int, std::string> options;
};

}