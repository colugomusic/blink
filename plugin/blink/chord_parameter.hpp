#pragma once

#include "parameter.hpp"

namespace blink {

class ChordParameter : public Parameter
{
public:
	blink_ParameterType get_type() const override { return blink_ParameterType_Chord; }
};

}