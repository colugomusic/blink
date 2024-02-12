#pragma once

#include "blink/envelope_data.hpp"
#include "blink/traverser.hpp"

namespace blink {
namespace transform {
namespace calculators {

template <class T> T ratio(T min, T max, T distance) {
	return std::pow(T(2), ((max - min) / distance) / T(12));
}

template <class T> T sum_of_geometric_series(T first, T ratio, T n) {
	return first * ((T(1) - std::pow(ratio, n)) / (T(1) - ratio));
}

template <class T> T sum_of_geometric_series_inverse(T first, T ratio, T sum) {
	return std::log(T(1) - sum * (T(1) - ratio) / first) / std::log(ratio);
}

template <class T> T weird_math(T min, T max, T distance, T n) {
	const auto r      = ratio(min, max, distance); 
	const auto ff_min = math::convert::p_to_ff(min);
	if (std::abs(T(1) - r) <= T(0)) {
		return n * ff_min;
	} 
	return sum_of_geometric_series(ff_min, r, n);
}

template <class T> T weird_math_inverse(T min, T max, T distance, T n) {
	const auto r      = ratio(min, max, distance); 
	const auto ff_min = math::convert::p_to_ff(min);
	if (std::abs(T(1) - r) <= T(0)) {
		return n / ff_min;
	} 
	return sum_of_geometric_series_inverse(ff_min, r, n);
}

template <class T> T weird_math_ff(T min, T max, T distance, T n) {
	return math::convert::p_to_ff(min) * std::pow(ratio(min, max, distance), n);
}

struct PitchPoint {
	blink_Position x;
	float pitch; 
	float ff;
}; 

inline
auto make_pitch_point(const blink_RealPoint& p, float min, float max, float transpose) {
	PitchPoint pp;
	pp.x     = p.x;
	pp.pitch = std::clamp(p.y, min, max) + transpose;
	pp.ff    = math::convert::p_to_ff(pp.pitch);
	return pp;
}

struct PitchUnit {
	struct Config {
		float transpose                 = 0.0f;
		const blink_EnvelopeData* pitch = nullptr;
	};
	// We use this for both sample playback and waveform generation. This
	// calculation needs to be fast, preferably O(n) or better.
	//
	// If we are calculating waveforms the start of the waveform might be
	// millions of pixels off the left edge of the screen therefore it is
	// not good enough to simply traverse the entire sample.
	//
	blink_Position xform(Config config, blink_Position block_position, float* derivative = nullptr) {
		for (blink_Index i = point_search_index_; i < config.pitch->points.count; i++) {
			const auto p1 = make_pitch_point(config.pitch->points.data[i], config.pitch->points.min, config.pitch->points.max, config.transpose);
			if (block_position < p1.x) {
				if (i == 0) {
					point_search_index_ = 0; 
					if (derivative) {
						*derivative = p1.ff;
					}
					return (block_position * p1.ff) + segment_start_;
				} 
				const auto p0     = make_pitch_point(config.pitch->points.data[i - 1], config.pitch->points.min, config.pitch->points.max, config.transpose); 
				auto n            = block_position - p0.x;
				auto segment_size = double(p1.x) - p0.x;
				if (segment_size > 0.0f) {
					point_search_index_ = i; 
					if (derivative) {
						*derivative = float(weird_math_ff(double(p0.pitch), double(p1.pitch), segment_size, n)); 
					}
					return (weird_math(double(p0.pitch), double(p1.pitch), segment_size, n)) + segment_start_;
				}
			}
			else {
				if (i == 0) {
					point_search_index_ = 1; 
					segment_start_ += (p1.x * p1.ff) + segment_start_;
				}
				else {
					point_search_index_ = i + 1; 
					const auto p0 = make_pitch_point(config.pitch->points.data[i - 1], config.pitch->points.min, config.pitch->points.max, config.transpose); 
					auto segment_size { double(p1.x) - p0.x }; 
					if (segment_size > 0.0f) {
						segment_start_ = (weird_math(double(p0.pitch), double(p1.pitch), segment_size, segment_size)) + segment_start_;
					}
				}
			}
		}
		const auto p0 = make_pitch_point(config.pitch->points.data[config.pitch->points.count - 1], config.pitch->points.min, config.pitch->points.max, config.transpose); 
		auto n        = block_position - p0.x;
		if (derivative) {
			*derivative = p0.ff; 
		}
		return (n * p0.ff) + segment_start_;
	} 
	void reset() {
		segment_start_      = 0.0f;
		point_search_index_ = 0;
	} 
private: 
	blink_Position segment_start_ = 0.0f;
	int point_search_index_       = 0;
};

struct Pitch {
	struct Config {
		uint64_t unit_state_id;
		float transpose;
		const blink_EnvelopeData* pitch; 
		struct {
			BlockPositions* positions;
			ml::DSPVector* derivatives;
		} outputs;
	};
	void xform(Config config, const BlockPositions& block_positions, int count) {
		if (!config.pitch || config.pitch->points.count < 1) {
			const auto get_pitch = [&config]() {
				if (!config.pitch) {
					return 0.0f; 
				}
				const auto min = config.pitch->points.min;
				const auto max = config.pitch->points.max;
				return std::clamp(0.0f, min, max);
			};
			const auto ff             = math::convert::p_to_ff(get_pitch() + config.transpose);
			*config.outputs.positions = block_positions.positions * ff;
			if (config.outputs.derivatives) {
				*config.outputs.derivatives = ff;
			} 
			return;
		} 
		traverser_.generate(config.unit_state_id, block_positions, count); 
		const auto& resets = traverser_.get_resets();
		PitchUnit::Config unit_config;
		unit_config.transpose = config.transpose;
		unit_config.pitch     = config.pitch;
		config.outputs.positions->rotate_prev_pos();
		for (int i = 0; i < count; i++) {
			if (resets[i] > 0) {
				unit_calculator_.reset();
			}
			const auto out_derivative = config.outputs.derivatives ? &config.outputs.derivatives->getBuffer()[i] : nullptr;
			const auto position       = unit_calculator_.xform(unit_config, block_positions.positions[i], out_derivative);
			config.outputs.positions->positions.set(i, position);
		}
	}
private:
	PitchUnit unit_calculator_;
	Traverser traverser_;
};

} // calculators
} // transform
} // blink