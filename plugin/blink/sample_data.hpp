#pragma once

#include <cmath>
#include <blink_sampler.h>
#include <blink/math.hpp>

namespace blink {

class SampleData
{
public:

	SampleData() = default;
	SampleData(SampleData&& rhs) = default;
	SampleData(const blink_SampleInfo* info, blink_ChannelMode channel_mode);
	SampleData& operator=(SampleData&& rhs) = default;

	blink_SR get_SR() const { return info_->SR; }
	blink_FrameCount get_num_frames() const { return info_->num_frames; }
	blink_FrameCount get_data(blink_ChannelCount channel, blink_FrameCount index, blink_FrameCount size, float* buffer) const;
	float read_frame(blink_ChannelCount channel, int pos) const;
	float read_frame_interp(blink_ChannelCount channel, float pos, bool loop = false) const;
	ml::DSPVector read_frames(blink_ChannelCount channel, const ml::DSPVectorInt& pos) const;
	ml::DSPVector read_frames_interp(blink_ChannelCount channel, const snd::transport::DSPVectorFramePosition& pos, bool loop) const;

	float get_loop_pos(float pos) const;
	snd::transport::DSPVectorFramePosition get_loop_pos(const snd::transport::DSPVectorFramePosition& pos) const;

	template <std::size_t ROWS>
	ml::DSPVectorArray<ROWS> read_frames_interp(const snd::transport::DSPVectorFramePosition& pos, bool loop) const;

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

	InterpPos get_interp_pos(float pos, bool loop = false) const;
	InterpVectorPos get_interp_pos(snd::transport::DSPVectorFramePosition pos, bool loop) const;

	const blink_SampleInfo* info_;
	blink_FrameCount loop_length_;
	blink_ChannelMode channel_mode_;
};

inline SampleData::SampleData(const blink_SampleInfo* info, blink_ChannelMode channel_mode)
	: info_{info}
	, loop_length_{info->loop_points ? info->loop_points[1].value - info->loop_points[0].value : 0}
	, channel_mode_{channel_mode}
{
}

inline blink_FrameCount SampleData::get_data(blink_ChannelCount channel, blink_FrameCount index, blink_FrameCount size, float* buffer) const
{
	return info_->get_data(info_->host, channel, index, size, buffer);
}

inline float SampleData::read_frame(blink_ChannelCount channel, int pos) const
{
	if (pos < 0 || pos >= int(info_->num_frames.value))
	{
		return 0.0f;
	}
	else
	{
		float out;

		// Could return zero if sample header wasn't loaded yet.
		if (get_data(channel, {uint64_t(pos)}, {1}, &out).value == 0) return 0.0f;

		return out;
	}
}

inline ml::DSPVector SampleData::read_frames(blink_ChannelCount channel, const ml::DSPVectorInt& pos) const
{
	ml::DSPVector out;

	for (int i = 0; i < kFloatsPerDSPVector; i++)
	{
		if (pos[i] < 0 || pos[i] >= int(info_->num_frames.value))
		{
			out[i] = 0.0f;
		}
		else
		{
			// Could return zero if sample header wasn't loaded yet.
			if (get_data(channel, {uint64_t(pos[i])}, {1}, &(out[i])).value == 0) break;
		}
	}

	return out;
}

inline float SampleData::get_loop_pos(float pos) const
{
	if (!info_->loop_points)
	{
		return math::wrap(pos, float(info_->num_frames.value));
	}
	
	return math::wrap(pos - info_->loop_points[0].value, float(loop_length_.value)) + info_->loop_points[0].value;
}

inline snd::transport::DSPVectorFramePosition SampleData::get_loop_pos(const snd::transport::DSPVectorFramePosition& pos) const
{
	if (!info_->loop_points)
	{
		return math::wrap(pos, float(info_->num_frames.value));
	}
	
	return math::wrap(pos - std::int32_t(info_->loop_points[0].value), float(loop_length_.value)) + std::int32_t(info_->loop_points[0].value);
}

inline auto SampleData::get_interp_pos(float pos, bool loop) const -> InterpPos
{
	InterpPos out;

	if (loop)
	{
		pos = get_loop_pos(pos);
	}

	out.next = int(std::ceil(pos));
	out.prev = int(std::floor(pos));

	out.x = pos - out.prev;

	return out;
}

inline auto SampleData::get_interp_pos(snd::transport::DSPVectorFramePosition pos, bool loop) const -> InterpVectorPos
{
	InterpVectorPos out;

	if (loop)
	{
		pos = get_loop_pos(pos);
	}

	out.next = math::ceil(pos);
	out.prev = math::floor(pos);

	if (loop)
	{
		out.next[kFloatsPerDSPVector - 1] %= info_->num_frames.value;
	}

	out.x = pos.fract;

	return out;
}

inline float SampleData::read_frame_interp(blink_ChannelCount channel, float pos, bool loop) const
{
	const auto interp_pos = get_interp_pos(pos, loop);

	const auto next_value = read_frame(channel, interp_pos.next);
	const auto prev_value = read_frame(channel, interp_pos.prev);

	return (interp_pos.x * (next_value - prev_value)) + prev_value;
}

inline ml::DSPVector SampleData::read_frames_interp(blink_ChannelCount channel, const snd::transport::DSPVectorFramePosition& pos, bool loop) const
{
	const auto interp_pos = get_interp_pos(pos, loop);

	const auto next_value = read_frames(channel, interp_pos.next);
	const auto prev_value = read_frames(channel, interp_pos.prev);

	return (interp_pos.x * (next_value - prev_value)) + prev_value;
}

template <std::size_t ROWS>
inline ml::DSPVectorArray<ROWS> SampleData::read_frames_interp(const snd::transport::DSPVectorFramePosition& pos, bool loop) const
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