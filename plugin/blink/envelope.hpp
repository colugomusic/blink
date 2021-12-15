#pragma once

#include "block_positions.hpp"
#include "envelope_spec.hpp"
#include "envelope_range.hpp"
#include "envelope_snap_settings.hpp"
#include <blink/math.hpp>

namespace blink {

class Envelope
{
public:

	Envelope(EnvelopeSpec spec);

	float get_default_value() const { return spec_.default_value; }
	const char* display_value(float value) const;
	const auto& value_slider() const { return value_slider_; }
	float stepify(float value) const { return spec_.stepify ? spec_.stepify(value) : value; }

	float snap_value(float value, float step_size, float snap_amount) const
	{
		return spec_.snap_value ? spec_.snap_value(value, step_size, snap_amount) : stepify(value);
	}

	std::optional<float> get_gridline(int index) const;
	std::optional<float> get_stepline(int index, float step_size) const;

	float search(const blink_EnvelopeData* data, blink_Position block_position) const;
	void search_vec(const blink_EnvelopeData* data, const BlockPositions& block_positions, int n, float* out) const;
	void search_vec(const blink_EnvelopeData* data, const BlockPositions& block_positions, float* out) const;
	ml::DSPVector search_vec(const blink_EnvelopeData* data, const BlockPositions& block_positions) const;

	EnvelopeRange& range() { return range_; }
	const EnvelopeRange& range() const { return range_; }
	const EnvelopeSnapSettings& snap_settings() const { return snap_settings_; }

private:

	EnvelopeSpec spec_;
	EnvelopeRange range_;
	Slider<float> value_slider_;
	EnvelopeSnapSettings snap_settings_;
	mutable std::string display_value_buffer_;
};

inline std::optional<float> Envelope::get_gridline(int index) const
{
	if (!spec_.get_gridline) return std::optional<float>();

	return spec_.get_gridline(index);
}

inline std::optional<float> Envelope::get_stepline(int index, float step_size) const
{
	if (!spec_.get_stepline) return std::optional<float>();

	return spec_.get_stepline(index, step_size);
}

inline float Envelope::search(const blink_EnvelopeData* data, blink_Position block_position) const
{
	int left;

	return spec_.search_binary(data, spec_.default_value, block_position, 0, &left);
}

inline void Envelope::search_vec(const blink_EnvelopeData* data, const BlockPositions& block_positions, float* out) const
{
	search_vec(data, block_positions, block_positions.count, out);
}

inline void Envelope::search_vec(const blink_EnvelopeData* data, const BlockPositions& block_positions, int n, float* out) const
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

			out[i] = spec_.search_binary(data, spec_.default_value, block_positions.positions[i], 0, &left);
		}
		else
		{
			out[i] = spec_.search_forward(data, spec_.default_value, block_positions.positions[i], left, &left);
		}

		prev_pos = pos;
	}
}

inline ml::DSPVector Envelope::search_vec(const blink_EnvelopeData* data, const BlockPositions& block_positions) const
{
	ml::DSPVector out;

	search_vec(data, block_positions, out.getBuffer());

	return out;
}

inline Envelope::Envelope(EnvelopeSpec spec)
	: spec_(spec)
	, range_(spec.range)
	, snap_settings_{ spec.step_size, spec.default_snap_amount }
	, value_slider_(spec.value_slider)
{
}

inline const char* Envelope::display_value(float value) const
{
	return (display_value_buffer_ = spec_.to_string(value)).c_str();
}

}