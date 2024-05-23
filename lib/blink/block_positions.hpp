#pragma once

#include <limits>
#include <optional>
#include <blink.h>
#include <snd/transport/frame_position.hpp>

#pragma warning(push, 0)
#include <DSP/MLDSPOps.h>
#pragma warning(pop)

namespace blink {

struct BlockPositions {
	snd::transport::DSPVectorFramePosition positions;
	snd::transport::FramePosition prev_pos = std::numeric_limits<std::int32_t>::max();
	int count = kFloatsPerDSPVector;
	BlockPositions() {
		positions.set(count - 1, std::numeric_limits<std::int32_t>::max());
	}
	BlockPositions(const snd::transport::DSPVectorFramePosition& positions_, snd::transport::FramePosition prev_pos_ = std::numeric_limits<std::int32_t>::max())
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
	void rotate_prev_pos() {
		prev_pos = positions[count - 1];
	}
	void add(const blink_Position* blink_positions, int count_) {
		prev_pos = positions[count - 1]; 
		for (int i = 0; i < count_; i++) {
			positions.set(i, blink_positions[i]);
		}
		count = count_;
	}
	void add(const snd::transport::DSPVectorFramePosition& vec_positions, int count_) {
		prev_pos  = positions[count - 1]; 
		positions = vec_positions;
		count     = count_;
	}
	snd::transport::FramePosition operator[](int index) const {
		if (index == -1) {
			return prev_pos; 
		}
		return positions.at(index);
	}
};

}