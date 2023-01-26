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
inline float float_points(const blink_FloatPoints* points, float default_value, blink_Position block_position, int search_beg_index, int* left, SearchFunc search)
{
	*left = 0;

	const auto clamp = [points](float value)
	{
		return std::clamp(value, points->min, points->max);
	};

	if (points->count < 1) return clamp(default_value);
	if (points->count == 1) return clamp(points->data[0].y);

	auto search_beg = points->data + search_beg_index;
	auto search_end = points->data + points->count;
	const auto pos = search(search_beg, search_end);

	if (pos == search_beg)
	{
		// It's the first point
		return clamp(pos->y);
	}

	if (pos == search_end)
	{
		// No points to the right so we're at the end of the envelope
		*left = int(std::distance<const blink_FloatPoint*>(points->data, (pos - 1)));

		return clamp((pos - 1)->y);
	}

	// We're somewhere in between two envelope points. Linear interpolate
	// between them.
	const auto p0 = *(pos - 1);
	const auto p1 = *pos;

	const auto segment_size = p1.x - p0.x;	// Should never be zero
	const auto r = (block_position - p0.x) / segment_size;

	*left = int(std::distance<const blink_FloatPoint*>(points->data, (pos - 1)));

	return math::lerp(clamp(p0.y), clamp(p1.y), float(r));
}

// Use a binary search to locate the envelope position
inline float float_points_binary(const blink_FloatPoints* points, float default_value, blink_Position block_position, int search_beg_index, int* left)
{
	const auto find = [block_position](const blink_FloatPoint* beg, const blink_FloatPoint* end)
	{
		const auto less = [](blink_Position position, blink_FloatPoint point)
		{
			return position < point.x;
		};

		return std::upper_bound(beg, end, block_position, less);
	};

	return float_points(points, default_value, block_position, search_beg_index, left, find);
}

// Use a forward search to locate the envelope position (can be
// faster when envelope is being traversed forwards)
inline float float_points_forward(const blink_FloatPoints* points, float default_value, blink_Position block_position, int search_beg_index, int* left)
{
	const auto find = [block_position](const blink_FloatPoint* beg, const blink_FloatPoint* end)
	{
		const auto greater = [block_position](blink_FloatPoint point)
		{
			return point.x > block_position;
		};

		return std::find_if(beg, end, greater);
	};

	return float_points(points, default_value, block_position, search_beg_index, left, find);
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

	if (data->points.count < 2) return 0;

	auto search_beg = data->points.data + search_beg_index;
	auto search_end = data->points.data + data->points.count;
	const auto pos = search(search_beg, search_end);

	if (pos == search_beg)
	{
		// The scale to the right is the first one
		return 0;
	}

	// Otherwise always return the scale to the left
	return (pos - 1)->y;
}

// Use a binary search to locate the scale at the block position
inline int chord_binary(const blink_ChordData* data, blink_Position block_position, int search_beg_index, int* left)
{
	const auto find = [block_position](const blink_ChordBlock* beg, const blink_ChordBlock* end)
	{
		const auto less = [](blink_Position position, blink_ChordBlock block)
		{
			return position < block.x;
		};

		return std::upper_bound(beg, end, block_position, less);
	};

	return chord(data, block_position, search_beg_index, left, find);
}

// Use a forward search to locate the scale at the block position (can be
// faster when block is being traversed forwards)
inline int chord_forward(const blink_ChordData* data, blink_Position block_position, int search_beg_index, int* left)
{
	const auto find = [block_position](const blink_ChordBlock* beg, const blink_ChordBlock* end)
	{
		const auto greater = [block_position](blink_ChordBlock block)
		{
			return block.x > block_position;
		};

		return std::find_if(beg, end, greater);
	};

	return chord(data, block_position, search_beg_index, left, find);
}

// returns the step value at the given block position
// [search_beg_index] is the index of the step transition to begin searching from
// [left] returns the index of the step transition to the left of the block position,
//        or zero if there isn't one.
//        in some scenarios this can be passed as search_beg_index to
//        speed up the search in the next iteration
template <class SearchFunc>
inline int64_t step(const blink_IntPoints* points, int64_t default_value, blink_Position block_position, int search_beg_index, int* left, SearchFunc search)
{
	*left = 0;

	if (points->count < 1) return default_value;
	if (points->count == 1) return points->data[0].y;

	auto search_beg = points->data + search_beg_index;
	auto search_end = points->data + points->count;
	const auto pos = search(search_beg, search_end);

	if (pos == search_beg)
	{
		// The step to the right is the first one
		return default_value;
	}

	*left = int(std::distance<const blink_IntPoint*>(points->data, (pos - 1)));

	if (pos == search_end)
	{
		// Nothing to the right so we're at the end
		return default_value;
	}

	// We're somewhere in between two bool transitions.
	// Return the step on the left
	return (pos - 1)->y;
}

// Use a binary search to locate the block position
inline int64_t step_binary(const blink_IntPoints* points, int64_t default_value, blink_Position block_position, int search_beg_index, int* left)
{
	const auto find = [block_position](const blink_IntPoint* beg, const blink_IntPoint* end)
	{
		const auto less = [](blink_Position position, blink_IntPoint point)
		{
			return position < point.x;
		};

		return std::upper_bound(beg, end, block_position, less);
	};

	return step(points, default_value, block_position, search_beg_index, left, find);
}

// Use a forward search to locate the block position (can be
// faster when block is being traversed forwards)
inline int64_t step_forward(const blink_IntPoints* points, int64_t default_value, blink_Position block_position, int search_beg_index, int* left)
{
	const auto find = [block_position](const blink_IntPoint* beg, const blink_IntPoint* end)
	{
		const auto greater = [block_position](blink_IntPoint point)
		{
			return point.x > block_position;
		};

		return std::find_if(beg, end, greater);
	};

	return step(points, default_value, block_position, search_beg_index, left, find);
}

} // search
} // blink
} // std_params