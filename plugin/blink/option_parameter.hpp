#pragma once

#include "parameter.hpp"
#include "option_spec.hpp"

namespace blink {

class OptionParameter : public Parameter
{
public:

	OptionParameter(OptionSpec spec);

	blink_ParameterType get_type() const override { return blink_ParameterType_Option; }

	const char* get_text(blink_Index index) const;
	blink_Index get_max_index() const { return blink_Index(options_.size() - 1); }
	blink_Index get_default_index() const { return default_index_; }

private:

	std::vector<std::string> options_;
	blink_Index default_index_;
};

inline OptionParameter::OptionParameter(OptionSpec spec)
	: Parameter(spec)
	, options_(spec.options)
	, default_index_(spec.default_index)
{
}

inline const char* OptionParameter::get_text(blink_Index index) const
{
	return options_[index].c_str();
}

}