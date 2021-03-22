#pragma once

#include <cmath>
#include <blink_sampler.h>
#include "math.hpp"

namespace blink {

class SampleData
{
public:

	SampleData(const blink_SampleInfo* info, blink_ChannelMode channel_mode);

	blink_FrameCount get_data(blink_ChannelCount channel, blink_Index index, blink_FrameCount size, float* buffer) const;
	ml::DSPVector read_frames(blink_ChannelCount channel, const ml::DSPVectorInt& pos) const;
	ml::DSPVector read_frames_interp(blink_ChannelCount channel, const ml::DSPVector& pos, bool loop) const;

	template <std::size_t ROWS>
	ml::DSPVectorArray<ROWS> read_frames_interp(const ml::DSPVector& pos, bool loop) const;

	blink_ChannelMode get_channel_mode() const { return channel_mode_; }

private:

	struct InterpPos
	{
		ml::DSPVectorInt prev;
		ml::DSPVectorInt next;
		ml::DSPVector x;
	};

	InterpPos get_interp_pos(ml::DSPVector pos, bool loop) const;

	const blink_SampleInfo* info_;
	blink_ChannelMode channel_mode_;
};

inline SampleData::SampleData(const blink_SampleInfo* info, blink_ChannelMode channel_mode)
	: info_(info)
	, channel_mode_(channel_mode)
{
}

inline blink_FrameCount SampleData::get_data(blink_ChannelCount channel, blink_Index index, blink_FrameCount size, float* buffer) const
{
	return info_->get_data(info_->host, channel, index, size, buffer);
}

inline ml::DSPVector SampleData::read_frames(blink_ChannelCount channel, const ml::DSPVectorInt& pos) const
{
	ml::DSPVector out;

	for (int i = 0; i < kFloatsPerDSPVector; i++)
	{
		if (pos[i] < 0 || pos[i] >= int(info_->num_frames))
		{
			out[i] = 0.0f;
		}
		else
		{
			// Could return zero if sample header wasn't loaded yet.
			if (get_data(channel, pos[i], 1, &(out[i])) == 0) break;
		}
	}

	return out;
}

inline auto SampleData::get_interp_pos(ml::DSPVector pos, bool loop) const -> InterpPos
{
	InterpPos out;

	if (loop) pos = math::wrap(pos, float(info_->num_frames));

	out.next = math::ceil(pos);
	out.prev = math::floor(pos);

	out.x = pos - ml::intToFloat(out.prev);

	return out;
}

inline ml::DSPVector SampleData::read_frames_interp(blink_ChannelCount channel, const ml::DSPVector& pos, bool loop) const
{
	const auto interp_pos = get_interp_pos(pos, loop);

	const auto next_value = read_frames(channel, interp_pos.next);
	const auto prev_value = read_frames(channel, interp_pos.prev);

	return (interp_pos.x * (next_value - prev_value)) + prev_value;
}

template <std::size_t ROWS>
inline ml::DSPVectorArray<ROWS> SampleData::read_frames_interp(const ml::DSPVector& pos, bool loop) const
{
	ml::DSPVectorArray<ROWS> out;

	const auto interp_pos = get_interp_pos(pos, loop);

	for (int r = 0; r < ROWS; r++)
	{
		const auto next_value = read_frames(r, interp_pos.next);
		const auto prev_value = read_frames(r, interp_pos.prev);

		out.row(r) = (interp_pos.x * (next_value - prev_value)) + prev_value;
	}

	return out;
}

}