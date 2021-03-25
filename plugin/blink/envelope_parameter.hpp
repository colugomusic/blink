#pragma once

#include "envelope_spec.hpp"
#include "envelope_range.hpp"
#include "envelope_search.hpp"
#include "envelope_snap_settings.hpp"
#include "parameter.hpp"
#include "traverser.hpp"
#include "math.hpp"

namespace blink {

class EnvelopeParameter : public Parameter
{
public:

	EnvelopeParameter(EnvelopeSpec spec);

	blink_ParameterType get_type() const override { return blink_ParameterType_Envelope; }

	float get_default_value() const;
	const char* display_value(float value) const;
	int get_flags() const;

	ml::DSPVector get_mod_values(Traverser* traverser, const blink_EnvelopeData* data) const;
	float get_mod_value(Traverser* traverser, const blink_EnvelopeData* data) const;
	float get_mod_value(const blink_EnvelopeData* data, float position, bool reset) const;

	EnvelopeRange& range();
	const EnvelopeRange& range() const;
	const EnvelopeSnapSettings& snap_settings() const;

private:

	EnvelopeRange range_;
	EnvelopeSnapSettings snap_settings_;
	float default_value_;
	int flags_;
	std::function<std::string(float)> display_value_;
	mutable std::string display_value_buffer_;
	mutable TraverserResetter<blink_EnvelopeData> traverser_resetter_;
	mutable int point_search_index_ = -1;
};

inline EnvelopeParameter::EnvelopeParameter(EnvelopeSpec spec)
	: Parameter(spec)
	, range_(spec.range)
	, snap_settings_{ spec.step_size, spec.default_snap_amount }
	, default_value_(spec.default_value)
	, flags_(spec.flags)
	, display_value_(spec.display_value)
{
}

inline float EnvelopeParameter::get_default_value() const
{
	return default_value_;
}

inline int EnvelopeParameter::get_flags() const
{
	return flags_;
}

inline const char* EnvelopeParameter::display_value(float value) const
{
	return (display_value_buffer_ = display_value_(value)).c_str();
}

inline EnvelopeRange& EnvelopeParameter::range()
{
	return range_;
}

inline const EnvelopeRange& EnvelopeParameter::range() const
{
	return range_;
}

inline const EnvelopeSnapSettings& EnvelopeParameter::snap_settings() const
{
	return snap_settings_;
}

inline ml::DSPVector EnvelopeParameter::get_mod_values(Traverser* traverser, const blink_EnvelopeData* data) const
{
	if (!data) return default_value_;
	if (data->points.count < 1) return default_value_;

	traverser_resetter_.check(data, traverser);

	const auto& resets = traverser->get_resets();
	const auto& read_position = traverser->get_read_position();

	ml::DSPVector out;

	for (int i = 0; i < kFloatsPerDSPVector; i++)
	{
		out[i] = get_mod_value(data, read_position[i], resets[i] > 0);
	}
	
	return out;
}

inline float EnvelopeParameter::get_mod_value(Traverser* traverser, const blink_EnvelopeData* data) const
{
	if (!data) return default_value_;
	if (data->points.count < 1) return default_value_;

	traverser_resetter_.check(data, traverser);

	const auto& resets = traverser->get_resets();
	const auto& read_position = traverser->get_read_position();

	return get_mod_value(data, read_position[0], resets[0] > 0);
}

inline float EnvelopeParameter::get_mod_value(const blink_EnvelopeData* data, float position, bool reset) const
{
	if (reset) point_search_index_ = -1;

	const auto min = range_.min();
	const auto max = range_.max();

	const auto normalized_value = envelope_search(&data->points, position, &point_search_index_);

	return normalized_value;
	// math::transform(curve_, data->range.min, data->range.max, normalized_value);
	// TODO:
	// is this all dead code now?
	return 0.0f;// math::transform_and_denormalize(curve_, min, max, normalized_value);
}

}