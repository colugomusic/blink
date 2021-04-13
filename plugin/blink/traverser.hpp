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
// Can also be used to generate a single position and reset event (the first elements
// of the vectors are used)
//
class Traverser
{
public:

	using DataOffset = std::int32_t;

	void generate(const BlockPositions& block_positions, int n)
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
};

//
// Puts a reset event at the traverser's start if the pointer changed
//
template <class T>
class TraverserResetter
{
public:

	void check(const T* ptr, Traverser* traverser)
	{
		if (ptr != last_ptr_)
		{
			traverser->set_reset(0);
		}

		last_ptr_ = ptr;
	}

private:

	const T* last_ptr_ = nullptr;
};
}