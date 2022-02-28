#pragma once

#include "blink/envelope_data.hpp"
#include "blink/traverser.hpp"

namespace blink {
namespace transform {
namespace calculators {

template <class T> T ratio(T min, T max, T distance)
{
	return std::pow(T(2), ((max - min) / distance) / T(12));
}

template <class T> T sum_of_geometric_series(T first, T ratio, T n)
{
	return first * ((T(1) - std::pow(ratio, n)) / (T(1) - ratio));
}

//
// pull requests extremely welcome
//

template <class T> T weird_math_that_i_dont_understand(T min, T max, T distance, T n)
{
	auto r = ratio(min, max, distance);

	if (std::abs(T(1) - r) <= T(0))
	{
		return n * math::convert::p_to_ff(min);
	}

	return sum_of_geometric_series(math::convert::p_to_ff(min), r, n);
}

template <class T> T weird_math_that_i_dont_understand_ff(T min, T max, T distance, T n)
{
	return math::convert::p_to_ff(min) * std::pow(ratio(min, max, distance), n);
}

class PitchUnit
{
public:

	struct Config
	{
		float transpose { 0.0f };
		const blink_EnvelopeData* pitch { nullptr };
	};

	// We use this for both sample playback and waveform generation. This
	// calculation needs to be fast, preferably O(n) or better.
	//
	// If we are calculating waveforms the start of the waveform might be
	// millions of pixels off the left edge of the screen therefore it is
	// not good enough to simply traverse the entire sample.
	//
	// blink_EnvelopePoint has an unused 'curve' member which could be
	// used to represent an ease-in/ease-out curve from one envelope point
	// to the next. This is not implemented because I do not understand
	// the mathematics involved in calculating the resulting sample
	// position.
	//
	blink_Position operator()(Config config, blink_Position block_position, float* derivative = nullptr)
	{
		struct PitchPoint
		{
			blink_IntPosition x;
			float pitch;

			PitchPoint(const blink_FloatPoint& p, float min, float max, float transpose)
				: x(p.x)
				, pitch(std::clamp(p.y, min, max) + transpose)
				, ff_(1.0f)
			{
			}

			PitchPoint(PitchPoint && rhs) = default;

			float get_ff() const
			{
				if (!ff_ready_)
				{
					ff_ = math::convert::p_to_ff(pitch);
					ff_ready_ = true;
				}

				return ff_;
			}

		private:

			mutable bool ff_ready_ = false;
			mutable float ff_;
		};

		for (blink_Index i { point_search_index_ }; i < config.pitch->points.count; i++)
		{
			const PitchPoint p1(config.pitch->points.data[i], config.pitch->points.min, config.pitch->points.max, config.transpose);

			if (block_position < p1.x)
			{
				if (i == 0)
				{
					point_search_index_ = 0;

					if (derivative) *derivative = p1.get_ff();

					return (block_position * p1.get_ff()) + segment_start_;
				}

				PitchPoint p0(config.pitch->points.data[i - 1], config.pitch->points.min, config.pitch->points.max, config.transpose);

				auto n { block_position - p0.x };
				auto segment_size { double(p1.x) - p0.x };

				if (segment_size > 0.0f)
				{
					point_search_index_ = i;

					if (derivative) *derivative = float(weird_math_that_i_dont_understand_ff(double(p0.pitch), double(p1.pitch), segment_size, n));

					return (weird_math_that_i_dont_understand(double(p0.pitch), double(p1.pitch), segment_size, n)) + segment_start_;
				}
			}
			else
			{
				if (i == 0)
				{
					point_search_index_ = 1;

					segment_start_ += (p1.x * p1.get_ff()) + segment_start_;
				}
				else
				{
					point_search_index_ = i + 1;

					PitchPoint p0(config.pitch->points.data[i - 1], config.pitch->points.min, config.pitch->points.max, config.transpose);

					auto segment_size { double(p1.x) - p0.x };

					if (segment_size > 0.0f)
					{
						segment_start_ = (weird_math_that_i_dont_understand(double(p0.pitch), double(p1.pitch), segment_size, segment_size)) + segment_start_;
					}
				}
			}
		}

		PitchPoint p0(config.pitch->points.data[config.pitch->points.count - 1], config.pitch->points.min, config.pitch->points.max, config.transpose);

		auto n { block_position - p0.x };

		if (derivative) *derivative = p0.get_ff();

		return (n * p0.get_ff()) + segment_start_;
	}

	void reset()
	{
		segment_start_ = 0.0f;
		point_search_index_ = 0;
	}

private:

	blink_Position segment_start_ { 0.0f };
	int point_search_index_ { 0 };
};

class Pitch
{
public:

	struct Config
	{
		uint64_t unit_state_id;
		float transpose;
		const blink_EnvelopeData* pitch;

		struct
		{
			BlockPositions* positions;
			ml::DSPVector* derivatives;
		} outputs;
	};

	void operator()(Config config, const BlockPositions& block_positions, int count)
	{
		if (!config.pitch || config.pitch->points.count < 1)
		{
			const auto get_pitch { [&config]()
			{
				if (!config.pitch) return 0.0f;
				
				const auto min { config.pitch->points.min };
				const auto max { config.pitch->points.max };

				return std::clamp(0.0f, min, max);
			}};

			const auto ff { math::convert::p_to_ff(get_pitch() + config.transpose) };

			*config.outputs.positions = block_positions.positions * ff;

			if (config.outputs.derivatives)
			{
				*config.outputs.derivatives = ff;
			}

			return;
		}

		traverser_.generate(config.unit_state_id, block_positions, count);

		const auto& resets { traverser_.get_resets() };

		PitchUnit::Config unit_config;

		unit_config.transpose = config.transpose;
		unit_config.pitch = config.pitch;

		for (int i = 0; i < count; i++)
		{
			if (resets[i] > 0)
			{
				unit_calculator_.reset();
			}

			const auto out_derivative { config.outputs.derivatives ? &config.outputs.derivatives->getBuffer()[i] : nullptr };
			const auto position { unit_calculator_( unit_config, block_positions.positions[i], out_derivative) };

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