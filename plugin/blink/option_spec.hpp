#pragma once

#include <map>
#include "parameter_spec.hpp"

namespace blink {

struct OptionSpec : public ParameterSpec
{
	blink_Index default_index;
	std::vector<std::string> options;
};

}