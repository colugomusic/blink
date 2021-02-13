#pragma once

#include "parameter.hpp"

namespace blkhdgen {

class ChordParameter : public Parameter
{
public:
	blkhdgen_ParameterType get_type() const override { return blkhdgen_ParameterType_Chord; }
};

}