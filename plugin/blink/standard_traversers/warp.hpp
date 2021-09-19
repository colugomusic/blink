#pragma once 

#include <blink_sampler.h>
#include "../math.hpp"

namespace blink {
namespace std_traversers {

class WarpCalculator
{
public:

	float calculate(const blink_WarpPoints* warp_points, float sculpted_position, float* derivative = nullptr)
	{
		if (warp_points->count == 1)
		{
			if (derivative) *derivative = 1.0f;

			return sculpted_position - float(warp_points->points[0].y - warp_points->points[0].x);
		}

		for (blink_Index i = point_search_index_; i < warp_points->count; i++)
		{
			point_search_index_ = i;

			auto p1 = warp_points->points[i];

			if (sculpted_position < p1.y)
			{
				if (i == 0)
				{
					auto x = sculpted_position / p1.y;

					if (derivative) *derivative = 1.0f;

					return p1.x + (sculpted_position - p1.y);
				}

				auto p0 = warp_points->points[i - 1];

				auto segment_size = p1.y - p0.y;

				if (segment_size > 0.f)
				{
					auto x = (sculpted_position - p0.y) / segment_size;

					if (derivative)
					{
						const auto x_diff = (p1.x - p0.x);
						const auto y_diff = (p1.y - p0.y);

						if (y_diff == 0.0f) *derivative = 0.0f;
						else *derivative = float(x_diff) / y_diff;
					}

					return math::lerp(float(p0.x), float(p1.x), float(x));
				}
			}
		}

		auto p0 = warp_points->points[warp_points->count - 1];

		point_search_index_ = warp_points->count;

		if (derivative) *derivative = 1.0f;

		return p0.x + (sculpted_position - p0.y);
	}

	void reset()
	{
		point_search_index_ = 0;
	}

private:

	int point_search_index_ = 0;
};

}}
