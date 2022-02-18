#pragma once

#include <blink/parameters/chord_parameter.hpp>
#include <blink/plugin.hpp>

namespace blink {

class ChordIndexData
{
public:

	static constexpr inline auto EMPTY { 0 };

	const blink_ChordData* const data;
	const blink_Scale value;
	const ChordParameter& chord;

	ChordIndexData(const ChordParameter& chord_, const blink_ParameterData* param_data, blink_Index index)
		: data { param_data ? &param_data[index].chord : nullptr }
		, value { data && data->points.count > 0 ? data->points.data[0].y : EMPTY }
		, chord { chord_ }
	{
	}

	blink_Scale search(blink_Position block_position) const
	{
		if (!data || data->points.count <= 1) return value;

		return chord.searcher.search(*data, block_position);
	}

	blink_Scale search(const BlockPositions& block_positions) const
	{
		return search(block_positions.positions[0]);
	}

	void search_vec(const BlockPositions& block_positions, int n, blink_Scale* out) const
	{
		if (!data || data->points.count <= 1)
		{
			std::fill(out, out + n, value);
			return;
		}

		chord.searcher.search_vec(*data, block_positions, n, out);
	}

	void search_vec(const BlockPositions& block_positions, blink_Scale* out) const
	{
		if (!data || data->points.count <= 1)
		{
			std::fill(out, out + block_positions.count, value);
			return;
		}

		chord.searcher.search_vec(*data, block_positions, out);
	}

	ml::DSPVectorInt search_vec(const BlockPositions& block_positions) const
	{
		if (!data || data->points.count <= 1) return ml::DSPVectorInt{ int32_t(value) };

		return chord.searcher.search_vec_(*data, block_positions);
	}
};

template <int Index>
class ChordData : public ChordIndexData
{
public:

	ChordData(const ChordParameter& chord_, const blink_ParameterData* param_data)
		: ChordIndexData(chord_, param_data, Index)
	{
	}
};

} // blink
