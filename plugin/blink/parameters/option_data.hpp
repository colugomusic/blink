#pragma once

#include <blink/plugin.hpp>
#include <blink/parameters/option_parameter.hpp>

namespace blink {

class OptionIndexData
{
public:

	const blink_OptionData* const data;
	const int64_t default_value;
	const int64_t value;
	const OptionParameter& option;

	OptionIndexData(const OptionParameter& option_, const blink_ParameterData* param_data, blink_Index index)
		: data { param_data ? &param_data[index].option : nullptr }
		, default_value { option_.default_index }
		, value { data && data->points.count > 0 ? data->points.data[0].y : default_value }
		, option { option_ }
	{
	}

	OptionIndexData(const OptionIndexData & rhs) = default;
	OptionIndexData(OptionIndexData && rhs) = default;
};

template <int Index>
class OptionData : public OptionIndexData
{
public:

	OptionData(const OptionParameter& option, const blink_ParameterData* param_data)
		: OptionIndexData(option, param_data, Index)
	{
	}

	OptionData(const OptionData & rhs) = default;
	OptionData(OptionData && rhs) = default;
};

} // blink