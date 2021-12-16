#pragma once

#include <blink/plugin.hpp>
#include <blink/parameters/option_parameter.hpp>

namespace blink {

template <int Index>
class OptionData
{
public:

	OptionData(const blink::Plugin* plugin, const blink_ParameterData* param_data, const blink::OptionParameter* param)
		: data_(plugin->get_option_data<Index>(param_data))
		, param_(param)
	{
	}

	blink_Index get() const { return blink_Index(data_->data.points[0].y); }

private:

	const blink_OptionData* data_;
	const blink::OptionParameter* param_;
};

} // blink