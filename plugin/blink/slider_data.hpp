#pragma once

#include <blink/plugin.hpp>
#include <blink/envelope.hpp>

namespace blink {

class SliderIndexData
{
public:

	const blink_SliderData* const data;
	const float default_value;
	const float value;
	const blink::Slider<float>& slider;

	SliderIndexData(const blink::Slider<float>& slider_, const blink_ParameterData* param_data, blink_Index index)
		: data { param_data ? &param_data[index].slider : nullptr }
		, default_value { slider_.spec.default_value }
		, value { data && data->points.count > 0 ? data->points.data[0].y : default_value }
		, slider { slider_ }
	{
	}

	float search(blink_Position block_position) const
	{
		if (!data || data->points.count <= 1) return value;

		return slider.searcher.search(data->points, block_position);
	}

	float search(const BlockPositions& block_positions) const
	{
		return search(block_positions.positions[0]);
	}

	void search_vec(const BlockPositions& block_positions, int n, float* out) const
	{
		if (!data || data->points.count <= 1)
		{
			std::fill(out, out + n, value);
			return;
		}

		slider.searcher.search_vec(data->points, block_positions, n, out);
	}

	void search_vec(const BlockPositions& block_positions, float* out) const
	{
		if (!data || data->points.count <= 1)
		{
			std::fill(out, out + block_positions.count, value);
			return;
		}

		slider.searcher.search_vec(data->points, block_positions, out);
	}

	ml::DSPVector search_vec(const BlockPositions& block_positions) const
	{
		if (!data || data->points.count <= 1) return value;

		return slider.searcher.search_vec_(data->points, block_positions);
	}
};

class IntSliderIndexData
{
public:

	const blink_IntSliderData* const data;
	const int default_value;
	const int value;
	const blink::Slider<int>* const slider;

	IntSliderIndexData(const blink::Slider<int>& slider_, const blink_ParameterData* param_data, blink_Index index)
		: data {param_data ? &param_data[index].int_slider : nullptr }
		, default_value { slider_.spec.default_value }
		, value { data && data->points.count > 0 ? data->points.data[0].y : default_value }
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
