#pragma once

#include <blink/plugin.hpp>
#include <blink/envelope.hpp>

namespace blink {

class SliderIndexData
{
public:

	SliderIndexData(const blink::Plugin& plugin, const blink::Slider<float>& slider, const blink_ParameterData* param_data, blink_Index index)
		: data_(&plugin.get_slider_data(param_data, index))
		, slider_(&slider)
	{
	}

	float value() const
	{
		return data_->data.count > 0 ? data_->data.points[0].y : slider_->spec().default_value;
	}

	float search(blink_Position block_position) const
	{
		return slider_->search().search(data_->data, block_position);
	}

	float search(const BlockPositions& block_positions) const
	{
		return search(block_positions.positions[0]);
	}

	void search_vec(const BlockPositions& block_positions, int n, float* out) const
	{
		slider_->search().search_vec(data_->data, block_positions, n, out);
	}

	void search_vec(const BlockPositions& block_positions, float* out) const
	{
		slider_->search().search_vec(data_->data, block_positions, out);
	}

	ml::DSPVector search_vec(const BlockPositions& block_positions) const
	{
		return slider_->search().search_vec_(data_->data, block_positions);
	}

	const auto& data() const { return *data_; }

private:

	const blink_SliderData* data_;
	const blink::Slider<float>* slider_;
};

template <int Index>
class SliderData : public SliderIndexData
{
public:

	SliderData(const blink::Plugin& plugin, const blink::Slider<float>& slider, const blink_ParameterData* param_data)
		: SliderIndexData(plugin, slider, param_data, Index)
	{
	}
};

} // blink
