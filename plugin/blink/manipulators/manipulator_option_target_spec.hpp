#pragma once

#include <string>
#include <vector>
#include "manipulator_target_spec.hpp"

namespace blink {

struct ManipulatorOptionTargetSpec : public ManipulatorTargetSpec
{
	blink_Index default_index;
	std::vector<std::string> options;
};

}
