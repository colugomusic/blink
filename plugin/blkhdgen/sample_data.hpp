#pragma once

#include <cmath>
#include <blkhdgen_sampler.h>
#include "math.hpp"

namespace blkhdgen {

class SampleData
{
public:

	SampleData(const blkhdgen_SampleInfo* info);

	blkhdgen_FrameCount get_data(blkhdgen_ChannelCount channel, blkhdgen_Index index, blkhdgen_FrameCount size, float* buffer) const;
	ml::DSPVector read_frames(blkhdgen_ChannelCount channel, const ml::DSPVectorInt& pos);
	ml::DSPVector read_frames_interp(blkhdgen_ChannelCount channel, ml::DSPVector pos, bool loop);

private:

	const blkhdgen_SampleInfo* info_;
};

inline SampleData::SampleData(const blkhdgen_SampleInfo* info)
	: info_(info)
{
}

inline blkhdgen_FrameCount SampleData::get_data(blkhdgen_ChannelCount channel, blkhdgen_Index index, blkhdgen_FrameCount size, float* buffer) const
{
	return info_->get_data(info_->host, channel, index, size, buffer);
}

inline ml::DSPVector SampleData::read_frames(blkhdgen_ChannelCount channel, const ml::DSPVectorInt& pos)
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
			get_data(channel, pos[i], 1, &(out[i]));
		}
	}

	return out;
}

inline ml::DSPVector SampleData::read_frames_interp(blkhdgen_ChannelCount channel, ml::DSPVector pos, bool loop)
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