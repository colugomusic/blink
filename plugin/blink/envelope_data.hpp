#pragma once

#include <blink/plugin.hpp>
#include <blink/envelope.hpp>

namespace blink {

class EnvelopeIndexData
{
public:

	EnvelopeIndexData(const blink::Plugin& plugin, const blink::Envelope& envelope, const blink_ParameterData* param_data, blink_Index index)
		: data_(&plugin.get_envelope_data(param_data, index))
		, envelope_(&envelope)
	{
	}

	float search(blink_Position block_position) const
	{
		return envelope_->search().search(data_->points, block_position);
	}

	float search(const BlockPositions& block_positions) const
	{
		return search(block_positions.positions[0]);
	}

	void search_vec(const BlockPositions& block_positions, int n, float* out) const
	{
		envelope_->search().search_vec(data_->points, block_positions, n, out);
	}

	void search_vec(const BlockPositions& block_positions, float* out) const
	{
		envelope_->search().search_vec(data_->points, block_positions, out);
	}

	ml::DSPVector search_vec(const BlockPositions& block_positions) const
	{
		return envelope_->search().search_vec_(data_->points, block_positions);
	}

	const auto& data() const { return *data_; }

private:

	const blink_EnvelopeData* data_;
	const blink::Envelope* envelope_;
};

template <int Index>
class EnvelopeData : public EnvelopeIndexData
{
public:

	EnvelopeData(const blink::Plugin& plugin, const blink::Envelope& envelope, const blink_ParameterData* param_data)
		: EnvelopeIndexData(plugin, envelope, param_data, Index)
	{
	}
};

} // blink
