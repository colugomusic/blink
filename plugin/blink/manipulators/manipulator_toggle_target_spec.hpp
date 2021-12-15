#pragma once

#include "manipulator_target_spec.hpp"

namespace blink {

struct ManipulatorToggleTargetSpec : public ManipulatorTargetSpec
{
	bool default_value = false;
};

}