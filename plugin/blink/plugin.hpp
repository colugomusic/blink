#pragma once

#define _USE_MATH_DEFINES
#include <map>
#include <memory>
#include <set>
#include <string>
#include "blink.h"
#include "instance.hpp"
#include <blink/envelope_spec.hpp>
#include <blink/slider_spec.hpp>
#include <blink/parameters/group.hpp>
#include <blink/parameters/chord_parameter.hpp>
#include <blink/parameters/envelope_manipulator_target.hpp>
#include <blink/parameters/envelope_parameter.hpp>
#include <blink/parameters/option_parameter.hpp>
#include <blink/parameters/slider_parameter.hpp>
#include <blink/parameters/toggle_parameter.hpp>
#include "resource_store.hpp"

namespace blink {

class Plugin
{
public:

	void register_instance(Instance* instance);
	void unregister_instance(Instance* instance);

	int get_num_groups() const;
	int get_num_parameters() const;

	int add_group(std::string name);
	std::shared_ptr<ChordParameter> add_parameter(ChordSpec spec);
	std::shared_ptr<EnvelopeParameter> add_parameter(EnvelopeParameterSpec spec);
	std::shared_ptr<OptionParameter> add_parameter(OptionSpec spec);
	std::shared_ptr<ToggleParameter> add_parameter(ToggleSpec spec);
	std::shared_ptr<EnvelopeManipulatorTarget> add_manipulator_target(blink_UUID uuid, EnvelopeManipulatorTargetSpec spec);

	template <class T>
	std::shared_ptr<SliderParameter<T>> add_parameter(SliderParameterSpec<T> spec);

	const Group& get_group(int index) const;
	const Parameter& get_parameter(blink_Index index) const;
	const Parameter& get_parameter(blink_UUID uuid) const;

	std::optional<const EnvelopeManipulatorTarget*> get_envelope_manipulator_target(blink_UUID uuid) const;

	//template <int Index> static const blink_ChordData& get_chord_data(const blink_ParameterData* data);
	//template <int Index> static const blink_EnvelopeData& get_envelope_data(const blink_ParameterData* data);
	//template <int Index> static const blink_OptionData& get_option_data(const blink_ParameterData* data);
	//template <int Index> static const blink_ToggleData& get_toggle_data(const blink_ParameterData* data);

	//static const blink_ChordData& get_chord_data(const blink_ParameterData* data, int index);
	//static const blink_EnvelopeData& get_envelope_data(const blink_ParameterData* data, int index);
	//static const blink_SliderData& get_slider_data(const blink_ParameterData* data, int index);
	//static const blink_IntSliderData& get_int_slider_data(const blink_ParameterData* data, int index);
	//static const blink_ToggleData& get_toggle_data(const blink_ParameterData* data, int index);
	//static const blink_OptionData& get_option_data(const blink_ParameterData* data, int index);

	auto& resources() { return resources_; }

private:

	void add_parameter(blink_UUID uuid, std::shared_ptr<Parameter> parameter);

	struct
	{
		std::vector<std::shared_ptr<Parameter>> store;
		std::map<std::string, Parameter*> uuid_map;
	} parameters_;

	struct
	{
		std::vector<std::shared_ptr<EnvelopeManipulatorTarget>> store;
		std::map<std::string, EnvelopeManipulatorTarget*> uuid_map;
	} envelope_manipulator_targets_;

