#pragma once

#include <functional>
#include "manipulator_target_spec.hpp"

namespace blink {

struct ManipulatorChordTargetSpec : public ManipulatorTargetSpec
{
	std::function<int(const blink_ChordData* data, blink_Position block_pos, int search_beg_index, int* left)> search_binary;
	std::function<int(const blink_ChordData* data, blink_Position block_pos, int search_beg_index, int* left)> search_forward;
};

}