#pragma once

#include <blink/chord_parameter.hpp>
#include <blink/plugin.hpp>

namespace blink {

template <int Index>
class ChordData
{
public:

	ChordData(const blink::Plugin* plugin, const blink_ParameterData* param_data, const blink::ChordParameter* param)
		: data_(plugin->get_chord_data<Index>(param_data))
		, param_(param)
	{
	}

	//float search(blink_Position block_position) const
	//{
	//	return param_->search(data_, block_position);
	//}

	//float search(const BlockPositions& block_positions) const
	//{
	//	return search(block_positions.positions[0]);
	//}

	void search_vec(const BlockPositions& block_positions, int n, float* out) const
	{
		return param_->search_vec(data_, block_positions, n, out);
	}

	void search_vec(const BlockPositions& block_positions, float* out) const
	{
		return param_->search_vec(data_, block_positions, out);
	}

	ml::DSPVectorInt search_vec(const BlockPositions& block_positions) const
	{
		return param_->search_vec(data_, block_positions);
	}

private:

	const blink_ChordData* data_;
	const blink::ChordParameter* param_;
};

} // blink