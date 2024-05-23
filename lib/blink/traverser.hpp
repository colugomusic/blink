#pragma once

#include <cstdint>
#include <blink.h>
#include "block_positions.hpp"

#pragma warning(push, 0)
#include <DSP/MLDSPOps.h>
#pragma warning(pop)

namespace blink {

//
// Generates a vector of block read positions and a vector of search reset points to
// indicate places where traversers need to reset themselves
// 
class Traverser
{
public:

	void generate(uint64_t state_id, const BlockPositions& block_positions, int n = kFloatsPerDSPVector)
	{
		generate(block_positions, n);

		if (state_id != state_id_)
		{
			set_reset(0);
			state_id_ = state_id;
		}
	}

	void generate(const BlockPositions& block_positions, int n = kFloatsPerDSPVector)
	{
		block_positions_ = &block_positions;
		reset_ = 0;

		auto position = block_positions.prev_pos;

		for (int i = 0; i < n; i++)
		{
			if (block_positions.positions[i] < position)
			{
				reset_[i] = 1;
			}

			position = block_positions.positions[i];
		}
	}

	const BlockPositions& block_positions() const { return *block_positions_; }
	const ml::DSPVectorInt& get_resets() const { return reset_; }
	void set_reset(int index) { reset_[index] = 1; }

private:

	const BlockPositions* block_positions_;
	ml::DSPVectorInt reset_;
	uint64_t state_id_ { 0 };
};

}