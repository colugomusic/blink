#pragma once

#include <cmath>
#include <blink_sampler.h>
#include "math.hpp"

namespace blink {

class SampleData
{
public:

	SampleData() = default;
	SampleData(SampleData&& rhs) = default;
	SampleData(const blink_SampleInfo* info, blink_ChannelMode channel_mode);
	SampleData& operator=(SampleData&& rhs) = default;

	blink_FrameCount get_data(blink_ChannelCount channel, blink_Index index, blink_FrameCount size, float* buffer) const;
	float read_frame(blink_ChannelCount channel, int pos) const;
	float read_frame_interp(blink_ChannelCount channel, float pos) const;
	ml::DSPVector read_frames(blink_ChannelCount channel, const ml::DSPVectorInt& pos) const;
	ml::DSPVector read_frames_interp(blink_ChannelCount channel, const ml::DSPVector& pos, bool loop) const;

	template <std::size_t ROWS>
	ml::DSPVectorArray<ROWS> read_frames_interp(const ml::DSPVector& pos, bool loop) const;

	blink_ChannelMode get_channel_mode() const { return channel_mode_; }

private:

	struct InterpPos
	{
		int prev;
		int next;
		float x;
	};

	struct InterpVectorPos
	{
		ml::DSPVectorInt prev;
		ml::DSPVectorInt next;
		ml::DSPVector x;
	};

	InterpPos get_interp_pos(float pos) const;
	InterpVectorPos get_interp_pos(ml::DSPVector pos, bool loop) const;

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

inline float SampleData::read_frame(blink_ChannelCount channel, int pos) const
{
	if (pos < 0 || pos >= int(info_->num_frames))
	{
		return 0.0f;
	}
	else
	{
		float out;

		// Could return zero if sample header wasn't loaded yet.
		if (get_data(channel, pos, 1, &out) == 0) return 0.0f;

		return out;
	}
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

inline auto SampleData::get_interp_pos(float pos) const -> InterpPos
{
	InterpPos out;

	out.next = int(std::ceil(pos));
	out.prev = int(std::floor(pos));

	out.x = pos - out.prev;

	return out;
}

inline auto SampleData::get_interp_pos(ml::DSPVector pos, bool loop) const -> InterpVectorPos
{
	InterpVectorPos out;

	if (loop) pos = math::wrap(pos, float(info_->num_frames));

	out.next = math::ceil(pos);
	out.prev = math::floor(pos);

	out.x = pos - ml::intToFloat(out.prev);

	return out;
}

inline float SampleData::read_frame_interp(blink_ChannelCount channel, float pos) const
{
	const auto interp_pos = get_interp_pos(pos);

	const auto next_value = read_frame(channel, interp_pos.next);
	const auto prev_value = read_frame(channel, interp_pos.prev);

	return (interp_pos.x * (next_value - prev_value)) + prev_value;
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