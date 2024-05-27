#pragma once

#include <ent.hpp>
#include "blink.h"
#include "block_positions.hpp"
#include "common_impl.hpp"
#include "entity_store.hpp"
#include "resource_store.hpp"
#include "types.hpp"

namespace blink {

template <typename... Ts>
using Instance = ::ent::dynamic_store<
	UnitVec,
	Ts...
>;
template <typename... Ts>
using Unit = ::ent::dynamic_store<
	Ts...
>;

struct Plugin {
	blink_PluginIdx index;
	blink_HostFns host;
	ResourceStore resource_store;
};

template <typename Instance, typename Unit>
struct Entities {
	Instance instance;
	Unit unit;
};

struct ChordData {
	const blink_ChordData* data;
};

struct EnvData {
	const blink_EnvData* data;
	float default_value;
	float value;
};

struct OptionData {
	const blink_OptionData* data;
	int64_t value;
	int64_t default_value;
};

struct SliderIntData {
	const blink_SliderIntData* data;
	int64_t value;
	int64_t default_value;
};

struct SliderRealData {
	const blink_SliderRealData* data;
	float value;
	float default_value;
};

inline
auto init(Plugin* plugin, blink_PluginIdx plugin_index, blink_HostFns host_fns) -> void {
	plugin->index = plugin_index;
	plugin->host = host_fns;
}

template <typename Instance, typename Unit> [[nodiscard]]
auto terminate(Entities<Instance, Unit>* ents) -> blink_Error {
	ents->instance.clear();
	ents->unit.clear();
	return BLINK_OK;
}

template <typename Instance, typename Unit> [[nodiscard]]
auto add_unit(Entities<Instance, Unit>* ents, blink_InstanceIdx instance_idx) -> blink_UnitIdx {
	const auto index = blink_UnitIdx{ents->unit.add()};
	auto& units = ents->instance.get<UnitVec>(instance_idx.value);
	units.value.push_back(index);
	return index;
}

template <typename Instance, typename Unit> [[nodiscard]]
auto make_instance(Entities<Instance, Unit>* ents) -> blink_InstanceIdx {
	return {ents->instance.add()};
}

template <typename Instance, typename Unit> [[nodiscard]]
auto destroy_instance(Entities<Instance, Unit>* ents, blink_InstanceIdx instance_idx) -> blink_Error {
	for (auto unit_idx : ents->instance.get<UnitVec>(instance_idx.value).value) {
		ents->unit.erase(unit_idx.value);
	}
	ents->instance.erase(instance_idx.value);
	return BLINK_OK;
}

[[nodiscard]] inline
auto get_std_error_string(blink_StdError error) -> const char* {
	switch (error) {
		case blink_StdError_AlreadyInitialized: return "already initialized";
		case blink_StdError_NotInitialized: return "not initialized";
		case blink_StdError_NotImplemented: return "not implemented";
		case blink_StdError_InvalidInstance: return "invalid instance";
		default: return "unknown error";
	}
}

namespace add {
namespace param {

inline
auto env(const Plugin& plugin, blink_UUID uuid) -> blink_ParamIdx {
	return plugin.host.add_param_env(plugin.host.usr, plugin.index, uuid);
}

inline
auto option(const Plugin& plugin, blink_UUID uuid) -> blink_ParamIdx {
	return plugin.host.add_param_option(plugin.host.usr, plugin.index, uuid);
}

inline
auto slider_int(const Plugin& plugin, blink_UUID uuid) -> blink_ParamIdx {
	return plugin.host.add_param_slider_int(plugin.host.usr, plugin.index, uuid);
}

inline
auto slider_real(const Plugin& plugin, blink_UUID uuid) -> blink_ParamIdx {
	return plugin.host.add_param_slider_real(plugin.host.usr, plugin.index, uuid);
}

} // param
} // add

namespace write {
namespace param {

inline
auto add_flags(const Plugin& plugin, blink_ParamIdx param_idx, int flags) -> void {
	plugin.host.write_param_add_flags(plugin.host.usr, plugin.index, param_idx, flags);
}

inline
auto manip_delegate(const Plugin& plugin, blink_ParamIdx param_idx, blink_ParamIdx delegate_idx) -> void {
	plugin.host.write_param_manip_delegate(plugin.host.usr, plugin.index, param_idx, delegate_idx);
}

inline
auto add_subparam(const Plugin& plugin, blink_ParamIdx param_idx, blink_ParamIdx subparam_idx) -> void {
	plugin.host.write_param_add_subparam(plugin.host.usr, plugin.index, param_idx, subparam_idx);
}

inline
auto group(const Plugin& plugin, blink_ParamIdx param_idx, blink_StaticString group_name) -> void {
	plugin.host.write_param_group(plugin.host.usr, plugin.index, param_idx, group_name);
}

inline
auto name(const Plugin& plugin, blink_ParamIdx param_idx, blink_StaticString name) -> void {
	plugin.host.write_param_name(plugin.host.usr, plugin.index, param_idx, name);
}

inline
auto short_name(const Plugin& plugin, blink_ParamIdx param_idx, blink_StaticString name) -> void {
	plugin.host.write_param_short_name(plugin.host.usr, plugin.index, param_idx, name);
}

inline
auto env_env_idx(const Plugin& plugin, blink_ParamIdx param_idx, blink_EnvIdx env_idx) -> void {
	plugin.host.write_param_env_env_idx(plugin.host.usr, plugin.index, param_idx, env_idx);
}

inline
auto env_clamp_range(const Plugin& plugin, blink_ParamIdx param_idx, blink_Range range) -> void {
	plugin.host.write_param_env_clamp_range(plugin.host.usr, plugin.index, param_idx, range);
}

inline
auto env_offset_env(const Plugin& plugin, blink_ParamIdx param_idx, blink_EnvIdx env_idx) -> void {
	plugin.host.write_param_env_offset_env(plugin.host.usr, plugin.index, param_idx, env_idx);
}

inline
auto env_override_env(const Plugin& plugin, blink_ParamIdx param_idx, blink_EnvIdx env_idx) -> void {
	plugin.host.write_param_env_override_env(plugin.host.usr, plugin.index, param_idx, env_idx);
}

} // param
} // write

[[nodiscard]] inline
auto make_chord_data(const Plugin& plugin, const blink_ParamData* param_data, blink_ParamIdx param_idx) -> ChordData {
	ChordData out;
	out.data = &param_data->chord;
	return out;
}

[[nodiscard]] inline
auto make_int_value(const blink_IntPoints& points, int64_t default_value) -> int64_t {
	return points.count > 0 ? points.data[0].y : default_value;
}

[[nodiscard]] inline
auto make_real_value(const blink_RealPoints& points, float default_value) -> float {
	return points.count > 0 ? points.data[0].y : default_value;
}

[[nodiscard]] inline
auto make_env_data(const Plugin& plugin, const blink_ParamData* param_data, blink_ParamIdx param_idx) -> EnvData {
	EnvData out;
	out.default_value = plugin.host.read_param_env_default_value(plugin.host.usr, plugin.index, param_idx);
	if (param_data) {
		out.data = &param_data->envelope;
		out.value = make_real_value(out.data->points, out.default_value);
	}
	else {
		out.data = nullptr;
		out.value = out.default_value;
	}
	return out;
}

[[nodiscard]] inline
auto make_option_data(const Plugin& plugin, const blink_ParamData* param_data, blink_ParamIdx param_idx) -> OptionData {
	OptionData out;
	out.default_value = plugin.host.read_param_option_default_value(plugin.host.usr, plugin.index, param_idx);
	if (param_data) {
		out.data = &param_data->option;
		out.value = make_int_value(out.data->points, out.default_value);
	}
	else {
		out.data = nullptr;
		out.value = out.default_value;
	}
	return out;
}

[[nodiscard]] inline
auto make_slider_int_data(const Plugin& plugin, const blink_ParamData* param_data, blink_ParamIdx param_idx) -> SliderIntData {
	SliderIntData out;
	out.default_value = plugin.host.read_param_slider_int_default_value(plugin.host.usr, plugin.index, param_idx);
	if (param_data) {
		out.data = &param_data->slider_int;
		out.value = make_int_value(out.data->points, out.default_value);
	}
	else {
		out.data = nullptr;
		out.value = out.default_value;
	}
	return out;
}

[[nodiscard]] inline
auto make_slider_real_data(const Plugin& plugin, const blink_ParamData* param_data, blink_ParamIdx param_idx) -> SliderRealData {
	SliderRealData out;
	out.default_value = plugin.host.read_param_slider_real_default_value(plugin.host.usr, plugin.index, param_idx);
	if (param_data) {
		out.data = &param_data->slider_real;
		out.value = make_real_value(out.data->points, out.default_value);
	}
	else {
		out.data = nullptr;
		out.value = out.default_value;
	}
	return out;
}

template <class FileSystem> [[nodiscard]] inline
auto get_resource_data(Plugin* plugin, const FileSystem& fs, const char* path) -> blink_ResourceData {
	if (plugin->resource_store.has(path)) {
		return plugin->resource_store.get(path);
	}
	if (!fs.exists(path)) return  { 0, 0 };
	if (!fs.is_file(path)) return { 0, 0 };
	const auto file = fs.open(path);
	return plugin->resource_store.store(path, file);
}

} // blink