	std::vector<Group> groups_;
	std::set<Instance*> instances_;
	ResourceStore resources_;
};

inline void Plugin::register_instance(Instance* instance)
{
	instances_.insert(instance);
}

inline void Plugin::unregister_instance(Instance* instance)
{
	instances_.erase(instance);
}

//template <int Index> const blink_ChordData& Plugin::get_chord_data(const blink_ParameterData* data)
//{
//	return data[Index].chord;
//}
//
//template <int Index> const blink_EnvelopeData& Plugin::get_envelope_data(const blink_ParameterData* data)
//{
//	return data[Index].envelope;
//}
//
//template <int Index> const blink_OptionData& Plugin::get_option_data(const blink_ParameterData* data)
//{
//	return data[Index].option;
//}
//
//template <int Index> const blink_ToggleData& Plugin::get_toggle_data(const blink_ParameterData* data)
//{
//	return data[Index].toggle;
//}
//
//inline const blink_ChordData& Plugin::get_chord_data(const blink_ParameterData* data, int index)
//{
//	return data[index].chord;
//}
//
//inline const blink_EnvelopeData& Plugin::get_envelope_data(const blink_ParameterData* data, int index)
//{
//	return data[index].envelope;
//}
//
//inline const blink_SliderData& Plugin::get_slider_data(const blink_ParameterData* data, int index)
//{
//	return data[index].slider;
//}
//
//inline const blink_IntSliderData& Plugin::get_int_slider_data(const blink_ParameterData* data, int index)
//{
//	return data[index].int_slider;
//}
//
//inline const blink_ToggleData& Plugin::get_toggle_data(const blink_ParameterData* data, int index)
//{
//	return data[index].toggle;
//}

//inline const blink_OptionData& Plugin::get_option_data(const blink_ParameterData* data, int index)
//{
//	return data[index].option;
//}

inline int Plugin::add_group(std::string name)
{
	groups_.push_back({ name });

	return int(groups_.size() - 1);
}

inline void Plugin::add_parameter(blink_UUID uuid, std::shared_ptr<Parameter> parameter)
{
	parameters_.store.push_back(parameter);
	parameters_.uuid_map[uuid] = parameter.get();
}

inline std::shared_ptr<ChordParameter> Plugin::add_parameter(ChordSpec spec)
{
	const auto param { std::make_shared<ChordParameter>(spec) };

	add_parameter(spec.uuid, param);

	return param;
}

inline std::shared_ptr<EnvelopeParameter> Plugin::add_parameter(EnvelopeParameterSpec spec)
{
	const auto param { std::make_shared<EnvelopeParameter>(spec) };

	add_parameter(spec.uuid, param);

	return param;
}

inline std::shared_ptr<OptionParameter> Plugin::add_parameter(OptionSpec spec)
{
	const auto param { std::make_shared<OptionParameter>(spec) };

	add_parameter(spec.uuid, param);

	return param;
}

template <class T>
inline std::shared_ptr<SliderParameter<T>> Plugin::add_parameter(SliderParameterSpec<T> spec)
{
	const auto param { std::make_shared<SliderParameter<T>>(spec) };

	add_parameter(spec.uuid, param);

	return param;
}

inline std::shared_ptr<ToggleParameter> Plugin::add_parameter(ToggleSpec spec)
{
	const auto param { std::make_shared<ToggleParameter>(spec) };

	add_parameter(spec.uuid, param);

	return param;
}

inline std::shared_ptr<EnvelopeManipulatorTarget> Plugin::add_manipulator_target(blink_UUID uuid, EnvelopeManipulatorTargetSpec spec)
{
	const auto out { std::make_shared<EnvelopeManipulatorTarget>(spec) };

	envelope_manipulator_targets_.store.push_back(out);
	envelope_manipulator_targets_.uuid_map[uuid] = out.get();

	return out;
}

inline int Plugin::get_num_groups() const
{
	return int(groups_.size());
}

inline int Plugin::get_num_parameters() const
{
	return int(parameters_.store.size());
}

inline const Group& Plugin::get_group(int index) const
{
	return groups_[index];
}

inline const Parameter& Plugin::get_parameter(blink_Index index) const
{
	return *(parameters_.store[index]);
}

inline const Parameter& Plugin::get_parameter(blink_UUID uuid) const
{
	const auto pos { parameters_.uuid_map.find(uuid) };

	assert(pos != parameters_.uuid_map.end());

	return *pos->second;
}

inline std::optional<const EnvelopeManipulatorTarget*> Plugin::get_envelope_manipulator_target(blink_UUID uuid) const
{
	const auto pos { envelope_manipulator_targets_.uuid_map.find(uuid) };

	if (pos == envelope_manipulator_targets_.uuid_map.end()) return {};

	return pos->second;
}

}
