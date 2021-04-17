#pragma once

#include "parameter_spec.hpp"

namespace blink {

struct ChordSpec : public ParameterSpec
{
	blink_StdIcon icon;
	int flags = 0;
};

}