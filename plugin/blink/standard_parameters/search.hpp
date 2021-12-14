#pragma once

#include <blink/math.hpp>

namespace blink {
namespace std_params {
namespace search {

// returns the y value at the given block position
// [search_beg_index] is the index of the point to begin searching from
// [left] returns the index of the point to the left of the block position,
//        or zero if there isn't one.
//        in some scenarios this can be passed as search_beg_index to
//        speed up the search in the next iteration
template <class SearchFunc>
inline float envelope(const blink_EnvelopeData* data, float default_value, blink_Position block_position, int search_beg_index, int* left, SearchFunc search)
{
	*left = 0;

	const auto clamp = [data](float value)
	{
		return std::clamp(value, data->points.min, data->points.max);
	};

	if (data->points.count < 1) return clamp(default_value);
	if (data->points.count == 1) return clamp(data->points.points[0].y);

	auto search_beg = data->points.points + search_beg_index;
	auto search_end = data->points.points + data->points.count;
	const auto pos = search(search_beg, search_end);

	if (pos == search_beg)
	{
		// It's the first point
		return clamp(pos->y);
	}

	if (pos == search_end)
	{
		// No points to the right so we're at the end of the envelope
		*left = int(std::distance<const blink_FloatPoint*>(data->points.points, (pos - 1)));

		return clamp((pos - 1)->y);
	}

	// We're somewhere in between two envelope points. Linear interpolate
	// between them.
	const auto p0 = *(pos - 1);
	const auto p1 = *pos;

	const auto segment_size = p1.x - p0.x;	// Should never be zero
	const auto r = (block_position - p0.x) / segment_size;

	*left = int(std::distance<const blink_FloatPoint*>(data->points.points, (pos - 1)));

	return math::lerp(clamp(p0.y), clamp(p1.y), float(r));
}

// Use a binary search to locate the envelope position
inline float envelope_binary(const blink_EnvelopeData* data, float default_value, blink_Position block_position, int search_beg_index, int* left)
{
	const auto find = [block_position](const blink_FloatPoint* beg, const blink_FloatPoint* end)
	{
		const auto less = [](blink_Position position, const blink_FloatPoint& point)
		{
			return position < point.x;
		};

		return std::upper_bound(beg, end, block_position, less);
	};

	return envelope(data, default_value, block_position, search_beg_index, left, find);
}

// Use a forward search to locate the envelope position (can be
// faster when envelope is being traversed forwards)
inline float envelope_forward(const blink_EnvelopeData* data, float default_value, blink_Position block_position, int search_beg_index, int* left)
{
	const auto find = [block_position](const blink_FloatPoint* beg, const blink_FloatPoint* end)
	{
		const auto greater = [block_position](const blink_FloatPoint& point)
		{
			return point.x > block_position;
		};

		return std::find_if(beg, end, greater);
	};

	return envelope(data, default_value, block_position, search_beg_index, left, find);
}

// returns the scale value at the given block position
// [search_beg_index] is the index of the scale transition to begin searching from
// [left] returns the index of the scale transition to the left of the block position,
//        or zero if there isn't one.
//        in some scenarios this can be passed as search_beg_index to
//        speed up the search in the next iteration
template <class SearchFunc>
inline int chord(const blink_ChordData* data, blink_Position block_position, int search_beg_index, int* left, SearchFunc search)
{
	*left = 0;

	if (data->blocks.count < 2) return 0;

	auto search_beg = data->blocks.blocks + search_beg_index;
	auto search_end = data->blocks.blocks + data->blocks.count;
	const auto pos = search(search_beg, search_end);

	if (pos == search_beg)
	{
		// The scale to the right is the first one
		return 0;
	}

	*left = int(std::distance<const blink_ChordBlock*>(data->blocks.blocks, (pos - 1)));

	if (pos == search_end)
	{
		// Nothing to the right so we're at the end
		return 0;
	}

	// We're somewhere in between two scale transitions.
	// Return the scale on the left
	return (pos - 1)->scale;
}

// Use a binary search to locate the envelope position
inline int chord_binary(const blink_ChordData* data, blink_Position block_position, int search_beg_index, int* left)
{
	const auto find = [block_position](const blink_ChordBlock* beg, const blink_ChordBlock* end)
	{
		const auto less = [](blink_Position position, const blink_ChordBlock& block)
		{
			return position < block.position;
		};

		return std::upper_bound(beg, end, block_position, less);
	};

	return chord(data, block_position, search_beg_index, left, find);
}

// Use a forward search to locate the envelope position (can be
// faster when envelope is being traversed forwards)
inline int chord_forward(const blink_ChordData* data, blink_Position block_position, int search_beg_index, int* left)
{
	const auto find = [block_position](const blink_ChordBlock* beg, const blink_ChordBlock* end)
	{
		const auto greater = [block_position](const blink_ChordBlock& block)
		{
			return block.position > block_position;
		};

		return std::find_if(beg, end, greater);
	};

	return chord(data, block_position, search_beg_index, left, find);
}

} // search
} // blink
} // std_params