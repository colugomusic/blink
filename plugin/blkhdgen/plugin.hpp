#pragma once

#include <map>
#include "envelope_spec.hpp"
#include "group.hpp"
#include "slider_spec.hpp"
#include "envelope_parameter.hpp"
#include "option_parameter.hpp"
#include "slider_parameter.hpp"
#include "toggle_parameter.hpp"

namespace blkhdgen {

class Plugin
{
public:

	int get_num_groups() const;
	int get_num_parameters() const;

	const Group& get_group(blkhdgen_Index index) const;
	const Group& get_group_by_id(blkhdgen_ID id) const;
	Parameter& get_parameter(blkhdgen_Index index);
	Parameter& get_parameter_by_uuid(blkhdgen_UUID uuid);

protected:

	void add_group(blkhdgen_ID id, std::string name);
	std::shared_ptr<EnvelopeParameter> add_parameter(EnvelopeSpec spec);
	std::shared_ptr<OptionParameter> add_parameter(OptionSpec spec);
	std::shared_ptr<ToggleParameter> add_parameter(ToggleSpec spec);

	template <class T>
	std::shared_ptr<SliderParameter<T>> add_parameter(SliderSpec<T> spec);

private:

	void add_parameter(blkhdgen_UUID uuid, std::shared_ptr<Parameter> parameter);

	std::map<blkhdgen_ID, Group> groups_;
	std::vector<std::shared_ptr<Parameter>> parameters_;
	std::map<blkhdgen_UUID, Parameter*> uuid_parameter_map_;
};

inline void Plugin::add_group(blkhdgen_ID id, std::string name)
{
	groups_[id] = { id, name };
}

inline void Plugin::add_parameter(blkhdgen_UUID uuid, std::shared_ptr<Parameter> parameter)
{
	parameters_.push_back(parameter);
	uuid_parameter_map_[uuid] = parameter.get();
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
inline std::shared_ptr<SliderParameter<T>> Plugin::add_parameter(SliderSpec<T> spec)
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

inline const Group& Plugin::get_group(blkhdgen_Index index) const
{
	auto pos = groups_.begin();

	std::advance(pos, index);

	return pos->second;
}

inline const Group& Plugin::get_group_by_id(blkhdgen_ID id) const
{
	auto pos = groups_.find(id);

	return pos->second;
}

inline Parameter& Plugin::get_parameter(blkhdgen_Index index)
{
	return *(parameters_[index]);
}

inline Parameter& Plugin::get_parameter_by_uuid(blkhdgen_UUID uuid)
{
	auto pos = uuid_parameter_map_.find(uuid);

	return *pos->second;
}

}