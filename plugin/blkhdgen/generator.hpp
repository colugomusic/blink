#pragma once

#include <map>
#include "envelope_spec.hpp"
#include "group.hpp"
#include "parameter.hpp"
#include "slider_spec.hpp"

namespace blkhdgen {

class Generator
{

public:

	virtual ~Generator() {}

	virtual const char* get_name() const = 0;
	virtual blkhdgen_Error process(const blkhdgen_Position* pos, float** out) = 0;
	virtual const char* get_error_string(blkhdgen_Error error) const = 0;

	int get_num_channels() const { return 2; }

	blkhdgen_Index add_warp_point(blkhdgen_IntPosition block_position);
	blkhdgen_Error remove_warp_point(blkhdgen_Index index);
	blkhdgen_Error move_warp_point(blkhdgen_Index index, blkhdgen_IntPosition new_position);
	blkhdgen_Error clear_warp_points();

	int get_num_groups() const;
	int get_num_parameters() const;

	Group& get_group(blkhdgen_Index index) const;
	Group& get_group_by_id(blkhdgen_ID id) const;
	Parameter& get_parameter(blkhdgen_Index index) const;
	Parameter& get_parameter_by_id(blkhdgen_UUID uuid) const;

	float get_mod_value(blkhdgen_Position block_position) const;
	blkhdgen_Position get_waveform_position(blkhdgen_Position block_position) const;

	blkhdgen_Error set_get_sample_info_cb(void* user, blkhdgen_GetSampleInfoCB cb);
	blkhdgen_Error set_get_sample_data_cb(void* user, blkhdgen_GetSampleDataCB cb);

protected:

	void add_group(blkhdgen_ID id, std::string name);
	void add_parameter(EnvelopeSpec spec);
	void add_parameter(SliderSpec spec);

private:

	std::map<blkhdgen_ID, Group> groups_;
	std::map<blkhdgen_UUID, std::shared_ptr<Parameter>> parameters_;
};

void Generator::add_group(blkhdgen_ID id, std::string name)
{
	groups_[id] = { id, name };
}

void Generator::add_parameter(EnvelopeSpec spec)
{
	parameters_[spec.uuid] = std::make_shared<EnvelopeParameter>(spec);
}

void Generator::add_parameter(SliderSpec spec)
{
	parameters_[spec.uuid] = std::make_shared<SliderParameter>(spec);
}

}