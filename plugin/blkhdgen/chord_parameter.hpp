#pragma once

#include "parameter.hpp"

namespace blkhdgen {

class ChordParameter : public Parameter
{
public:
	blkhdgen_ParameterType get_type() const override { return blkhdgen_ParameterType_Chord; }

	blkhdgen_Error set(blkhdgen_Index note, blkhdgen_Index beg, blkhdgen_Index end, bool on)
	{
		return BLKHDGEN_OK;
	}
};

}