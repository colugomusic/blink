#pragma once

#include "bits.hpp"
#include "block_positions.hpp"
#include "search.hpp"

namespace blink {

inline auto stereo_pan(const ml::DSPVectorArray<2>& in, float pan, ml::DSPVector env_pan) -> ml::DSPVectorArray<2>
{
	auto out = in;

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

inline auto stereo_pan(
	const ml::DSPVectorArray<2>& in,
	float pan,
	const EnvData& pan_envelope,
	const BlockPositions& block_positions) -> ml::DSPVectorArray<2>
{
	return stereo_pan(in, pan, blink::search::vec(pan_envelope, block_positions));
}

inline auto snap_pitch_to_scale(float pitch, std::int32_t scale) -> float
{
	if (scale == 0) return pitch;

	auto octaves = int(std::floor(pitch / 12));
	auto octave_offset = (octaves * 12);

	auto note = int(blink::math::wrap(pitch, 12.0f));

	if (blink::bits::check(scale, note)) return float(note + octave_offset);

	int offset = 1;

	for (int i = 0; i < 6; i++)
	{
		int check = blink::math::wrap(note + offset, 12);

		if (blink::bits::check(scale, check)) return float(check + octave_offset);

		check = blink::math::wrap(note - offset, 12);

		if (blink::bits::check(scale, check)) return float(check + octave_offset);

		offset++;
	}

	return pitch;
}

inline auto snap_pitch_to_scale(const ml::DSPVector& pitch, const ml::DSPVectorInt& scale) -> ml::DSPVector
{
	ml::DSPVector out;

	for (int i = 0; i < kFloatsPerDSPVector; i++)
	{
		out[i] = snap_pitch_to_scale(pitch[i], scale[i]);
	}

	return out;
}

} // blink
