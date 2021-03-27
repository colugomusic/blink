#pragma once

#include <functional>
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
	RangeSpec<float> range;
	SliderSpec<float> step_size;
	float default_value = 0.0f;
	float default_snap_amount = 0.0f;
	int flags = 0;
};

}