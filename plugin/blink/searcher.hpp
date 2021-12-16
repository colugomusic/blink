#pragma once

#include "block_positions.hpp"
#include "searcher_spec.hpp"

namespace blink {

class FloatSearcher
{
public:

	FloatSearcher(FloatSearcherSpec spec);

	float search(const blink_FloatPoints& data, float default_value, blink_Position block_position) const;
	void search_vec(const blink_FloatPoints& data, float default_value, const BlockPositions& block_positions, int n, float* out) const;
	void search_vec(const blink_FloatPoints& data, float default_value, const BlockPositions& block_positions, float* out) const;
	ml::DSPVector search_vec(const blink_FloatPoints& data, float default_value, const BlockPositions& block_positions) const;

private:

	FloatSearcherSpec spec_;
};

inline float FloatSearcher::search(const blink_FloatPoints& data, float default_value, blink_Position block_position) const
{
	int left;

	return spec_.binary(&data, default_value, block_position, 0, &left);
}

inline void FloatSearcher::search_vec(const blink_FloatPoints& data, float default_value, const BlockPositions& block_positions, float* out) const
{
	search_vec(data, default_value, block_positions, block_positions.count, out);
}

inline void FloatSearcher::search_vec(const blink_FloatPoints& data, float default_value, const BlockPositions& block_positions, int n, float* out) const
{
	int left = 0;
	bool reset = false;
	auto prev_pos = block_positions.prev_pos;

	for (int i = 0; i < n; i++)
	{
		const auto pos = block_positions.positions[i];

		if (pos < prev_pos)
		{
			// This occurs when Blockhead loops back to an earlier song position.
			// We perform a binary search to get back on track
			reset = true;
		}

		if (reset)
		{
			reset = false;

			out[i] = spec_.binary(&data, default_value, block_positions.positions[i], 0, &left);
		}
		else
		{
			out[i] = spec_.forward(&data, default_value, block_positions.positions[i], left, &left);
		}

		prev_pos = pos;
	}
}

inline ml::DSPVector FloatSearcher::search_vec(const blink_FloatPoints& data, float default_value, const BlockPositions& block_positions) const
{
	ml::DSPVector out;

	search_vec(data, default_value, block_positions, out.getBuffer());

	return out;
}

inline FloatSearcher::FloatSearcher(FloatSearcherSpec spec)
	: spec_(spec)
{
}

}
