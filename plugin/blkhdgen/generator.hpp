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

protected:

	blkhdgen_FrameCount get_sample_data(blkhdgen_ChannelCount channel, blkhdgen_Index index, blkhdgen_FrameCount size, float* buffer) const;
	ml::DSPVector read_sample_frames_interp(blkhdgen_ChannelCount channel, const ml::DSPVector& pos, bool loop);
	int get_data_offset() const { return data_offset_; }

	//static ml::DSPVectorArray<2> stereo_pan(const ml::DSPVectorArray<2> in, float pan, std::shared_ptr<EnvelopeParameter> pan_envelope, Traverser* traverser);

private:

	std::function<blkhdgen_WarpPoints*()> get_warp_point_data_;
	//std::function<blkhdgen_ManipulatorData*()> get_manipulator_data_;

	std::atomic<int> data_offset_;
};


void Generator::set_data_offset(int offset)
{
	data_offset_ = offset;
}

}