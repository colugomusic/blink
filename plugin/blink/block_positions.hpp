#pragma once

#include <limits>

#pragma warning(push, 0)
#include <DSP/MLDSPOps.h>
#pragma warning(pop)

namespace blink {

struct BlockPositions
{
	ml::DSPVector positions;
	int count = kFloatsPerDSPVector;
	float prev_pos = std::numeric_limits<float>::max();

	BlockPositions()
	{
		positions[count - 1] = std::numeric_limits<float>::max();
	}

	BlockPositions(const ml::DSPVector& positions_, float prev_pos_ = std::numeric_limits<float>::max())
		: positions(positions_)
		, prev_pos(prev_pos_)
	{
	}

	BlockPositions(const blink_Position* blink_positions, int offset, int count_)
		: count(count_)
	{
		for (int i = 0; i < count_; i++)
		{
			positions[i] = float(blink_positions[i] - offset);
		}
	}

	void operator()(const blink_Position* blink_positions, int offset, int count_)
	{
		prev_pos = positions[count - 1];

		for (int i = 0; i < count_; i++)
		{
			positions[i] = float(blink_positions[i] - offset);
		}
		
		count = count_;
	}

	void operator()(const ml::DSPVector& vec_positions, int offset, int count_)
	{
		prev_pos = positions[count - 1];

		positions = vec_positions - float(offset);

		count = count_;
	}
};

}