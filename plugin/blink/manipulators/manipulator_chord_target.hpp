#pragma once

#include "manipulator_target.hpp"
#include "manipulator_chord_target_spec.hpp"

namespace blink {

class ManipulatorChordTarget : public ManipulatorTarget
{
public:

	blink_MT_Type get_type() const override { return blink_MT_Type_Chord; }

	ManipulatorChordTarget(const ManipulatorChordTargetSpec& spec)
		: ManipulatorTarget(spec)
		, spec_(spec)
	{
	}

	ml::DSPVectorInt search_vec(const blink_ChordData* data, const BlockPositions& block_positions) const;
	void search_vec(const blink_ChordData* data, const BlockPositions& block_positions, int n, int* out) const;
	void search_vec(const blink_ChordData* data, const BlockPositions& block_positions, int* out) const;

private:

	ManipulatorChordTargetSpec spec_;
};

//
// TODO: clean up this copy+pastage from ChordParameter
//
inline ml::DSPVectorInt ManipulatorChordTarget::search_vec(const blink_ChordData* data, const BlockPositions& block_positions) const
{
	ml::DSPVectorInt out;

	search_vec(data, block_positions, out.getBufferInt());

	return out;
}

inline void ManipulatorChordTarget::search_vec(const blink_ChordData* data, const BlockPositions& block_positions, int* out) const
{
	search_vec(data, block_positions, block_positions.count, out);
}

inline void ManipulatorChordTarget::search_vec(const blink_ChordData* data, const BlockPositions& block_positions, int n, int* out) const
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

} // blink