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
	blkhdgen_Error set_warp_points(blkhdgen_Index count, blkhdgen_WarpPoint* points);

	int get_num_groups() const;
	int get_num_parameters() const;

	const Group& get_group(blkhdgen_Index index) const;
	const Group& get_group_by_id(blkhdgen_ID id) const;
	Parameter& get_parameter(blkhdgen_Index index);
	Parameter& get_parameter_by_id(blkhdgen_UUID uuid);

	float get_mod_value(blkhdgen_Position block_position) const;
	blkhdgen_Position get_waveform_position(blkhdgen_Position block_position) const;

	blkhdgen_Error set_get_sample_info_cb(void* user, blkhdgen_GetSampleInfoCB cb);
	blkhdgen_Error set_get_sample_data_cb(void* user, blkhdgen_GetSampleDataCB cb);

protected:

	void add_group(blkhdgen_ID id, std::string name);
	void add_parameter(EnvelopeSpec spec);
	void add_parameter(OptionSpec spec);
	void add_parameter(SliderSpec spec);
	void add_parameter(ToggleSpec spec);

private:

	std::map<blkhdgen_ID, Group> groups_;
	std::map<blkhdgen_UUID, std::shared_ptr<Parameter>> parameters_;
	std::function<void(blkhdgen_SampleInfo*)> get_sample_info_;
	std::function<void(blkhdgen_ChannelCount, blkhdgen_Index, blkhdgen_FrameCount, float*)> get_sample_data_;
};

void Generator::add_group(blkhdgen_ID id, std::string name)
{
	groups_[id] = { id, name };
}

void Generator::add_parameter(EnvelopeSpec spec)
{
	parameters_[spec.uuid] = std::make_shared<EnvelopeParameter>(spec);
}

void Generator::add_parameter(OptionSpec spec)
{
	parameters_[spec.uuid] = std::make_shared<OptionParameter>(spec);
}

void Generator::add_parameter(SliderSpec spec)
{
	parameters_[spec.uuid] = std::make_shared<SliderParameter>(spec);
}

void Generator::add_parameter(ToggleSpec spec)
{
	parameters_[spec.uuid] = std::make_shared<ToggleParameter>(spec);
}

blkhdgen_Index Generator::add_warp_point(blkhdgen_IntPosition block_position)
{
	// TODO: implement this
}

blkhdgen_Error Generator::remove_warp_point(blkhdgen_Index index)
{
	// TODO: implement this
}

blkhdgen_Error Generator::move_warp_point(blkhdgen_Index index, blkhdgen_IntPosition new_position)
{
	// TODO: implement this
}

blkhdgen_Error Generator::clear_warp_points()
{
	// TODO: implement this
}

blkhdgen_Error Generator::set_warp_points(blkhdgen_Index count, blkhdgen_WarpPoint* points)
{
	// TODO: implement this
}

int Generator::get_num_groups() const
{
	return int(groups_.size());
}

int Generator::get_num_parameters() const
{
	return int(parameters_.size());
}

const Group& Generator::get_group(blkhdgen_Index index) const
{
	auto pos = groups_.begin();

	std::advance(pos, index);

	return pos->second;
}

const Group& Generator::get_group_by_id(blkhdgen_ID id) const
{
	auto pos = groups_.find(id);

	return pos->second;
}

Parameter& Generator::get_parameter(blkhdgen_Index index)
{
	auto pos = parameters_.begin();

	std::advance(pos, index);

	return *pos->second;
}

Parameter& Generator::get_parameter_by_id(blkhdgen_UUID uuid)
{
	auto pos = parameters_.find(uuid);

	return *pos->second;
}

float Generator::get_mod_value(blkhdgen_Position block_position) const
{
	// TODO: implement this
}

blkhdgen_Position Generator::get_waveform_position(blkhdgen_Position block_position) const
{
	// TODO: implement this
}

blkhdgen_Error Generator::set_get_sample_info_cb(void* user, blkhdgen_GetSampleInfoCB cb)
{
	get_sample_info_ = [user, cb](blkhdgen_SampleInfo* info)
	{
		cb(user, info);
	};
}

blkhdgen_Error Generator::set_get_sample_data_cb(void* user, blkhdgen_GetSampleDataCB cb)
{
	get_sample_data_ = [user, cb](blkhdgen_ChannelCount channel, blkhdgen_Index index, blkhdgen_FrameCount size, float* buffer)
	{
		cb(user, channel, index, size, buffer);
	};
}

}