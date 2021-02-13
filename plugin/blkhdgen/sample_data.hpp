#pragma once

#include <cmath>
#include <blkhdgen_sampler.h>
#include "math.hpp"

namespace blkhdgen {

class SampleData
{
public:

	//blkhdgen_Error set_get_sample_info_cb(void* user, blkhdgen_GetSampleInfoCB cb);
	//blkhdgen_Error set_get_sample_data_cb(void* user, blkhdgen_GetSampleDataCB cb);

	const blkhdgen_SampleInfo* get_info() const;
	blkhdgen_FrameCount get_data(blkhdgen_ChannelCount channel, blkhdgen_Index index, blkhdgen_FrameCount size, float* buffer) const;
	ml::DSPVector read_frames(blkhdgen_ChannelCount channel, const ml::DSPVectorInt& pos);
	ml::DSPVector read_frames_interp(blkhdgen_ChannelCount channel, ml::DSPVector pos, bool loop);

private:

	//std::function<const blkhdgen_SampleInfo* (void)> get_sample_info_;
	//std::function<blkhdgen_FrameCount(blkhdgen_ChannelCount, blkhdgen_Index, blkhdgen_FrameCount, float*)> get_sample_data_;
};

inline const blkhdgen_SampleInfo* SampleData::get_info() const
{
	return nullptr;// get_sample_info_();
}

inline blkhdgen_FrameCount SampleData::get_data(blkhdgen_ChannelCount channel, blkhdgen_Index index, blkhdgen_FrameCount size, float* buffer) const
{
	return 0;// get_sample_data_(channel, index, size, buffer);
}
//
//blkhdgen_Error SampleData::set_get_sample_info_cb(void* user, blkhdgen_GetSampleInfoCB cb)
//{
//	get_sample_info_ = [user, cb]()
//	{
//		return cb(user);
//	};
//
//	return BLKHDGEN_OK;
//}
//
//blkhdgen_Error SampleData::set_get_sample_data_cb(void* user, blkhdgen_GetSampleDataCB cb)
//{
//	get_sample_data_ = [user, cb](blkhdgen_ChannelCount channel, blkhdgen_Index index, blkhdgen_FrameCount size, float* buffer)
//	{
//		return cb(user, channel, index, size, buffer);
//	};
//
//	return BLKHDGEN_OK;
//}

inline ml::DSPVector SampleData::read_frames(blkhdgen_ChannelCount channel, const ml::DSPVectorInt& pos)
{
	ml::DSPVector out;

	//for (int i = 0; i < kFloatsPerDSPVector; i++)
	//{
	//	// Callback returns zero if we go out of bounds
	//	get_sample_data_(channel, pos[i], 1, &(out[i]));
	//}

	return out;
}

inline ml::DSPVector SampleData::read_frames_interp(blkhdgen_ChannelCount channel, ml::DSPVector pos, bool loop)
{
	const auto info = get_info();

	if (loop) pos = math::wrap(pos, float(info->num_frames));

	const auto next = math::ceil(pos);
	const auto prev = math::floor(pos);

	const auto x = pos - ml::intToFloat(prev);

	const auto next_value = read_frames(channel, next);
	const auto prev_value = read_frames(channel, prev);

	return (x * (next_value - prev_value)) + prev_value;
}

}