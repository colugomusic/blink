#pragma once

#include <functional>
#include <vector>
#include <blink/range_spec.hpp>
#include <blink.h>

namespace blink {

struct EnvelopeSpec
{
	std::function<float(const blink_EnvelopeData* points, float default_value, blink_Position block_pos, int search_beg_index, int* left)> search_binary;
	std::function<float(const blink_EnvelopeData* points, float default_value, blink_Position block_pos, int search_beg_index, int* left)> search_forward;
	std::function<std::string(float)> to_string;

	/*
	Where to display grid lines. Value returned should be in linear space

	index == 0 is the default value
	index == 1 is the first grid line above the default value
	index == -1 is the first grid line below the default value

	The host calls this repeatedly in both directions until the returned grid
	line falls off the top or bottom of the envelope editor, or until two grid
	lines are generated which are too close together on the screen, or until
	and empty value is returned
	*/
	std::function<std::optional<float>(int index)> get_gridline;

	std::function<float(int index, float step_size)> get_stepline;
	std::function<float(float)> stepify;
	std::function<float(float, float, float)> snap_value;
	RangeSpec<float> range;
	SliderSpec<float> step_size;
	SliderSpec<float> value_slider;
	float default_value = 0.0f;
	float default_snap_amount = 0.0f;
};

}
