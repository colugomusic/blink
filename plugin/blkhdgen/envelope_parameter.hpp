#pragma once

#include "envelope_spec.hpp"
#include "envelope_range.hpp"
#include "envelope_search.hpp"
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
	float curve(float value) const;
	float inverse_curve(float value) const;
	const char* display_value(float value) const;
	int get_flags() const;

	float get_mod_value(blkhdgen_Position block_position) const;
	blkhdgen_Error set_get_point_data_cb(void* user, blkhdgen_GetPointDataCB cb);

	EnvelopeRange& range();
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
	std::function<blkhdgen_EnvelopePoints*(void)> get_point_data_;
	mutable blkhdgen_EnvelopePoints* last_point_data_ = nullptr;
	mutable int point_search_index_ = -1;
};

EnvelopeParameter::EnvelopeParameter(EnvelopeSpec spec)
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

float EnvelopeParameter::get_default_value() const
{
	return default_value_;
}

int EnvelopeParameter::get_flags() const
{
	return flags_;
}

float EnvelopeParameter::curve(float value) const
{
	return curve_(value);
}

float EnvelopeParameter::inverse_curve(float value) const
{
	return inverse_curve_(value);
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

float EnvelopeParameter::get_mod_value(blkhdgen_Position block_position) const
{
	//const auto points = get_point_data_();

	//if (!points) return default_value_;
	//if (points->count < 1) return default_value_;
	//if (points->count == 1) return points->points[0].position.y;

	//if (points != last_point_data_)
	//{
	//	last_point_data_ = points;
	//	point_search_index_ = -1;
	//}

	//if (point_search_index_)
	//{

	//}
	//
	//// TODO: implement this
	return 0.0;
}

blkhdgen_Error EnvelopeParameter::set_get_point_data_cb(void* user, blkhdgen_GetPointDataCB cb)
{
	get_point_data_ = [user, cb]()
	{
		return cb(user);
	};

	return BLKHDGEN_OK;
}

}