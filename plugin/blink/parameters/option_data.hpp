#pragma once

#include <blink/plugin.hpp>
#include <blink/parameters/option_parameter.hpp>

namespace blink {

class OptionIndexData
{
public:

	const blink_OptionData* const data;
	const float default_value;
	const float value;
	const OptionParameter& option;

	OptionIndexData(const OptionParameter& option_, const blink_ParameterData* param_data, blink_Index index)
		: data { param_data ? &param_data[index].option : nullptr }
		, default_value { float(option_.default_index) }
		, value { data && data->data.count > 0 ? data->data.points[0].y : default_value }
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