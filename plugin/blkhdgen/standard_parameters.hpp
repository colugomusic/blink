#pragma once

#include "envelope_spec.hpp"

namespace blkhdgen {
namespace std_params {

inline EnvelopeSpec amp()
{
	EnvelopeSpec out;

	out.uuid = BLKHDGEN_STDPARAM_AMP;
	out.name = "Amp";

	out.normalize = [](float min, float max, float v)
	{
		return math::inverse_lerp(math::db_to_af(min), math::db_to_af(max), math::db_to_af(v));
	};

	out.inverse_normalize = [](float min, float max, float v)
	{
		return math::af_to_db(math::lerp(math::db_to_af(min), math::db_to_af(max), v));
	};

	out.range.min_range.min = -60.0f;
	out.range.min_range.max = -60.0f;
	out.range.max_range.min = -60.0f;
	out.range.max_range.max = 12.0f;
	out.range.min_default_value = -60.0f;
	out.range.max_default_value = 0.0f;
	out.range.step_size = 1.0f;

	return out;
}

inline EnvelopeSpec pan()
{
	EnvelopeSpec out;

	out.uuid = BLKHDGEN_STDPARAM_PAN;
	out.name = "Pan";

	out.normalize = [](float min, float max, float v)
	{
		return v;
	};

	out.inverse_normalize = [](float min, float max, float v)
	{
		return v;
	};

	out.range.min_range.min = -1.0f;
	out.range.min_range.max = -1.0f;
	out.range.max_range.min = 1.0f;
	out.range.max_range.max = 1.0f;
	out.range.min_default_value = -1.0f;
	out.range.max_default_value = 1.0f;
	out.range.step_size = 0.0f;

	return out;
}

inline EnvelopeSpec pitch()
{
	EnvelopeSpec out;

	out.uuid = BLKHDGEN_STDPARAM_PITCH;
	out.name = "Pitch";

	out.normalize = [](float min, float max, float v)
	{
		return math::inverse_lerp(min, max, v);
	};

	out.inverse_normalize = [](float min, float max, float v)
	{
		return math::lerp(min, max, v);
	};

	out.range.min_range.min = -60.0f;
	out.range.min_range.max = 60.0f;
	out.range.max_range.min = -60.0f;
	out.range.max_range.max = 60.0f;
	out.range.min_default_value = -24.0f;
	out.range.max_default_value = 24.0f;
	out.range.step_size = 1.0f;

	return out;
}


}}