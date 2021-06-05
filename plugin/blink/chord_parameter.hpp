#pragma once

#include "parameter.hpp"
#include "chord_spec.hpp"
#include "block_positions.hpp"

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