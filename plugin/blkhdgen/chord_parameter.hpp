#pragma once

#include "parameter.hpp"

namespace blkhdgen {

class ChordParameter : public Parameter
{
public:
	blkhdgen_ParameterType get_type() const override { return blkhdgen_ParameterType_Chord; }
	
	blkhdgen_Error set_get_chord_data_cb(void* user, blkhdgen_GetChordDataCB cb)
	{
		return BLKHDGEN_OK;
	}
};

}