#pragma once

#include <blink.h>

namespace blink {

struct FloatSearcherSpec
{
	std::function<float(const blink_FloatPoints* points, float default_value, blink_Position block_pos, int search_beg_index, int* left)> binary;
	std::function<float(const blink_FloatPoints* points, float default_value, blink_Position block_pos, int search_beg_index, int* left)> forward;
};

struct ChordSearcherSpec
{
	std::function<blink_Scale(const blink_ChordData* data, blink_Position block_pos, int search_beg_index, int* left)> binary;
	std::function<blink_Scale(const blink_ChordData* data, blink_Position block_pos, int search_beg_index, int* left)> forward;
};

struct StepSearcherSpec
{
	std::function<bool(const blink_IntPoints* points, int64_t default_value, blink_Position block_pos, int search_beg_index, int* left)> binary;
	std::function<bool(const blink_IntPoints* points, int64_t default_value, blink_Position block_pos, int search_beg_index, int* left)> forward;
};

} // blink