#pragma once

#include <blink/plugin.hpp>
#include <blink/parameters/option_parameter.hpp>

namespace blink {

class OptionIndexData
{
public:

	const blink_OptionData* const data;
	const blink_Index value;
	const OptionParameter& option;

	OptionIndexData(const OptionParameter& option_, const blink_ParameterData* param_data, blink_Index index)
		: data { param_data ? &param_data[index].option : nullptr }
		, value { data ? data->data.points[0].y : option.default_index }
		, option { option_ }
	{
	}
};

template <int Index>
class OptionData : public OptionIndexData
{
public:

	OptionData(const OptionParameter& option, const blink_ParameterData* param_data)
		: OptionIndexData(option, param_data, Index)
	{
	}
};

} // blink