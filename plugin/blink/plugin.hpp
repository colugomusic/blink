#pragma once

#define _USE_MATH_DEFINES
#include <map>
#include <memory>
#include <set>
#include "blink.h"
#include "generator_base.hpp"
#include "envelope_spec.hpp"
#include "group.hpp"
#include "slider_spec.hpp"
#include "chord_parameter.hpp"
#include "envelope_parameter.hpp"
#include "option_parameter.hpp"
#include "slider_parameter.hpp"
#include "toggle_parameter.hpp"

namespace blink {

class Plugin
{
public:

	void register_instance(GeneratorBase* instance);
	void unregister_instance(GeneratorBase* instance);

	void stream_init(blink_SR SR);

	int get_num_groups() const;
	int get_num_parameters() const;

	int add_group(std::string name);
	std::shared_ptr<ChordParameter> add_parameter(ChordSpec spec);
	std::shared_ptr<EnvelopeParameter> add_parameter(EnvelopeSpec spec);
	std::shared_ptr<OptionParameter> add_parameter(OptionSpec spec);
	std::shared_ptr<ToggleParameter> add_parameter(ToggleSpec spec);

	template <class T>
	std::shared_ptr<SliderParameter<T>> add_parameter(SliderParameterSpec<T> spec);

	const Group& get_group(int index) const;
	Parameter& get_parameter(blink_Index index);
	Parameter& get_parameter_by_uuid(blink_UUID uuid);

	template <int Index> static const blink_EnvelopeData* get_envelope_data(const blink_ParameterData* data);
	template <int Index> static const blink_ToggleData* get_toggle_data(const blink_ParameterData* data);
	template <int Index, bool Default = false> static bool get_toggle_value(const blink_ParameterData* data);

	static const blink_ChordData* get_chord_data(const blink_ParameterData* data, int index);
	static const blink_EnvelopeData* get_envelope_data(const blink_ParameterData* data, int index);
	static const blink_SliderData* get_slider_data(const blink_ParameterData* data, int index);
	static const blink_IntSliderData* get_int_slider_data(const blink_ParameterData* data, int index);
	static const blink_ToggleData* get_toggle_data(const blink_ParameterData* data, int index);
	static const blink_OptionData* get_option_data(const blink_ParameterData* data, int index);

	void begin_process(std::uint64_t buffer_id, int instance_group);

protected:

	void initialize_instance_group(int instance_group);

private:

	virtual void hard_reset(int instance_group) {}

	void add_parameter(blink_UUID uuid, std::shared_ptr<Parameter> parameter);

	blink_SR SR_ = 0;
	std::vector<Group> groups_;
	std::vector<std::shared_ptr<Parameter>> parameters_;
	std::map<blink_UUID, Parameter*> uuid_parameter_map_;
	std::set<GeneratorBase*> instances_;

	struct InstanceGroupData
	{
		std::uint64_t buffer_id = 0;
		int active_buffer_instances = 0;
	};

