#pragma once

#include "parameter.hpp"
#include "toggle_spec.hpp"

namespace blkhdgen {

class ToggleParameter : public Parameter
{
public:

	ToggleParameter(ToggleSpec spec);

	blkhdgen_ParameterType get_type() const override { return blkhdgen_ParameterType_Toggle; }

	bool get_default_value() const;
	int get_flags() const;

private:

	bool default_value_;
	int flags_;
};

inline ToggleParameter::ToggleParameter(ToggleSpec spec)
	: Parameter(spec)
	, default_value_(spec.default_value)
	, flags_(spec.flags)
{

}

inline bool ToggleParameter::get_default_value() const
{
	return default_value_;
}

inline int ToggleParameter::get_flags() const
{
	return flags_;
}

}