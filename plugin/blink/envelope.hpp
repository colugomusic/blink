#pragma once

#include "block_positions.hpp"
#include "envelope_spec.hpp"
#include "envelope_range.hpp"
#include "envelope_snap_settings.hpp"
#include <blink/math.hpp>
#include <blink/searcher.hpp>

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

	const auto& search() const { return searcher_; }

	EnvelopeRange& range() { return range_; }
	const EnvelopeRange& range() const { return range_; }
	const EnvelopeSnapSettings& snap_settings() const { return snap_settings_; }

private:

	EnvelopeSpec spec_;
	EnvelopeRange range_;
	Slider<float> value_slider_;
	EnvelopeSnapSettings snap_settings_;
	FloatSearcher searcher_;
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

inline Envelope::Envelope(EnvelopeSpec spec)
	: spec_(spec)
	, range_(spec.range)
	, snap_settings_{ spec.step_size, spec.default_snap_amount }
	, value_slider_(spec.value_slider)
	, searcher_(spec.searcher, spec.default_value)
{
}

inline const char* Envelope::display_value(float value) const
{
	return (display_value_buffer_ = spec_.to_string(value)).c_str();
}

}