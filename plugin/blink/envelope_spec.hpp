#pragma once

#include <functional>
#include "option_spec.hpp"
#include "parameter_spec.hpp"
#include "range_spec.hpp"
#include <blink.h>

namespace blink {

struct EnvelopeSpec : public ParameterSpec
{
	std::function<float(const blink_EnvelopeData* points, float default_value, blink_Position block_pos, int search_beg_index, int* left)> search_binary;
	std::function<float(const blink_EnvelopeData* points, float default_value, blink_Position block_pos, int search_beg_index, int* left)> search_forward;
	std::function<std::string(float)> display_value;
	std::function<std::optional<float>(const std::string&)> from_string;
	std::function<float(int index)> get_gridline;
	std::function<float(int index, float step_size)> get_stepline;
	std::function<float(float)> stepify;
	std::function<float(float, float, float)> snap_value;
	RangeSpec<float> range;
	SliderSpec<float> step_size;
	SliderSpec<float> value_slider;
	std::vector<blink_Index> options;
	float default_value = 0.0f;
	float default_snap_amount = 0.0f;
	int flags = 0;
};

}