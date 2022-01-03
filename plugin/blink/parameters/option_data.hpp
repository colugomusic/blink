#pragma once

#include <blink/plugin.hpp>
#include <blink/parameters/option_parameter.hpp>

namespace blink {

template <int Index>
class OptionData
{
public:

	OptionData(const blink_ParameterData* param_data, const blink::OptionParameter& param)
		: data_(&param_data[Index].option)
		, param_(&param)
	{
	}

	blink_Index get() const { return blink_Index(data_->data.points[0].y); }

private:

	const blink_OptionData* data_;
	const blink::OptionParameter* param_;
};

} // blink