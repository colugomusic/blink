#pragma once

#include <map>
#include "envelope_spec.hpp"
#include "group.hpp"
#include "parameter.hpp"
#include "sample_data.hpp"
#include "slider_spec.hpp"
#include "envelope_parameter.hpp"

namespace blkhdgen {

class GeneratorBase
{

public:

	virtual ~GeneratorBase() {}

	int get_num_channels() const { return 2; }

	static ml::DSPVectorArray<2> stereo_pan(const ml::DSPVectorArray<2> in, float pan, std::shared_ptr<EnvelopeParameter> pan_envelope, Traverser* traverser);

};

inline ml::DSPVectorArray<2> GeneratorBase::stereo_pan(const ml::DSPVectorArray<2> in, float pan, std::shared_ptr<EnvelopeParameter> pan_envelope, Traverser* traverser)
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