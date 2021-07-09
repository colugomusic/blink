#pragma once

#include <map>
#include "block_positions.hpp"
#include "envelope_spec.hpp"
#include "group.hpp"
#include "parameter.hpp"
#include "sample_data.hpp"
#include "slider_spec.hpp"
#include "envelope_parameter.hpp"

namespace blink {

class GeneratorBase
{

public:

	GeneratorBase(int instance_group)
		: instance_group_(instance_group)
	{
	}

	// Called in UI thread
	virtual void stream_init(blink_SR SR) {}

	virtual ~GeneratorBase() {}

	int get_num_channels() const { return 2; }
	int get_instance_group() const { return instance_group_; }

	static ml::DSPVectorArray<2> stereo_pan(
		const ml::DSPVectorArray<2> in,
		float pan,
		const EnvelopeParameter& pan_envelope,
		const blink_EnvelopeData* data,
		const BlockPositions& block_positions);

protected:

	void begin_process(std::uint64_t buffer_id, const blink_Position* positions, int data_offset)
	{
		if (buffer_id > buffer_id_ + 1)
		{
			reset();
		}

		buffer_id_ = buffer_id;

		block_positions_(positions, data_offset, kFloatsPerDSPVector);
	}

	const BlockPositions& block_positions() const
	{
		return block_positions_;
	}

private:

	virtual void reset() = 0;

	BlockPositions block_positions_;
	int instance_group_ = 0;
	std::uint64_t buffer_id_ = 0;
};

inline ml::DSPVectorArray<2> GeneratorBase::stereo_pan(
	const ml::DSPVectorArray<2> in,
	float pan,
	const EnvelopeParameter& pan_envelope,
	const blink_EnvelopeData* data,
	const BlockPositions& block_positions)
{
	auto out = in;

	auto env_pan = pan_envelope.search_vec(data, block_positions);

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