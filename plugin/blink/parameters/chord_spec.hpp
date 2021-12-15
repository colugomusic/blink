#pragma once

#include <functional>
#include "parameter_spec.hpp"

namespace blink {

struct ChordSpec : public ParameterSpec
{
	std::function<int(const blink_ChordData* data, blink_Position block_pos, int search_beg_index, int* left)> search_binary;
	std::function<int(const blink_ChordData* data, blink_Position block_pos, int search_beg_index, int* left)> search_forward;
	blink_StdIcon icon;
	int flags = 0;
};

}