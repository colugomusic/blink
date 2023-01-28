#pragma once

#include "parameter.hpp"
#include "option_spec.hpp"
#include <blink/searcher.hpp>

namespace blink {

class OptionParameter : public Parameter
{
public:

	const std::vector<std::string> options;
	const blink_Index default_index;
	const blink_StdIcon icon;
	const StepSearcher searcher;
	const int flags;

	OptionParameter(OptionSpec spec);

	blink_ParameterType get_type() const override { return blink_ParameterType_Option; }

	const char* get_text(blink_Index index) const;
	blink_Index get_max_index() const { return blink_Index(options.size() - 1); }
};

inline OptionParameter::OptionParameter(OptionSpec spec)
	: Parameter { spec }
	, options { spec.options }
	, default_index { spec.default_index }
	, icon(spec.icon)
	, searcher(spec.searcher, default_index)
	, flags { spec.flags }
{
}

inline const char* OptionParameter::get_text(blink_Index index) const
{
	if (options.empty())
	{
		assert (bool(flags & blink_OptionFlags_IsToggle));

		if (index > 0) return "ON";

		return "OFF";
	}

	return options[index].c_str();
}

}