#pragma once

#define _USE_MATH_DEFINES
#include <map>
#include <memory>
#include <set>
#include <string>
#include "blink.h"
#include "instance.hpp"
#include <blink/bind.hpp>
#include <blink/envelope_spec.hpp>
#include <blink/slider_spec.hpp>
#include <blink/parameters/chord_parameter.hpp>
#include <blink/parameters/envelope_manipulator_target.hpp>
#include <blink/parameters/envelope_parameter.hpp>
#include <blink/parameters/option_parameter.hpp>
#include <blink/parameters/slider_parameter.hpp>
#include "resource_store.hpp"

namespace blink {

class Plugin
{
public:

	void register_instance(Instance* instance);
	void unregister_instance(Instance* instance);

	int get_num_parameters() const;

	std::shared_ptr<ChordParameter> add_parameter(ChordSpec spec);
	std::shared_ptr<EnvelopeParameter> add_parameter(EnvelopeParameterSpec spec);
	std::shared_ptr<OptionParameter> add_parameter(OptionSpec spec);

	template <class T>
	std::shared_ptr<SliderParameter<T>> add_parameter(SliderParameterSpec<T> spec);

	const Parameter& get_parameter(blink_Index index) const;
	const Parameter& get_parameter(blink_UUID uuid) const;

	auto& resources() { return resources_; }

	template <class FileSystem>
	auto get_resource_data(const FileSystem& fs, const char* path) -> blink_ResourceData;

private:

	void add_parameter(blink_UUID uuid, std::shared_ptr<Parameter> parameter);

	struct
	{
		std::vector<std::shared_ptr<Parameter>> store;
		std::map<std::string, Parameter*> uuid_map;
	} parameters_;

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

inline int Plugin::get_num_parameters() const
{
	return int(parameters_.store.size());
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

template <class FileSystem>
inline auto Plugin::get_resource_data(const FileSystem& fs, const char* path) -> blink_ResourceData
{
	if (resources_.has(path)) return resources_.get(path);

	if (!fs.exists(path)) return { 0, 0 };
	if (!fs.is_file(path)) return { 0, 0 };

	const auto file { fs.open(path) };

	return resources_.store(path, file);
}

} // blink