	std::map<int, InstanceGroupData> instance_group_data_;
};

inline void Plugin::register_instance(GeneratorBase* instance)
{
	if (SR_ > 0)
	{
		instance->stream_init(SR_);
	}

	instances_.insert(instance);
}

inline void Plugin::unregister_instance(GeneratorBase* instance)
{
	instances_.erase(instance);
}

inline void Plugin::stream_init(blink_SR SR)
{
	SR_ = SR;

	for (auto instance : instances_)
	{
		instance->stream_init(SR);
	}
}

inline void Plugin::initialize_instance_group(int instance_group)
{
	if (instance_group_data_.find(instance_group) == instance_group_data_.end())
	{
		instance_group_data_[instance_group] = InstanceGroupData();
	}
}

inline void Plugin::begin_process(std::uint64_t buffer_id, int instance_group)
{
	auto& instance_group_data = instance_group_data_[instance_group];

	if (buffer_id > instance_group_data.buffer_id)
	{
		if (buffer_id > instance_group_data.buffer_id + 1 || instance_group_data.active_buffer_instances == 0)
		{
			hard_reset(instance_group);
		}

		instance_group_data.buffer_id = buffer_id;
		instance_group_data.active_buffer_instances = 0;
	}

	instance_group_data.active_buffer_instances++;
}

template <int Index> const blink_EnvelopeData* Plugin::get_envelope_data(const blink_ParameterData* data)
{
	return data ? &data[Index].envelope : nullptr;
}

template <int Index> const blink_ToggleData* Plugin::get_toggle_data(const blink_ParameterData* data)
{
	return data ? &data[Index].toggle : nullptr;
}

template <int Index, bool Default> bool Plugin::get_toggle_value(const blink_ParameterData* data)
{
	const auto toggle = get_toggle_data<Index>(data);

	return toggle ? toggle->value : Default;
}

inline const blink_ChordData* Plugin::get_chord_data(const blink_ParameterData* data, int index)
{
	return data ? &data[index].chord : nullptr;
}

inline const blink_EnvelopeData* Plugin::get_envelope_data(const blink_ParameterData* data, int index)
{
	return data ? &data[index].envelope : nullptr;
}

inline const blink_SliderData* Plugin::get_slider_data(const blink_ParameterData* data, int index)
{
	return data ? &data[index].slider : nullptr;
}

inline const blink_IntSliderData* Plugin::get_int_slider_data(const blink_ParameterData* data, int index)
{
	return data ? &data[index].int_slider : nullptr;
}

inline const blink_ToggleData* Plugin::get_toggle_data(const blink_ParameterData* data, int index)
{
	return data ? &data[index].toggle : nullptr;
}

inline const blink_OptionData* Plugin::get_option_data(const blink_ParameterData* data, int index)
{
	return data ? &data[index].option : nullptr;
}

inline int Plugin::add_group(std::string name)
{
	groups_.push_back({ name });

	return int(groups_.size() - 1);
}

inline void Plugin::add_parameter(blink_UUID uuid, std::shared_ptr<Parameter> parameter)
{
	parameters_.push_back(parameter);
	uuid_parameter_map_[uuid] = parameter.get();
}

inline std::shared_ptr<ChordParameter> Plugin::add_parameter(ChordSpec spec)
{
	const auto param = std::make_shared<ChordParameter>(spec);

	add_parameter(spec.uuid, param);

	return param;
}

inline std::shared_ptr<EnvelopeParameter> Plugin::add_parameter(EnvelopeSpec spec)
{
	const auto param = std::make_shared<EnvelopeParameter>(spec);

	add_parameter(spec.uuid, param);

	return param;
}

inline std::shared_ptr<OptionParameter> Plugin::add_parameter(OptionSpec spec)
{
	const auto param = std::make_shared<OptionParameter>(spec);

	add_parameter(spec.uuid, param);

	return param;
}

template <class T>
inline std::shared_ptr<SliderParameter<T>> Plugin::add_parameter(SliderParameterSpec<T> spec)
{
	const auto param = std::make_shared<SliderParameter<T>>(spec);

	add_parameter(spec.uuid, param);

	return param;
}

inline std::shared_ptr<ToggleParameter> Plugin::add_parameter(ToggleSpec spec)
{
	const auto param = std::make_shared<ToggleParameter>(spec);

	add_parameter(spec.uuid, param);

	return param;
}

inline int Plugin::get_num_groups() const
{
	return int(groups_.size());
}

inline int Plugin::get_num_parameters() const
{
	return int(parameters_.size());
}

inline const Group& Plugin::get_group(int index) const
{
	return groups_[index];
}

inline Parameter& Plugin::get_parameter(blink_Index index)
{
	return *(parameters_[index]);
}

inline Parameter& Plugin::get_parameter_by_uuid(blink_UUID uuid)
{
	auto pos = uuid_parameter_map_.find(uuid);

	return *pos->second;
}

}
