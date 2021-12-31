#pragma once

#include <blink/plugin.hpp>
#include <blink/parameters/toggle_parameter.hpp>

namespace blink {

class ToggleIndexData
{
public:

	ToggleIndexData(const blink::Plugin& plugin, const blink::ToggleParameter& toggle, const blink_ParameterData* param_data, blink_Index index)
		: data_(&plugin.get_toggle_data(param_data, index))
		, toggle(&toggle)
	{
	}

	bool value() const
	{
		return data_->data.points[0].value;
	}

	bool search(blink_Position block_position) const
	{
		if (data_->data.count == 1) return data_->data.points[0].value;

		return toggle->search().search(data_->data, block_position);
	}

	bool search(const BlockPositions& block_positions) const
	{
		return search(block_positions.positions[0]);
	}

	void search_vec(const BlockPositions& block_positions, int n, bool* out) const
	{
		if (data_->data.count == 1)
		{
			std::fill(out, out + n, value());
			return;
		}

		toggle->search().search_vec(data_->data, block_positions, n, out);
	}

	void search_vec(const BlockPositions& block_positions, bool* out) const
	{
		if (data_->data.count == 1)
		{
			std::fill(out, out + block_positions.count, value());
			return;
		}

		toggle->search().search_vec(data_->data, block_positions, out);
	}

	ml::DSPVectorInt search_vec(const BlockPositions& block_positions) const
	{
		return toggle->search().search_vec_(data_->data, block_positions);
	}

	const auto& data() const { return *data_; }

private:

	const blink_ToggleData* data_;
	const blink::ToggleParameter* toggle;
};

template <int Index>
class ToggleData : public ToggleIndexData
{
public:

	ToggleData(const blink::Plugin& plugin, const blink::ToggleParameter& toggle, const blink_ParameterData* param_data)
		: ToggleIndexData(plugin, toggle, param_data, Index)
	{
	}
};

} // blink
