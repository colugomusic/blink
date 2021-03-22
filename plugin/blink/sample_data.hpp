#pragma once

#include <cmath>
#include <blink_sampler.h>
#include "math.hpp"

namespace blink {

class SampleData
{
public:

	SampleData(const blink_SampleInfo* info);

	blink_FrameCount get_data(blink_ChannelCount channel, blink_Index index, blink_FrameCount size, float* buffer) const;
	ml::DSPVector read_frames(blink_ChannelCount channel, const ml::DSPVectorInt& pos);
	ml::DSPVector read_frames_interp(blink_ChannelCount channel, ml::DSPVector pos, bool loop);

private:

	const blink_SampleInfo* info_;
};

inline SampleData::SampleData(const blink_SampleInfo* info)
	: info_(info)
{
}

inline blink_FrameCount SampleData::get_data(blink_ChannelCount channel, blink_Index index, blink_FrameCount size, float* buffer) const
{
	return info_->get_data(info_->host, channel, index, size, buffer);
}

inline ml::DSPVector SampleData::read_frames(blink_ChannelCount channel, const ml::DSPVectorInt& pos)
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

inline ml::DSPVector SampleData::read_frames_interp(blink_ChannelCount channel, ml::DSPVector pos, bool loop)
{
	if (loop) pos = math::wrap(pos, float(info_->num_frames));

	const auto next = math::ceil(pos);
	const auto prev = math::floor(pos);

	const auto x = pos - ml::intToFloat(prev);

	const auto next_value = read_frames(channel, next);
	const auto prev_value = read_frames(channel, prev);

	return (x * (next_value - prev_value)) + prev_value;
}

}