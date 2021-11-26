#pragma once

#include "bits.hpp"
#include "parameter.hpp"
#include "chord_spec.hpp"
#include "block_positions.hpp"
#include "math.hpp"

#pragma warning(push, 0)
#include <DSP/MLDSPOps.h>
#pragma warning(pop)

namespace blink {

class ChordParameter : public Parameter
{
public:

	blink_ParameterType get_type() const override { return blink_ParameterType_Chord; }

	ChordParameter(const ChordSpec& spec)
		: Parameter(spec)
		, spec_(spec)
	{
	}

	ml::DSPVectorInt search_vec(const blink_ChordData* data, const BlockPositions& block_positions) const;
	void search_vec(const blink_ChordData* data, const BlockPositions& block_positions, int n, int* out) const;
	void search_vec(const blink_ChordData* data, const BlockPositions& block_positions, int* out) const;

	blink_StdIcon icon() const { return spec_.icon; }
	int flags() const { return spec_.flags; }

	static float snap_pitch_to_scale(float pitch, std::int32_t scale)
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

	static ml::DSPVector snap_pitch_to_scale(const ml::DSPVector& pitch, const ml::DSPVectorInt& scale)
	{
		ml::DSPVector out;

		for (int i = 0; i < kFloatsPerDSPVector; i++)
		{
			out[i] = snap_pitch_to_scale(pitch[i], scale[i]);
		}

		return out;
	}

private:

	ChordSpec spec_;
};


inline ml::DSPVectorInt ChordParameter::search_vec(const blink_ChordData* data, const BlockPositions& block_positions) const
{
	ml::DSPVectorInt out;

	search_vec(data, block_positions, out.getBufferInt());

	return out;
}

inline void ChordParameter::search_vec(const blink_ChordData* data, const BlockPositions& block_positions, int* out) const
{
	search_vec(data, block_positions, block_positions.count, out);
}

inline void ChordParameter::search_vec(const blink_ChordData* data, const BlockPositions& block_positions, int n, int* out) const
{
	int left = 0;
	bool reset = false;
	auto prev_pos = block_positions.prev_pos;

	for (int i = 0; i < n; i++)
	{
		const auto pos = block_positions.positions[i];

		if (pos < prev_pos)
		{
			// This occurs when Blockhead loops back to an earlier song position.
			// We perform a binary search to get back on track
			reset = true;
		}

		if (reset)
		{
			reset = false;

			out[i] = spec_.search_binary(data, block_positions.positions[i], 0, &left);
		}
		else
		{
			out[i] = spec_.search_forward(data, block_positions.positions[i], left, &left);
		}

		prev_pos = pos;
	}
}

}