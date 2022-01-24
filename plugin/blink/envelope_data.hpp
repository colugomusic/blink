#pragma once

#include <blink/plugin.hpp>
#include <blink/envelope.hpp>

namespace blink {

class EnvelopeIndexData
{
public:

	const blink_EnvelopeData* const data;
	const float default_value;
	const blink::Envelope& envelope;

	EnvelopeIndexData(const blink::Envelope& envelope_, const blink_ParameterData* param_data, blink_Index index)
		: data { param_data ? &param_data[index].envelope : nullptr }
		, default_value { envelope_.default_value }
		, envelope { envelope_ }
	{
	}

	float search(blink_Position block_position) const
	{
		if (!data) return default_value;

		return envelope.searcher.search(data->points, block_position);
	}

	float search(const BlockPositions& block_positions) const
	{
		return search(block_positions.positions[0]);
	}

	void search_vec(const BlockPositions& block_positions, int n, float* out) const
	{
		if (!data || data->points.count == 1)
		{
			std::fill(out, out + n, default_value);
			return;
		}

		envelope.searcher.search_vec(data->points, block_positions, n, out);
	}

	void search_vec(const BlockPositions& block_positions, float* out) const
	{
		if (!data || data->points.count == 1)
		{
			std::fill(out, out + block_positions.count, default_value);
			return;
		}

		envelope.searcher.search_vec(data->points, block_positions, out);
	}

	ml::DSPVector search_vec(const BlockPositions& block_positions) const
	{
		if (!data) return default_value;

		return envelope.searcher.search_vec_(data->points, block_positions);
	}
};

template <int Index>
class EnvelopeData : public EnvelopeIndexData
{
public:

	EnvelopeData(const blink::Envelope& envelope, const blink_ParameterData* param_data)
		: EnvelopeIndexData(envelope, param_data, Index)
	{
	}
};

} // blink
