#pragma once

#include <limits>
#include <optional>
#include <snd/transport/frame_position.hpp>

#pragma warning(push, 0)
#include <DSP/MLDSPOps.h>
#pragma warning(pop)

namespace blink {

struct BlockPositions
{
	const blink_Position* raw { nullptr };
	snd::transport::DSPVectorFramePosition positions;
	snd::transport::FramePosition prev_pos { std::numeric_limits<std::int32_t>::max() };
	mutable std::optional<snd::transport::FramePosition> min_;
	mutable std::optional<snd::transport::FramePosition> max_;

	int count { kFloatsPerDSPVector };
	std::int64_t data_offset { 0 };

	BlockPositions()
	{
		positions.set(count - 1, std::numeric_limits<std::int32_t>::max());
	}

	BlockPositions(const snd::transport::DSPVectorFramePosition& positions_, snd::transport::FramePosition prev_pos_ = std::numeric_limits<std::int32_t>::max())
		: positions(positions_)
		, prev_pos(prev_pos_)
	{
	}

	BlockPositions(const blink_Position* blink_positions, std::int64_t offset, int count_)
		: raw(blink_positions)
		, count(count_)
		, data_offset(offset)
	{
		for (int i = 0; i < count_; i++)
		{
			positions.set(i, blink_positions[i] - offset);
		}
	}

	void operator()(const blink_Position* blink_positions, std::int64_t offset, int count_)
	{
		raw = blink_positions;
		prev_pos = positions[count - 1];

		for (int i = 0; i < count_; i++)
		{
			positions.set(i, blink_positions[i] - offset);
		}
		
		count = count_;
		data_offset = offset;
	}

	void operator()(const snd::transport::DSPVectorFramePosition& vec_positions, std::int64_t offset, int count_)
	{
		prev_pos = positions[count - 1];

		positions = vec_positions - std::int32_t(offset);

		count = count_;
		data_offset = offset;
	}

	snd::transport::FramePosition operator[](int index) const
	{
		if (index == -1) return prev_pos;

		return positions.at(index);
	}

	snd::transport::FramePosition min() const
	{
		if (min_) return *min_;

		snd::transport::FramePosition lowest { std::numeric_limits<std::int32_t>::max() };

		for (int i = 0; i < kFloatsPerDSPVector; i++)
		{
			if (positions.at(i) < lowest) lowest = positions.at(i);
		}

		min_ = lowest;

		return *min_;
	}

	snd::transport::FramePosition max() const
	{
		if (max_) return *max_;

		snd::transport::FramePosition highest { std::numeric_limits<std::int32_t>::min() };

		for (int i = 0; i < kFloatsPerDSPVector; i++)
		{
			if (positions.at(i) > highest) highest = positions.at(i);
		}

		min_ = highest;

		return *max_;
	}
};

}