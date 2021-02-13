#pragma once

#include <map>
#include "envelope_spec.hpp"
#include "group.hpp"
#include "parameter.hpp"
#include "sample_data.hpp"
#include "slider_spec.hpp"

namespace blkhdgen {

class Generator
{

public:

	virtual ~Generator() {}

	virtual blkhdgen_Error process(blkhdgen_SR song_rate, blkhdgen_SR sample_rate, const blkhdgen_Position* pos, float** out) = 0;
	virtual blkhdgen_Error get_waveform_positions(const blkhdgen_Position* block_positions, float* out, float* derivative = nullptr) const = 0;
	virtual size_t get_required_aux_buffer_size() const = 0;
	virtual blkhdgen_Error preprocess_sample(void* host, blkhdgen_PreprocessCallbacks callbacks) const = 0;

	void set_data_offset(int offset);
	int get_num_channels() const { return 2; }

	//blkhdgen_Error set_get_sample_info_cb(void* user, blkhdgen_GetSampleInfoCB cb);
	//blkhdgen_Error set_get_sample_data_cb(void* user, blkhdgen_GetSampleDataCB cb);
	//blkhdgen_Error set_get_warp_point_data_cb(void* user, blkhdgen_GetWarpPointDataCB cb);
	//blkhdgen_Error set_get_manipulator_data_cb(void* user, blkhdgen_GetManipulatorDataCB cb);

protected:

	const blkhdgen_SampleInfo* get_sample_info() const;
	blkhdgen_FrameCount get_sample_data(blkhdgen_ChannelCount channel, blkhdgen_Index index, blkhdgen_FrameCount size, float* buffer) const;
	ml::DSPVector read_sample_frames_interp(blkhdgen_ChannelCount channel, const ml::DSPVector& pos, bool loop);
	int get_data_offset() const { return data_offset_; }

	static ml::DSPVectorArray<2> stereo_pan(const ml::DSPVectorArray<2> in, float pan, std::shared_ptr<EnvelopeParameter> pan_envelope, Traverser* traverser);

private:

	std::function<blkhdgen_WarpPoints*()> get_warp_point_data_;
	//std::function<blkhdgen_ManipulatorData*()> get_manipulator_data_;

	SampleData sample_data_;
	std::atomic<int> data_offset_;
};

//blkhdgen_Error Generator::set_get_sample_info_cb(void* user, blkhdgen_GetSampleInfoCB cb)
//{
//	sample_data_.set_get_sample_info_cb(user, cb);
//
//	return BLKHDGEN_OK;
//}
//
//blkhdgen_Error Generator::set_get_sample_data_cb(void* user, blkhdgen_GetSampleDataCB cb)
//{
//	sample_data_.set_get_sample_data_cb(user, cb);
//
//	return BLKHDGEN_OK;
//}

void Generator::set_data_offset(int offset)
{
	data_offset_ = offset;
}

const blkhdgen_SampleInfo* Generator::get_sample_info() const
{
	return sample_data_.get_info();
}

blkhdgen_FrameCount Generator::get_sample_data(blkhdgen_ChannelCount channel, blkhdgen_Index index, blkhdgen_FrameCount size, float* buffer) const
{
	return sample_data_.get_data(channel, index, size, buffer);
}


//blkhdgen_Error Generator::set_get_warp_point_data_cb(void* user, blkhdgen_GetWarpPointDataCB cb)
//{
//	get_warp_point_data_ = [user, cb]()
//	{
//		return cb(user);
//	};
//
//	return BLKHDGEN_OK;
//}

//blkhdgen_Error Generator::set_get_manipulator_data_cb(void* user, blkhdgen_GetManipulatorDataCB cb)
//{
//	get_manipulator_data_ = [user, cb]()
//	{
//		return cb(user);
//	};
//
//	return BLKHDGEN_OK;
//}

ml::DSPVector Generator::read_sample_frames_interp(blkhdgen_ChannelCount channel, const ml::DSPVector& pos, bool loop)
{
	return sample_data_.read_frames_interp(channel, pos, loop);
}

ml::DSPVectorArray<2> Generator::stereo_pan(const ml::DSPVectorArray<2> in, float pan, std::shared_ptr<EnvelopeParameter> pan_envelope, Traverser* traverser)
{
	auto out = in;

	ml::DSPVector env_pan;

	for (int i = 0; i < kFloatsPerDSPVector; i++)
	{
		env_pan[i] = pan_envelope->get_mod_value(traverser);
	}

	const auto zero = ml::DSPVector(0.0f);
	const auto one = ml::DSPVector(1.0f);

	env_pan = ml::clamp(env_pan + pan, ml::DSPVector(-1.0f), ml::DSPVector(1.0f));

	const auto pan_amp_L = ml::lerp(one, zero, ml::max(zero, env_pan));
	const auto pan_amp_R = ml::lerp(one, zero, ml::max(zero, 0.0f - env_pan));

	const auto pan_vec = ml::concatRows(pan_amp_L, pan_amp_R);

	out *= pan_vec;

	out.row(0) += out.row(1) * (1.0f - pan_amp_R);
	out.row(1) += out.row(0) * (1.0f - pan_amp_L);

	return out;
}

}