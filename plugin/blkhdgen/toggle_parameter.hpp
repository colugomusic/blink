#pragma once

#include "parameter.hpp"

namespace blkhdgen {

class ToggleParameter : public Parameter
{
public:
	blkhdgen_ParameterType get_type() const override { return blkhdgen_ParameterType_Toggle; }

	blkhdgen_Error set(bool on)
	{
		return BLKHDGEN_OK;
	}
};

}