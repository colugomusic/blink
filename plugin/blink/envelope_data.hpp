#pragma once

#include <blink/plugin.hpp>
#include <blink/envelope_parameter.hpp>

namespace blink {

template <int Index>
class EnvelopeData
{
public:

	EnvelopeData(const blink::Plugin* plugin, const blink_ParameterData* param_data, const blink::EnvelopeParameter* param)
		: data_(plugin->get_envelope_data<Index>(param_data))
		, param_(param)
	{
	}

	float search(blink_Position block_position) const
	{
		return param_->search(data_, block_position);
	}

	float search(const BlockPositions& block_positions) const
	{
		return search(block_positions.positions[0]);
	}

	void search_vec(const BlockPositions& block_positions, int n, float* out) const
	{
		return param_->search_vec(data_, block_positions, n, out);
	}

	void search_vec(const BlockPositions& block_positions, float* out) const
	{
		return param_->search_vec(data_, block_positions, out);
	}

	ml::DSPVector search_vec(const BlockPositions& block_positions) const
	{
		return param_->search_vec(data_, block_positions);
	}

private:

	const blink_EnvelopeData* data_;
	const blink::EnvelopeParameter* param_;
};

} // blink