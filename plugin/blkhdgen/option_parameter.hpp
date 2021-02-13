#pragma once

#include "parameter.hpp"
#include "option_spec.hpp"

namespace blkhdgen {

class OptionParameter : public Parameter
{
public:

	OptionParameter(OptionSpec spec);

	blkhdgen_ParameterType get_type() const override { return blkhdgen_ParameterType_Option; }

	blkhdgen_Error set(blkhdgen_Index value);
	blkhdgen_Index get() const;
	const char* get_text(blkhdgen_Index value) const;
	blkhdgen_Index get_default_value() const;

private:

	std::map<int, std::string> options_;
	std::atomic<blkhdgen_Index> current_value_;
	blkhdgen_Index default_value_;
};

inline OptionParameter::OptionParameter(OptionSpec spec)
	: Parameter(spec)
	, options_(spec.options)
	, current_value_(spec.default_value)
	, default_value_(spec.default_value)
{
}

inline blkhdgen_Error OptionParameter::set(blkhdgen_Index value)
{
	current_value_ = value;

	return BLKHDGEN_OK;
}

inline blkhdgen_Index OptionParameter::get() const
{
	return current_value_;
}

inline const char* OptionParameter::get_text(blkhdgen_Index value) const
{
	const auto pos = options_.find(value);

	if (pos == options_.end()) return "";

	return pos->second.c_str();
}

inline blkhdgen_Index OptionParameter::get_default_value() const
{
	return default_value_;
}

}