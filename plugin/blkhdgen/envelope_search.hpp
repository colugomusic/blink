#pragma once

#include <algorithm>
#include <blkhdgen/draft.h>
#include "math.hpp"

namespace blkhdgen {

inline float envelope_search(blkhdgen_EnvelopePoints* points, float min, float max, blkhdgen_Position block_position, float default_value, int* search_index)
{
	if (!points) return default_value;
	if (points->count < 1) return default_value;
	if (points->count == 1) return points->points[0].position.y;

	const auto beg = points->points;
	const auto end = points->points + points->count;

	auto search_pos = beg;

	if (*search_index < 0)
	{
		const auto find = [](blkhdgen_Position a, blkhdgen_EnvelopePoint point)
		{
			return a < point.position.x;
		};

		const auto pos = std::upper_bound(points->points, points->points + points->count, block_position, find);

		if (pos != beg && pos != end)
		{
			search_pos = pos - 1;
		}
	}
	else
	{
		search_pos = beg + *search_index;
	}

	for (; search_pos != end; search_pos++, *search_index++)
	{
		const auto p1 = search_pos;

		if (block_position < p1->position.x)
		{
			if (search_pos == beg)
			{
				return beg->position.y;
			}

			const auto p0 = search_pos - 1;
			const auto segment_size = p1->position.x - p0->position.x;

			if (segment_size > 0)
			{
				auto t = (block_position - p0->position.x) / segment_size;

				return math::lerp(p0->position.y, p1->position.y, float(t));
			}
		}
	}

	*search_index = points->count - 1;

	return (end - 1)->position.y;
}

}