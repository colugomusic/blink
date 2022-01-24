#pragma once

#include <blink/plugin.hpp>
#include <blink/envelope.hpp>

namespace blink {

class SliderIndexData
{
public:

	const blink_SliderData* const data;
	const float value;
	const blink::Slider<float>& slider;

	SliderIndexData(const blink::Slider<float>& slider_, const blink_ParameterData* param_data, blink_Index index)
		: data { param_data ? &param_data[index].slider : nullptr }
		, value { data ? data->data.points[0].y : slider_.spec.default_value }
		, slider { slider_ }
	{
	}

	float search(blink_Position block_position) const
	{
		if (!data || data->data.count == 1) return value;

		return slider.searcher.search(data->data, block_position);
	}

	float search(const BlockPositions& block_positions) const
	{
		return search(block_positions.positions[0]);
	}

	void search_vec(const BlockPositions& block_positions, int n, float* out) const
	{
		if (!data || data->data.count == 1)
		{
			std::fill(out, out + n, value);
			return;
		}

		slider.searcher.search_vec(data->data, block_positions, n, out);
	}

	void search_vec(const BlockPositions& block_positions, float* out) const
	{
		if (!data || data->data.count == 1)
		{
			std::fill(out, out + block_positions.count, value);
			return;
		}

		slider.searcher.search_vec(data->data, block_positions, out);
	}

	ml::DSPVector search_vec(const BlockPositions& block_positions) const
	{
		if (!data || data->data.count == 1) return value;

		return slider.searcher.search_vec_(data->data, block_positions);
	}
};

class IntSliderIndexData
{
public:

	const blink_IntSliderData* const data;
	const int value;
	const blink::Slider<int>* const slider;

	IntSliderIndexData(const blink::Slider<int>& slider_, const blink_ParameterData* param_data, blink_Index index)
		: data {param_data ? &param_data[index].int_slider : nullptr }
		, value { data ? data->value : slider_.spec.default_value }
		, slider { &slider_ }
	{
	}
};

template <int Index>
class SliderData : public SliderIndexData
{
public:

	SliderData(const blink::Slider<float>& slider, const blink_ParameterData* param_data)
		: SliderIndexData(slider, param_data, Index)
	{
	}
};

template <int Index>
class IntSliderData : public IntSliderIndexData
{
public:

	IntSliderData(const blink::Slider<int>& slider, const blink_ParameterData* param_data)
		: IntSliderIndexData(slider, param_data, Index)
	{
	}
};

} // blink
