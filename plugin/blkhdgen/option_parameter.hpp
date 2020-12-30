#pragma once

#include "parameter.hpp"

namespace blkhdgen {

class OptionParameter : public Parameter
{
public:
	blkhdgen_ParameterType get_type() const override { return blkhdgen_ParameterType_Option; }

	blkhdgen_Error set(blkhdgen_Index value);
	const char* get_text(blkhdgen_Index value);
};

}