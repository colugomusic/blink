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

	float get_default_value() const;
	float transform(float value) const;
	float inverse_transform(float value) const;
	const char* display_value(float value) const;

	blkhdgen_Index add_point(blkhdgen_EnvelopePoint point);
	blkhdgen_Error remove_point(blkhdgen_Index index);
	blkhdgen_Error move_point(blkhdgen_Index index, blkhdgen_EnvelopePointPosition new_position);
	blkhdgen_Error clear();
	blkhdgen_Error set_points(blkhdgen_Index count, blkhdgen_EnvelopePoint* points);
	blkhdgen_Error set_point_curve(blkhdgen_Index index, float curve);

	EnvelopeRange& range();
	const EnvelopeSnapSettings& snap_settings() const;

private:

	EnvelopeRange range_;
	EnvelopeSnapSettings snap_settings_;
	float default_value_;
	std::function<float(float)> transform_;
	std::function<float(float)> inverse_transform_;
	std::function<std::string(float)> display_value_;
	mutable std::string display_value_buffer_;
	std::vector<blkhdgen_EnvelopePoint> points_;
};

EnvelopeParameter::EnvelopeParameter(EnvelopeSpec spec)
	: Parameter(spec)
	, range_(spec.range)
	, snap_settings_{ spec.step_size, spec.default_snap_amount }
	, default_value_(spec.default_value)
	, transform_(spec.transform)
	, inverse_transform_(spec.inverse_transform)
	, display_value_(spec.display_value)
{
}

float EnvelopeParameter::get_default_value() const
{
	return default_value_;
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

blkhdgen_Index EnvelopeParameter::add_point(blkhdgen_EnvelopePoint point)
{
	points_.insert(std::lower_bound(points_.begin(), points_.end(), point, [](blkhdgen_EnvelopePoint a, blkhdgen_EnvelopePoint b) { return a.position.x < a.position.y; }), point);
}

blkhdgen_Error EnvelopeParameter::remove_point(blkhdgen_Index index)
{

}

blkhdgen_Error EnvelopeParameter::move_point(blkhdgen_Index index, blkhdgen_EnvelopePointPosition new_position)
{

}

blkhdgen_Error EnvelopeParameter::clear()
{

}

blkhdgen_Error set_points(blkhdgen_Index count, blkhdgen_EnvelopePoint* points)
{

}

blkhdgen_Error EnvelopeParameter::set_point_curve(blkhdgen_Index index, float curve)
{
	return BLKHDGEN_OK;
}


}