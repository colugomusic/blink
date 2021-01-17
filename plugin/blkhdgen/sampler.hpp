#pragma once

#include <map>
#include "envelope_spec.hpp"
#include "group.hpp"
#include "parameter.hpp"
#include "sample_data.hpp"
#include "slider_spec.hpp"
#include "generator_base.hpp"

namespace blkhdgen {

class Sampler : public GeneratorBase
{

public:

	virtual ~Sampler() {}

	virtual blkhdgen_Error process(blkhdgen_SR song_rate, blkhdgen_SR sample_rate, const blkhdgen_Position* pos, float** out) = 0;
	virtual blkhdgen_Error get_waveform_positions(const blkhdgen_Position* block_positions, float* out, float* derivative = nullptr) const = 0;
	virtual blkhdgen_Error preprocess_sample(void* host, blkhdgen_PreprocessCallbacks callbacks) const = 0;

	blkhdgen_Error set_get_sample_info_cb(void* user, blkhdgen_GetSampleInfoCB cb);
	blkhdgen_Error set_get_sample_data_cb(void* user, blkhdgen_GetSampleDataCB cb);
	blkhdgen_Error set_get_warp_point_data_cb(void* user, blkhdgen_GetWarpPointDataCB cb);

protected:

	const blkhdgen_SampleInfo* get_sample_info() const;
	blkhdgen_FrameCount get_sample_data(blkhdgen_ChannelCount channel, blkhdgen_Index index, blkhdgen_FrameCount size, float* buffer) const;
	ml::DSPVector read_sample_frames_interp(blkhdgen_ChannelCount channel, const ml::DSPVector& pos, bool loop);

private:

	std::function<blkhdgen_WarpPoints*()> get_warp_point_data_;
	SampleData sample_data_;
};

blkhdgen_Error Sampler::set_get_sample_info_cb(void* user, blkhdgen_GetSampleInfoCB cb)
{
	sample_data_.set_get_sample_info_cb(user, cb);

	return BLKHDGEN_OK;
}

blkhdgen_Error Sampler::set_get_sample_data_cb(void* user, blkhdgen_GetSampleDataCB cb)
{
	sample_data_.set_get_sample_data_cb(user, cb);

	return BLKHDGEN_OK;
}

const blkhdgen_SampleInfo* Sampler::get_sample_info() const
{
	return sample_data_.get_info();
}

blkhdgen_FrameCount Sampler::get_sample_data(blkhdgen_ChannelCount channel, blkhdgen_Index index, blkhdgen_FrameCount size, float* buffer) const
{
	return sample_data_.get_data(channel, index, size, buffer);
}

blkhdgen_Error Sampler::set_get_warp_point_data_cb(void* user, blkhdgen_GetWarpPointDataCB cb)
{
	get_warp_point_data_ = [user, cb]()
	{
		return cb(user);
	};

	return BLKHDGEN_OK;
}

ml::DSPVector Sampler::read_sample_frames_interp(blkhdgen_ChannelCount channel, const ml::DSPVector& pos, bool loop)
{
	return sample_data_.read_frames_interp(channel, pos, loop);
}

}