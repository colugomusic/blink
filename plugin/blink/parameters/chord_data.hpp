#pragma once

#include <blink/parameters/chord_parameter.hpp>
#include <blink/plugin.hpp>

namespace blink {

template <int Index>
class ChordData
{
public:

	ChordData(const blink::Plugin& plugin, const blink_ParameterData* param_data, const blink::ChordParameter* param)
		: data_(&plugin.get_chord_data<Index>(param_data))
		, param_(param)
	{
	}

	void search_vec(const BlockPositions& block_positions, int n, int* out) const
	{
		param_->search().search_vec(*data_, block_positions, n, out);
	}

	void search_vec(const BlockPositions& block_positions, int* out) const
	{
		param_->search().search_vec(*data_, block_positions, out);
	}

	ml::DSPVectorInt search_vec(const BlockPositions& block_positions) const
	{
		return param_->search().search_vec_(*data_, block_positions);
	}

private:

	const blink_ChordData* data_;
	const blink::ChordParameter* param_;
};

} // blink
