#pragma once

#include <map>
#include "parameter_spec.hpp"

namespace blink {

struct OptionSpec : public ParameterSpec
{
	blink_Index default_value;
	std::map<int, std::string> options;
};

}