#pragma once

#include "blink/data.hpp"
#include "blink/traverser.hpp"
#include <algorithm>

namespace blink {
namespace transform {
namespace calculators {

template <class T> T quadratic_formula(T a, T b, T c, T n) {
	return (a * (n * n)) + (b * n) + c;
}

template <class T> T quadratic_formula_inverse(T a, T b, T c, T y) {
	assert (a != T(0));
	return (-b + std::sqrt(b * b - 4 * a * (c - y))) / (2 * a);
}

template <class T> T spooky_maths(T f0, T f1, T N, T n, T C) {
	auto accel = (f1 - f0) / (T(2) * N); 
	return quadratic_formula(accel, f0, C, n);
}

//template <class T> T spooky_maths_inverse(T f0, T f1, T N, T n, T C) {
	//auto accel = (f1 - f0) / (T(2) * N);
	//if (accel == T(0)) {
		//return (n - C) / b;
	//}
	//return quadratic_formula_inverse(accel, f0, C, n);
//}

class SpeedUnit {
public: 
	struct Config {
		float speed { 0.0f };
		const blink_EnvData* env_speed { nullptr };
	}; 
	// We use this for both sample playback and waveform generation. This
	// calculation needs to be fast, preferably O(n) or better.
	//
	// If we are calculating waveforms the start of the waveform might be
	// millions of pixels off the left edge of the screen therefore it is
	// not good enough to simply traverse the entire sample.
	//
	blink_Position operator()(Config config, blink_Position block_position, float* derivative = nullptr) {
		struct FFPoint {
			blink_Position x;
			float y;
			double ff; 
			FFPoint(const blink_RealPoint& p, float min, float max, float speed)
				: x(p.x)
				, y(p.y)
				, ff(double(std::clamp(p.y, min, max)) * speed)
			{
			}
		}; 
		for (size_t i = point_search_index_; i < config.env_speed->points.count; i++) {
			const FFPoint p1(config.env_speed->points.data[i], config.env_speed->points.min, config.env_speed->points.max, config.speed); 
			if (block_position < p1.x) {
				if (i == 0) {
					point_search_index_ = 0; 
					if (derivative) *derivative = float(p1.ff); 
					return spooky_maths(p1.ff, p1.ff, 1.0, block_position, double(segment_start_));
				} 
				FFPoint p0(config.env_speed->points.data[i - 1], config.env_speed->points.min, config.env_speed->points.max, config.speed); 
				auto n = block_position - p0.x;
				auto segment_size = p1.x - p0.x; 
				if (segment_size > 0.0f) {
					point_search_index_ = i; 
					if (derivative) *derivative = float(math::lerp(p0.ff, p1.ff, n / segment_size)); 
					const auto f0 = p0.ff;
					const auto f1 = p1.ff; 
					return spooky_maths(f0, f1, segment_size, double(n), double(segment_start_));
				}
			}
			else {
				if (i == 0) {
					point_search_index_ = 1; 
					const auto f1 = p1.ff; 
					segment_start_ = spooky_maths(f1, f1, 1.0, p1.x, segment_start_);
				}
				else {
					point_search_index_ = i + 1; 
					FFPoint p0(config.env_speed->points.data[i - 1], config.env_speed->points.min, config.env_speed->points.max, config.speed); 
					auto segment_size = p1.x - p0.x; 
					if (segment_size > 0.0f) {
						if (p0.y > 0.f || p1.y > 0.f) {
							auto f0 = p0.ff;
							auto f1 = p1.ff; 
							segment_start_ = float(spooky_maths(f0, f1, segment_size, segment_size, segment_start_));
						}
					}
				}
			}
		} 
		FFPoint p0(config.env_speed->points.data[config.env_speed->points.count - 1], config.env_speed->points.min, config.env_speed->points.max, config.speed); 
		auto n = block_position - p0.x; 
		if (derivative) *derivative = float(p0.ff); 
		if (p0.y == 0.0f) {
			return segment_start_;
		} 
		return float(spooky_maths(p0.ff, p0.ff, 1.0, double(n), double(segment_start_)));
	}
	void reset() {
		segment_start_      = 0.0;
		point_search_index_ = 0;
	} 
private: 
	blink_Position segment_start_ = 0.0;
	size_t point_search_index_    = 0;
};

class Speed {
public: 
	struct Config {
		uint64_t unit_state_id;
		float speed;
		const blink_EnvData* env_speed; 
		struct {
			BlockPositions* positions;
			ml::DSPVector* derivatives;
		} outputs;
	}; 
	void operator()(Config config, const BlockPositions& block_positions, int count) {
		if (!config.env_speed || config.env_speed->points.count < 1) {
			const auto get_env_speed { [&config]() {
				if (!config.env_speed) return 0.0f; 
				const auto min { config.env_speed->points.min };
				const auto max { config.env_speed->points.max }; 
				return std::clamp(1.0f, min, max);
			}}; 
			const auto ff { get_env_speed() * config.speed }; 
			*config.outputs.positions = block_positions.positions * ff; 
			if (config.outputs.derivatives) {
				*config.outputs.derivatives = ff;
			} 
			return;
		} 
		traverser_.generate(config.unit_state_id, block_positions, count); 
		const auto& resets { traverser_.get_resets() }; 
		SpeedUnit::Config unit_config; 
		unit_config.speed = config.speed;
		unit_config.env_speed = config.env_speed; 
		config.outputs.positions->rotate_prev_pos(); 
		for (int i = 0; i < count; i++) {
			if (resets[i] > 0) {
				unit_calculator_.reset();
			} 
			const auto out_derivative { config.outputs.derivatives ? &config.outputs.derivatives->getBuffer()[i] : nullptr };
			const auto position { unit_calculator_( unit_config, block_positions.positions[i], out_derivative) }; 
			config.outputs.positions->positions.set(i, position);
		}
	} 
private: 
	SpeedUnit unit_calculator_;
	Traverser traverser_;
};

} // calculators
} // transform
} // blink
