#pragma once

#include "parameter.hpp"
#include "option_spec.hpp"

namespace blink {

class OptionParameter : public Parameter
{
public:

	OptionParameter(OptionSpec spec);

	blink_ParameterType get_type() const override { return blink_ParameterType_Option; }

	blink_Error set(blink_Index value);
	blink_Index get() const;
	const char* get_text(blink_Index value) const;
	blink_Index get_default_value() const;

private:

	std::map<int, std::string> options_;
	std::atomic<blink_Index> current_value_;
	blink_Index default_value_;
};

inline OptionParameter::OptionParameter(OptionSpec spec)
	: Parameter(spec)
	, options_(spec.options)
	, current_value_(spec.default_value)
	, default_value_(spec.default_value)
{
}

inline blink_Error OptionParameter::set(blink_Index value)
{
	current_value_ = value;

	return BLINK_OK;
}

inline blink_Index OptionParameter::get() const
{
	return current_value_;
}

inline const char* OptionParameter::get_text(blink_Index value) const
{
	const auto pos = options_.find(value);

	if (pos == options_.end()) return "";

	return pos->second.c_str();
}

inline blink_Index OptionParameter::get_default_value() const
{
	return default_value_;
}

}