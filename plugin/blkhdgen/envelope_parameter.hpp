#pragma once

#include "envelope_spec.hpp"
#include "envelope_range.hpp"
#include "envelope_search.hpp"
#include "envelope_snap_settings.hpp"
#include "parameter.hpp"
#include "traverser.hpp"
#include "math.hpp"

namespace blkhdgen {

class EnvelopeParameter : public Parameter
{
public:

	EnvelopeParameter(EnvelopeSpec spec);

	blkhdgen_ParameterType get_type() const override { return blkhdgen_ParameterType_Envelope; }

	float get_default_value() const;
	float curve(float value) const;
	float inverse_curve(float value) const;
	const char* display_value(float value) const;
	int get_flags() const;

	ml::DSPVector get_mod_values(Traverser* traverser, const blkhdgen_EnvelopePoints* points) const;
	float get_mod_value(Traverser* traverser, const blkhdgen_EnvelopePoints* points) const;
	float get_mod_value(const blkhdgen_EnvelopePoints* points, float position, bool reset) const;

	EnvelopeRange& range();
	const EnvelopeRange& range() const;
	const EnvelopeSnapSettings& snap_settings() const;

private:

	EnvelopeRange range_;
	EnvelopeSnapSettings snap_settings_;
	float default_value_;
	int flags_;
	std::function<float(float)> curve_;
	std::function<float(float)> inverse_curve_;
	std::function<std::string(float)> display_value_;
	mutable std::string display_value_buffer_;
	mutable TraverserPointDataResetter traverser_resetter_;
	mutable int point_search_index_ = -1;
};

inline EnvelopeParameter::EnvelopeParameter(EnvelopeSpec spec)
	: Parameter(spec)
	, range_(spec.range)
	, snap_settings_{ spec.step_size, spec.default_snap_amount }
	, default_value_(spec.default_value)
	, flags_(spec.flags)
	, curve_(spec.curve)
	, inverse_curve_(spec.inverse_curve)
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

inline float EnvelopeParameter::curve(float value) const
{
	return curve_(value);
}

inline float EnvelopeParameter::inverse_curve(float value) const
{
	return inverse_curve_(value);
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

inline ml::DSPVector EnvelopeParameter::get_mod_values(Traverser* traverser, const blkhdgen_EnvelopePoints* points) const
{
	if (!points) return curve_(default_value_);
	if (points->count < 1) return curve_(default_value_);

	traverser_resetter_.check(points, traverser);

	const auto& resets = traverser->get_resets();
	const auto& read_position = traverser->get_read_position();

	ml::DSPVector out;

	for (int i = 0; i < kFloatsPerDSPVector; i++)
	{
		out[i] = get_mod_value(points, read_position[i], resets[i] > 0);
	}
	
	return out;
}

inline float EnvelopeParameter::get_mod_value(Traverser* traverser, const blkhdgen_EnvelopePoints* points) const
{
	if (!points) return curve_(default_value_);
	if (points->count < 1) return curve_(default_value_);

	traverser_resetter_.check(points, traverser);

	const auto& resets = traverser->get_resets();
	const auto& read_position = traverser->get_read_position();

	return get_mod_value(points, read_position[0], resets[0] > 0);
}

inline float EnvelopeParameter::get_mod_value(const blkhdgen_EnvelopePoints* points, float position, bool reset) const
{
	if (reset) point_search_index_ = -1;

	const auto min = range_.min().get();
	const auto max = range_.max().get();

	const auto normalized_value = envelope_search(points, position, &point_search_index_);

	return math::transform_and_denormalize(curve_, min, max, normalized_value);
}

}