#pragma once

#include "block_positions.hpp"
#include "envelope_data.hpp"

namespace blink {

inline ml::DSPVectorArray<2> stereo_pan(
	const ml::DSPVectorArray<2> in,
	float pan,
	const EnvelopeIndexData& pan_envelope,
	const BlockPositions& block_positions)
{
	auto out = in;

	auto env_pan = pan_envelope.search_vec(block_positions);

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

} // blink
