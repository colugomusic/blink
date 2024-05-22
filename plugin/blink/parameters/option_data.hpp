/* TODELETE:
#pragma once

#include <blink/plugin.hpp>
#include <blink/parameters/option_parameter.hpp>

namespace blink {

class OptionIndexData
{
public:

	const blink_OptionData* const data;
	const int64_t default_value;
	const int64_t value;
	const OptionParameter& option;

	OptionIndexData(const OptionParameter& option_, const blink_ParameterData* param_data, blink_Index index)
		: data { param_data ? &param_data[index].option : nullptr }
		, default_value { option_.default_index }
		, value { data && data->points.count > 0 ? data->points.data[0].y : default_value }
		, option { option_ }
	{
	}

	OptionIndexData(const OptionIndexData & rhs) = default;
	OptionIndexData(OptionIndexData && rhs) = default;

	int64_t search(blink_Position block_position) const
	{
		if (!data || data->points.count <= 1) return value;

		return option.searcher.search(data->points, block_position);
	}

	int64_t search(const BlockPositions& block_positions) const
	{
		return search(block_positions.positions[0]);
	}

	void search_vec(const BlockPositions& block_positions, int n, int64_t* out) const
	{
		if (!data || data->points.count <= 1)
		{
			std::fill(out, out + n, value);
			return;
		}

		option.searcher.search_vec(data->points, block_positions, n, out);
	}

	void search_vec(const BlockPositions& block_positions, int64_t* out) const
	{
		if (!data || data->points.count <= 1)
		{
			std::fill(out, out + block_positions.count, value);
			return;
		}

		option.searcher.search_vec(data->points, block_positions, out);
	}

	ml::DSPVectorInt search_vec(const BlockPositions& block_positions) const
	{
		return option.searcher.search_vec_(data->points, block_positions);
	}
};

template <int Index>
class OptionData : public OptionIndexData
{
public:

	OptionData(const OptionParameter& option, const blink_ParameterData* param_data)
		: OptionIndexData(option, param_data, Index)
	{
	}

	OptionData(const OptionData & rhs) = default;
	OptionData(OptionData && rhs) = default;
};

} // blink
*/