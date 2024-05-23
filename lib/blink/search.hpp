#pragma once

#include <array>
#include <blink/plugin_impl.hpp>
#include <blink/math.hpp>
#pragma warning(push, 0)
#include <DSP/MLDSPOps.h>
#pragma warning(pop)

namespace blink {
namespace search {

template <typename BinarySearchFn, typename ForwardSearchFn>
struct Searcher {
	BinarySearchFn binary;
	ForwardSearchFn forward;
};

template <typename SearchFn>
auto make_searcher(SearchFn binary, SearchFn forward) -> Searcher<SearchFn, SearchFn> {
	return {binary, forward};
}

template <typename BinarySearchFn, typename ForwardSearchFn>
auto make_searcher(BinarySearchFn binary, ForwardSearchFn forward) -> Searcher<BinarySearchFn, ForwardSearchFn> {
	return {binary, forward};
}

// returns the y value at the given block position
// [search_beg_index] is the index of the point to begin searching from
// [left] returns the index of the point to the left of the block position,
//        or zero if there isn't one.
//        in some scenarios this can be passed as search_beg_index to
//        speed up the search in the next iteration
template <class SearchFunc> [[nodiscard]]
auto float_points(const blink_RealPoints& points, float default_value, blink_Position block_position, size_t search_beg_index, size_t* left, SearchFunc search) -> float {
	*left = 0; 
	const auto clamp = [&points](float value) {
		return std::clamp(value, points.min, points.max);
	}; 
	if (points.count < 1) return clamp(default_value);
	if (points.count == 1) return clamp(points.data[0].y); 
	auto search_beg = points.data + search_beg_index;
	auto search_end = points.data + points.count;
	const auto pos = search(search_beg, search_end); 
	if (pos == search_beg) {
		// It's the first point
		return clamp(pos->y);
	} 
	if (pos == search_end) {
		// No points to the right so we're at the end of the envelope
		*left = int(std::distance<const blink_RealPoint*>(points.data, (pos - 1))); 
		return clamp((pos - 1)->y);
	} 
	// We're somewhere in between two envelope points. Linear interpolate
	// between them.
	const auto p0 = *(pos - 1);
	const auto p1 = *pos; 
	const auto segment_size = p1.x - p0.x;	// Should never be zero
	const auto r = (block_position - p0.x) / segment_size; 
	*left = int(std::distance<const blink_RealPoint*>(points.data, (pos - 1))); 
	return math::lerp(clamp(p0.y), clamp(p1.y), float(r));
}

// Use a binary search to locate the envelope position
[[nodiscard]] inline
auto float_points_binary(const blink_RealPoints& points, float default_value, blink_Position block_position, size_t search_beg_index, size_t* left) -> float {
	const auto find = [block_position](const blink_RealPoint* beg, const blink_RealPoint* end) {
		const auto less = [](blink_Position position, blink_RealPoint point) {
			return position < point.x;
		}; 
		return std::upper_bound(beg, end, block_position, less);
	}; 
	return float_points(points, default_value, block_position, search_beg_index, left, find);
}

// Use a forward search to locate the envelope position (can be
// faster when envelope is being traversed forwards)
[[nodiscard]] inline
auto float_points_forward(const blink_RealPoints& points, float default_value, blink_Position block_position, size_t search_beg_index, size_t* left) -> float {
	const auto find = [block_position](const blink_RealPoint* beg, const blink_RealPoint* end) {
		const auto greater = [block_position](blink_RealPoint point) {
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
template <class SearchFunc> [[nodiscard]] [[nodiscard]]
auto chord(const blink_ChordData& data, blink_Position block_position, size_t search_beg_index, size_t* left, SearchFunc search) -> blink_Scale {
	*left = 0; 
	if (data.points.count < 2) return 0; 
	auto search_beg = data.points.data + search_beg_index;
	auto search_end = data.points.data + data.points.count;
	const auto pos = search(search_beg, search_end); 
	if (pos == search_beg) {
		// The scale to the right is the first one
		return 0;
	} 
	// Otherwise always return the scale to the left
	return (pos - 1)->y;
}

// Use a binary search to locate the scale at the block position
[[nodiscard]] inline
auto chord_binary(const blink_ChordData& data, blink_Position block_position, size_t search_beg_index, size_t* left) -> blink_Scale {
	const auto find = [block_position](const blink_ChordBlock* beg, const blink_ChordBlock* end) {
		const auto less = [](blink_Position position, blink_ChordBlock block) {
			return position < block.x;
		}; 
		return std::upper_bound(beg, end, block_position, less);
	}; 
	return chord(data, block_position, search_beg_index, left, find);
}

// Use a forward search to locate the scale at the block position (can be
// faster when block is being traversed forwards)
[[nodiscard]] inline
auto chord_forward(const blink_ChordData& data, blink_Position block_position, size_t search_beg_index, size_t* left) -> blink_Scale {
	const auto find = [block_position](const blink_ChordBlock* beg, const blink_ChordBlock* end) {
		const auto greater = [block_position](blink_ChordBlock block) {
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
template <class SearchFunc> [[nodiscard]]
auto step(const blink_IntPoints& points, int64_t default_value, blink_Position block_position, size_t search_beg_index, size_t* left, SearchFunc search) -> int64_t {
	*left = 0; 
	if (points.count < 1) return default_value;
	if (points.count == 1) return points.data[0].y; 
	auto search_beg = points.data + search_beg_index;
	auto search_end = points.data + points.count;
	const auto pos = search(search_beg, search_end); 
	if (pos == search_beg) {
		// The step to the right is the first one
		return default_value;
	} 
	*left = int(std::distance<const blink_IntPoint*>(points.data, (pos - 1))); 
	if (pos == search_end) {
		// Nothing to the right so we're at the end
		return default_value;
	} 
	// We're somewhere in between two bool transitions.
	// Return the step on the left
	return (pos - 1)->y;
}

// Use a binary search to locate the block position
[[nodiscard]] inline
auto step_binary(const blink_IntPoints& points, int64_t default_value, blink_Position block_position, size_t search_beg_index, size_t* left) -> int64_t {
	const auto find = [block_position](const blink_IntPoint* beg, const blink_IntPoint* end) {
		const auto less = [](blink_Position position, blink_IntPoint point) {
			return position < point.x;
		}; 
		return std::upper_bound(beg, end, block_position, less);
	}; 
	return step(points, default_value, block_position, search_beg_index, left, find);
}

// Use a forward search to locate the block position (can be
// faster when block is being traversed forwards)
[[nodiscard]] inline
auto step_forward(const blink_IntPoints& points, int64_t default_value, blink_Position block_position, size_t search_beg_index, size_t* left) -> int64_t {
	const auto find = [block_position](const blink_IntPoint* beg, const blink_IntPoint* end) {
		const auto greater = [block_position](blink_IntPoint point) {
			return point.x > block_position;
		}; 
		return std::find_if(beg, end, greater);
	}; 
	return step(points, default_value, block_position, search_beg_index, left, find);
}

template <typename T, typename U, typename Searcher>
auto vec(const U& data, const BlockPositions& block_positions, int n, Searcher searcher, T* out) -> void {
	size_t left = 0;
	bool reset = false;
	auto prev_pos = block_positions.prev_pos; 
	for (int i = 0; i < n; i++) {
		const auto pos = block_positions.positions[i]; 
		if (pos < prev_pos) {
			// This occurs when Blockhead loops back to an earlier song position.
			// We perform a binary search to get back on track
			reset = true;
		} 
		if (reset) {
			reset = false; 
			out[i] = searcher.binary(data, block_positions.positions[i], 0, &left);
		}
		else {
			out[i] = searcher.forward(data, block_positions.positions[i], left, &left);
		} 
		prev_pos = pos;
	}
} 

template <typename T, typename U, typename Searcher>
auto vec(const U& data, const BlockPositions& block_positions, Searcher searcher, T* out) -> void {
	vec(data, block_positions, block_positions.count, searcher, out);
} 

[[nodiscard]] inline
auto vec(const blink_ChordData& data, const BlockPositions& block_positions) -> ml::DSPVectorInt {
	ml::DSPVectorInt out;
	std::array<blink_Scale, kFloatsPerDSPVector> buffer; 
	vec(data, block_positions, make_searcher(chord_binary, chord_forward), buffer.data()); 
	for (int i = 0; i < kFloatsPerDSPVector; i++) {
		out[i] = buffer[i];
	} 
	return out;
} 

[[nodiscard]] inline
auto vec(float default_value, const blink_RealPoints& data, const BlockPositions& block_positions) -> ml::DSPVector {
	ml::DSPVector out; 
	auto binary_search = [default_value](const blink_RealPoints& data, blink_Position position, size_t search_beg_index, size_t* left) -> float {
		return float_points_binary(data, default_value, position, search_beg_index, left);
	};
	auto forward_search = [default_value](const blink_RealPoints& data, blink_Position position, size_t search_beg_index, size_t* left) -> float {
		return float_points_forward(data, default_value, position, search_beg_index, left);
	};
	vec(data, block_positions, make_searcher(binary_search, forward_search), out.getBuffer());
	return out;
} 

[[nodiscard]] inline
auto vec(int64_t default_value, const blink_IntPoints& data, const BlockPositions& block_positions) -> ml::DSPVectorInt {
	ml::DSPVectorInt out; 
	auto binary_search = [default_value](const blink_IntPoints& data, blink_Position position, size_t search_beg_index, size_t* left) -> int64_t {
		return step_binary(data, default_value, position, search_beg_index, left);
	};
	auto forward_search = [default_value](const blink_IntPoints& data, blink_Position position, size_t search_beg_index, size_t* left) -> int64_t {
		return step_forward(data, default_value, position, search_beg_index, left);
	};
	std::array<int64_t, kFloatsPerDSPVector> buffer; 
	vec(data, block_positions, make_searcher(binary_search, forward_search), buffer.data());
	for (int i = 0; i < kFloatsPerDSPVector; i++) {
		out[i] = static_cast<int>(buffer[i]);
	} 
	return out;
} 

[[nodiscard]] inline
auto one(const blink_IntPoints& points, int64_t default_value, blink_Position block_position) -> int64_t {
	if (points.count == 0) { return default_value; }
	if (points.count == 1) { return points.data[0].y; }
	size_t dummy;
	return step_binary(points, default_value, block_position, 0, &dummy);
}

[[nodiscard]] inline
auto one(const blink_RealPoints& points, float default_value, blink_Position block_position) -> float {
	if (points.count == 0) { return default_value; }
	if (points.count == 1) { return points.data[0].y; }
	size_t dummy;
	return float_points_binary(points, default_value, block_position, 0, &dummy);
}

[[nodiscard]] inline
auto one(const blink::EnvData& env_data, blink_Position block_position) -> float {
	if (!env_data.data) { return env_data.value; }
	return one(env_data.data->points, env_data.default_value, block_position);
}

[[nodiscard]] inline
auto one(const blink::EnvData& env_data, const BlockPositions& block_positions) -> float {
	return one(env_data, block_positions.positions[0]);
}

[[nodiscard]] inline
auto one(const blink::SliderRealData& slider_data, blink_Position block_position) -> float {
	if (!slider_data.data) { return slider_data.value; }
	return one(slider_data.data->points, slider_data.default_value, block_position);
}

[[nodiscard]] inline
auto one(const blink::SliderRealData& slider_data, const BlockPositions& block_positions) -> float {
	return one(slider_data, block_positions.positions[0]);
}

[[nodiscard]] inline
auto vec(const blink::EnvData& env_data, const BlockPositions& block_positions) -> ml::DSPVector {
	if (!env_data.data)                   { return env_data.value; }
	if (env_data.data->points.count <= 1) { return env_data.value; }
	return vec(env_data.default_value, env_data.data->points, block_positions);
}

[[nodiscard]] inline
auto vec(const blink::SliderRealData& slider_data, const BlockPositions& block_positions) -> ml::DSPVector {
	if (!slider_data.data)                   { return slider_data.value; }
	if (slider_data.data->points.count <= 1) { return slider_data.value; }
	return vec(slider_data.default_value, slider_data.data->points, block_positions);
}

} // search
} // blink