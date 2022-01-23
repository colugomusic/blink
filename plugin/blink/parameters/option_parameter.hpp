#pragma once

#include "parameter.hpp"
#include "option_spec.hpp"

namespace blink {

class OptionParameter : public Parameter
{
public:

	const std::vector<std::string> options;
	const blink_Index default_index;

	OptionParameter(OptionSpec spec);

	blink_ParameterType get_type() const override { return blink_ParameterType_Option; }

	const char* get_text(blink_Index index) const;
	blink_Index get_max_index() const { return blink_Index(options.size() - 1); }
};

inline OptionParameter::OptionParameter(OptionSpec spec)
	: Parameter(spec)
	, options(spec.options)
	, default_index(spec.default_index)
{
}

inline const char* OptionParameter::get_text(blink_Index index) const
{
	return options[index].c_str();
}

}