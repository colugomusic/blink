#pragma once

#include "envelope_spec.hpp"
#include "envelope_range.hpp"
#include "envelope_snap_settings.hpp"
#include "parameter.hpp"
#include "math.hpp"

namespace blkhdgen {

class EnvelopeParameter : public Parameter
{
public:

	EnvelopeParameter(EnvelopeSpec spec);

	blkhdgen_ParameterType get_type() const override { return blkhdgen_ParameterType_Envelope; }

	float transform(float value) const;
	float inverse_transform(float value) const;
	const char* display_value(float value) const;

	blkhdgen_Index add_point(blkhdgen_IntPosition position, float value);
	blkhdgen_Error remove_point(blkhdgen_Index index);
	blkhdgen_Error move_point(blkhdgen_Index index, blkhdgen_IntPosition new_position, float new_value);
	blkhdgen_Error clear();
	blkhdgen_Error set_point_curve(blkhdgen_Index index, float curve);

	EnvelopeRange& range();
	const EnvelopeSnapSettings& snap_settings() const;

private:

	EnvelopeRange range_;
	EnvelopeSnapSettings snap_settings_;
	std::function<float(float)> transform_;
	std::function<float(float)> inverse_transform_;
	std::function<std::string(float)> display_value_;
	mutable std::string display_value_buffer_;
};

EnvelopeParameter::EnvelopeParameter(EnvelopeSpec spec)
	: Parameter(spec.uuid, spec.group_id, spec.name, spec.flags)
	, range_(spec.range)
	, snap_settings_{ spec.step_size, spec.default_snap_amount }
	, transform_(spec.transform)
	, inverse_transform_(spec.inverse_transform)
	, display_value_(spec.display_value)
{
}

float EnvelopeParameter::transform(float value) const
{
	return math::inverse_lerp(transform_(range_.min().get()), transform_(range_.max().get()), transform_(value));
}

float EnvelopeParameter::inverse_transform(float value) const
{
	return inverse_transform_(math::lerp(transform_(range_.min().get()), transform_(range_.max().get()), value));
}

const char* EnvelopeParameter::display_value(float value) const
{
	return (display_value_buffer_ = display_value_(value)).c_str();
}

EnvelopeRange& EnvelopeParameter::range()
{
	return range_;
}

const EnvelopeSnapSettings& EnvelopeParameter::snap_settings() const
{
	return snap_settings_;
}

}