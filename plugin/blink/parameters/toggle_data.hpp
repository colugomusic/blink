#pragma once

#include <blink/plugin.hpp>
#include <blink/parameters/toggle_parameter.hpp>

namespace blink {

class ToggleIndexData
{
public:

	const blink_ToggleData* const data;
	const bool default_value;
	const bool value;
	const blink::ToggleParameter& toggle;

	ToggleIndexData(const blink::ToggleParameter& toggle_, const blink_ParameterData* param_data, blink_Index index)
		: data { param_data ? &param_data[index].toggle : nullptr }
		, default_value { toggle_.default_value }
		, value { data && data->data.count > 0 ? data->data.points[0].value == BLINK_TRUE : default_value }
		, toggle { toggle_ }
	{
	}

	bool search(blink_Position block_position) const
	{
		if (!data || data->data.count <= 1) return value;

		return toggle.searcher.search(data->data, block_position);
	}

	bool search(const BlockPositions& block_positions) const
	{
		return search(block_positions.positions[0]);
	}

	void search_vec(const BlockPositions& block_positions, int n, bool* out) const
	{
		if (!data || data->data.count <= 1)
		{
			std::fill(out, out + n, value);
			return;
		}

		toggle.searcher.search_vec(data->data, block_positions, n, out);
	}

	void search_vec(const BlockPositions& block_positions, bool* out) const
	{
		if (!data || data->data.count <= 1)
		{
			std::fill(out, out + block_positions.count, value);
			return;
		}

		toggle.searcher.search_vec(data->data, block_positions, out);
	}

	ml::DSPVectorInt search_vec(const BlockPositions& block_positions) const
	{
		return toggle.searcher.search_vec_(data->data, block_positions);
	}
};

template <int Index>
class ToggleData : public ToggleIndexData
{
public:

	ToggleData(const blink::ToggleParameter& toggle, const blink_ParameterData* param_data)
		: ToggleIndexData(toggle, param_data, Index)
	{
	}
};

} // blink
