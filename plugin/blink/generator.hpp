#pragma once

#include <map>
#include "envelope_spec.hpp"
#include "group.hpp"
#include "parameter.hpp"
#include "sample_data.hpp"
#include "slider_spec.hpp"

namespace blink {

class Generator
{

public:

	virtual ~Generator() {}

	virtual blink_Error process(blink_SR song_rate, blink_SR sample_rate, const blink_Position* pos, float** out) = 0;
	virtual blink_Error get_waveform_positions(const blink_Position* block_positions, float* out, float* derivative = nullptr) const = 0;
	virtual size_t get_required_aux_buffer_size() const = 0;
	virtual blink_Error preprocess_sample(void* host, blink_PreprocessCallbacks callbacks) const = 0;

	void set_data_offset(int offset);
	int get_num_channels() const { return 2; }

protected:

	blink_FrameCount get_sample_data(blink_ChannelCount channel, blink_Index index, blink_FrameCount size, float* buffer) const;
	ml::DSPVector read_sample_frames_interp(blink_ChannelCount channel, const ml::DSPVector& pos, bool loop);
	int get_data_offset() const { return data_offset_; }

	//static ml::DSPVectorArray<2> stereo_pan(const ml::DSPVectorArray<2> in, float pan, std::shared_ptr<EnvelopeParameter> pan_envelope, Traverser* traverser);

private:

	std::function<blink_WarpPoints*()> get_warp_point_data_;
	//std::function<blink_ManipulatorData*()> get_manipulator_data_;

	std::atomic<int> data_offset_;
};


void Generator::set_data_offset(int offset)
{
	data_offset_ = offset;
}

}