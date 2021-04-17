#pragma once

#include "parameter.hpp"
#include "chord_spec.hpp"

namespace blink {

class ChordParameter : public Parameter
{
public:

	blink_ParameterType get_type() const override { return blink_ParameterType_Chord; }

	ChordParameter(const ChordSpec& spec)
		: Parameter(spec)
		, icon_(spec.icon)
		, flags_(spec.flags)
	{
	}

	blink_StdIcon icon() const { return icon_; }
	int flags() const { return flags_; }

private:

	blink_StdIcon icon_;
	int flags_;
};

}