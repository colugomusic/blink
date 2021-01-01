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
	virtual blkhdgen_Error process(blkhdgen_SR song_rate, blkhdgen_SR sample_rate, const blkhdgen_Position* pos, float** out) = 0;
	virtual const char* get_error_string(blkhdgen_Error error) const = 0;
	virtual blkhdgen_Position get_waveform_position(blkhdgen_Position block_position, float* derivative = nullptr) const = 0;
	virtual void set_data_offset(int offset) = 0;

	int get_num_channels() const { return 2; }

	int get_num_groups() const;
	int get_num_parameters() const;

	const Group& get_group(blkhdgen_Index index) const;
	const Group& get_group_by_id(blkhdgen_ID id) const;
	Parameter& get_parameter(blkhdgen_Index index);
	Parameter& get_parameter_by_id(blkhdgen_UUID uuid);

	blkhdgen_Error set_get_sample_info_cb(void* user, blkhdgen_GetSampleInfoCB cb);
	blkhdgen_Error set_get_sample_data_cb(void* user, blkhdgen_GetSampleDataCB cb);
	blkhdgen_Error set_get_warp_point_data_cb(void* user, blkhdgen_GetWarpPointDataCB cb);
	blkhdgen_Error set_get_manipulator_data_cb(void* user, blkhdgen_GetManipulatorDataCB cb);

protected:

	void add_group(blkhdgen_ID id, std::string name);
	std::shared_ptr<EnvelopeParameter> add_parameter(EnvelopeSpec spec);
	std::shared_ptr<OptionParameter> add_parameter(OptionSpec spec);
	std::shared_ptr<ToggleParameter> add_parameter(ToggleSpec spec);

	template <class T>
	std::shared_ptr<SliderParameter<T>> add_parameter(SliderSpec<T> spec);

private:

	std::map<blkhdgen_ID, Group> groups_;
	std::map<blkhdgen_UUID, std::shared_ptr<Parameter>> parameters_;
	std::function<void(blkhdgen_SampleInfo*)> get_sample_info_;
	std::function<void(blkhdgen_ChannelCount, blkhdgen_Index, blkhdgen_FrameCount, float*)> get_sample_data_;
	std::function<blkhdgen_WarpPoints*()> get_warp_point_data_;
	std::function<blkhdgen_ManipulatorData*()> get_manipulator_data_;
};

void Generator::add_group(blkhdgen_ID id, std::string name)
{
	groups_[id] = { id, name };
}

std::shared_ptr<EnvelopeParameter> Generator::add_parameter(EnvelopeSpec spec)
{
	const auto param = std::make_shared<EnvelopeParameter>(spec);

	parameters_[spec.uuid] = param;

	return param;
}

std::shared_ptr<OptionParameter> Generator::add_parameter(OptionSpec spec)
{
	const auto param = std::make_shared<OptionParameter>(spec);

	parameters_[spec.uuid] = param;

	return param;
}

template <class T>
std::shared_ptr<SliderParameter<T>> Generator::add_parameter(SliderSpec<T> spec)
{
	const auto param = std::make_shared<SliderParameter<T>>(spec);

	parameters_[spec.uuid] = param;

	return param;
}

std::shared_ptr<ToggleParameter> Generator::add_parameter(ToggleSpec spec)
{
	const auto param = std::make_shared<ToggleParameter>(spec);

	parameters_[spec.uuid] = param;

	return param;
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

blkhdgen_Error Generator::set_get_sample_info_cb(void* user, blkhdgen_GetSampleInfoCB cb)
{
	get_sample_info_ = [user, cb](blkhdgen_SampleInfo* info)
	{
		cb(user, info);
	};

	return BLKHDGEN_OK;
}

blkhdgen_Error Generator::set_get_sample_data_cb(void* user, blkhdgen_GetSampleDataCB cb)
{
	get_sample_data_ = [user, cb](blkhdgen_ChannelCount channel, blkhdgen_Index index, blkhdgen_FrameCount size, float* buffer)
	{
		cb(user, channel, index, size, buffer);
	};

	return BLKHDGEN_OK;
}

blkhdgen_Error Generator::set_get_warp_point_data_cb(void* user, blkhdgen_GetWarpPointDataCB cb)
{
	get_warp_point_data_ = [user, cb]()
	{
		return cb(user);
	};

	return BLKHDGEN_OK;
}

blkhdgen_Error Generator::set_get_manipulator_data_cb(void* user, blkhdgen_GetManipulatorDataCB cb)
{
	get_manipulator_data_ = [user, cb]()
	{
		return cb(user);
	};

	return BLKHDGEN_OK;
}

}