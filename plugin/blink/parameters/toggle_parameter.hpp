#pragma once

#include "parameter.hpp"
#include "toggle_spec.hpp"
#include <blink/searcher.hpp>

namespace blink {

class ToggleParameter : public Parameter
{
public:

	const BoolSearcher searcher;
	const bool default_value;
	const blink_StdIcon icon;
	const int flags;

	ToggleParameter(ToggleSpec spec);

	blink_ParameterType get_type() const override { return blink_ParameterType_Toggle; }
};

inline ToggleParameter::ToggleParameter(ToggleSpec spec)
	: Parameter(spec)
	, searcher(spec.searcher)
	, default_value(spec.default_value)
	, icon(spec.icon)
	, flags(spec.flags)
{
}

}