#pragma once

#include "parameter.hpp"
#include "toggle_spec.hpp"

namespace blkhdgen {

class ToggleParameter : public Parameter
{
public:

	ToggleParameter(ToggleSpec spec);

	blkhdgen_ParameterType get_type() const override { return blkhdgen_ParameterType_Toggle; }

	blkhdgen_Error set(bool on);
	bool get() const;
	bool get_default_value() const;
	int get_flags() const;

private:
	std::atomic<bool> current_value_;
	bool default_value_;
	int flags_;
};

ToggleParameter::ToggleParameter(ToggleSpec spec)
	: Parameter(spec)
	, default_value_(spec.default_value)
	, flags_(spec.flags)
{

}

blkhdgen_Error ToggleParameter::set(bool on)
{
	current_value_ = on;

	return BLKHDGEN_OK;
}

bool ToggleParameter::get() const
{
	return current_value_;
}

bool ToggleParameter::get_default_value() const
{
	return default_value_;
}

int ToggleParameter::get_flags() const
{
	return flags_;
}

}