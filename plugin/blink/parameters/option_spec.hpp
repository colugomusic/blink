#pragma once

#include <map>
#include <string>
#include <vector>
#include "parameter_spec.hpp"
#include <blink/searcher_spec.hpp>

namespace blink {

struct OptionSpec : public ParameterSpec
{
	blink_Index default_index;
	blink_StdIcon icon = blink_StdIcon_None;
	StepSearcherSpec searcher;
	std::vector<std::string> options;
};

}
