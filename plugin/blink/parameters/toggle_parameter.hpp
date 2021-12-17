#pragma once

#include "parameter.hpp"
#include "toggle_spec.hpp"
#include <blink/searcher.hpp>

namespace blink {

class ToggleParameter : public Parameter
{
public:

	ToggleParameter(ToggleSpec spec);

	blink_ParameterType get_type() const override { return blink_ParameterType_Toggle; }

	const auto& search() const { return searcher_; }
	bool get_default_value() const { return default_value_; }
	blink_StdIcon get_icon() const { return icon_; }
	int get_flags() const { return flags_; }

private:

	BoolSearcher searcher_;
	bool default_value_;
	blink_StdIcon icon_;
	int flags_;
};

inline ToggleParameter::ToggleParameter(ToggleSpec spec)
	: Parameter(spec)
	, searcher_(spec.searcher)
	, default_value_(spec.default_value)
	, icon_(spec.icon)
	, flags_(spec.flags)
{

}
}