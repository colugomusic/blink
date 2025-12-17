#pragma once

#include <blink.h>
#include <limits>
#include <snd/frame-pos.hpp>
#pragma warning(push, 0)
#include <DSP/MLDSPOps.h>
#pragma warning(pop)

namespace blink {

struct BlockPositions {
	snd::frame_vec<64> positions;
	snd::frame_pos prev_pos = std::numeric_limits<snd::frame_pos>::max();
	int count = BLINK_VECTOR_SIZE;
	BlockPositions() {
		positions.set(count - 1, std::numeric_limits<snd::frame_pos>::max());
	}
	BlockPositions(const snd::frame_vec<64>& positions_, snd::frame_pos prev_pos_ = std::numeric_limits<snd::frame_pos>::max())
		: positions(positions_)
		, prev_pos(prev_pos_)
	{
	}
	BlockPositions(const blink_Position* blink_positions, int count_)
		: count(count_)
	{
		for (int i = 0; i < count_; i++) {
			positions.set(i, blink_positions[i]);
		}
	}
	auto rotate_prev_pos() -> void {
		prev_pos = positions[count - 1];
	}
	auto add(const blink_Position* blink_positions, int count_) -> void {
		prev_pos = positions[count - 1]; 
		for (int i = 0; i < count_; i++) {
			positions.set(i, blink_positions[i]);
		}
		count = count_;
	}
	auto add(const snd::frame_vec<64>& vec_positions, int count_) -> void {
		prev_pos  = positions[count - 1]; 
		positions = vec_positions;
		count     = count_;
	}
	auto operator[](int index) const -> snd::frame_pos {
		if (index == -1) {
			return prev_pos; 
		}
		return positions.at(index);
	}
};

} // blink
