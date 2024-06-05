#pragma once

#include <ent.hpp>
#include "blink.h"
#include "block_positions.hpp"
#include "common_impl.hpp"
#include "data.hpp"
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
	blink_InstanceIdx,
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

inline
auto init(Plugin* plugin, blink_PluginIdx plugin_index, blink_HostFns host_fns) -> void {
	plugin->index = plugin_index;
	plugin->host = host_fns;
}

inline
auto init(Plugin* plugin, blink_PluginIdx plugin_index, blink_HostFns host_fns, blink_SamplerInfo sampler_info) -> void {
	init(plugin, plugin_index, host_fns);
	host_fns.write_sampler_info(host_fns.usr, plugin_index, sampler_info);
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
	ents->unit.get<blink_InstanceIdx>(index.value) = instance_idx;
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

namespace read {

inline
auto env(const Plugin& plugin, blink_ParamIdx param_idx) -> blink_EnvIdx {
	return plugin.host.read_param_env_env(plugin.host.usr, plugin.index, param_idx);
}

} // read

namespace add {
namespace param {

inline
auto chord(const Plugin& plugin, blink_UUID uuid) -> blink_ParamIdx {
	return plugin.host.add_param_chord(plugin.host.usr, plugin.index, uuid);
}

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
namespace env {

inline
auto add_flags(const Plugin& plugin, blink_EnvIdx env_idx, int flags) -> void {
	plugin.host.write_env_add_flags(plugin.host.usr, env_idx, flags);
}

inline
auto default_max(const Plugin& plugin, blink_EnvIdx env_idx, float value) -> void {
	plugin.host.write_env_default_max(plugin.host.usr, env_idx, value);
}

inline
auto default_min(const Plugin& plugin, blink_EnvIdx env_idx, float value) -> void {
	plugin.host.write_env_default_min(plugin.host.usr, env_idx, value);
}

inline
auto default_value(const Plugin& plugin, blink_EnvIdx env_idx, float value) -> void {
	plugin.host.write_env_default_value(plugin.host.usr, env_idx, value);
}

inline
auto fns(const Plugin& plugin, blink_EnvIdx env_idx, blink_EnvFns value) -> void {
	plugin.host.write_env_fns(plugin.host.usr, env_idx, value);
}

inline
auto max_slider(const Plugin& plugin, blink_EnvIdx env_idx, blink_SliderRealIdx sld_idx) -> void {
	plugin.host.write_env_max_slider(plugin.host.usr, env_idx, sld_idx);
}

inline
auto min_slider(const Plugin& plugin, blink_EnvIdx env_idx, blink_SliderRealIdx sld_idx) -> void {
	plugin.host.write_env_min_slider(plugin.host.usr, env_idx, sld_idx);
}

inline
auto value_slider(const Plugin& plugin, blink_EnvIdx env_idx, blink_SliderRealIdx sld_idx) -> void {
	plugin.host.write_env_value_slider(plugin.host.usr, env_idx, sld_idx);
}

} // env

namespace slider {

inline
auto default_value(const Plugin& plugin, blink_SliderIntIdx sld_idx, int64_t value) -> void {
	plugin.host.write_slider_int_default_value(plugin.host.usr, sld_idx, value);
}

inline
auto default_value(const Plugin& plugin, blink_SliderRealIdx sld_idx, float value) -> void {
	plugin.host.write_slider_real_default_value(plugin.host.usr, sld_idx, value);
}

inline
auto tweaker(const Plugin& plugin, blink_SliderIntIdx sld_idx, blink_TweakerInt value) -> void {
	plugin.host.write_slider_int_tweaker(plugin.host.usr, sld_idx, value);
}

inline
auto tweaker(const Plugin& plugin, blink_SliderRealIdx sld_idx, blink_TweakerReal value) -> void {
	plugin.host.write_slider_real_tweaker(plugin.host.usr, sld_idx, value);
}

} // slider

namespace param {

inline
auto add_flags(const Plugin& plugin, blink_ParamIdx param_idx, int flags) -> void {
	plugin.host.write_param_add_flags(plugin.host.usr, plugin.index, param_idx, flags);
}

inline
auto apply_offset_fn(const Plugin& plugin, blink_ParamIdx param_idx, blink_ApplyOffsetFn fn) -> void {
	plugin.host.write_param_env_apply_offset_fn(plugin.host.usr, plugin.index, param_idx, fn);
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
auto long_desc(const Plugin& plugin, blink_ParamIdx param_idx, blink_StaticString long_desc) -> void {
	plugin.host.write_param_long_desc(plugin.host.usr, plugin.index, param_idx, long_desc);
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
auto strings(const Plugin& plugin, blink_ParamIdx option_idx, StringVec strings) -> void {
	for (const auto string : strings.value) {
		plugin.host.write_param_option_add_string(plugin.host.usr, plugin.index, option_idx, {string.c_str()});
	}
}

inline
auto env(const Plugin& plugin, blink_ParamIdx param_idx, blink_EnvIdx env_idx) -> void {
	plugin.host.write_param_env_env(plugin.host.usr, plugin.index, param_idx, env_idx);
}

inline
auto clamp_range(const Plugin& plugin, blink_ParamIdx param_idx, blink_Range range) -> void {
	plugin.host.write_param_env_clamp_range(plugin.host.usr, plugin.index, param_idx, range);
}

inline
auto offset_env(const Plugin& plugin, blink_ParamIdx param_idx, blink_EnvIdx env_idx) -> void {
	plugin.host.write_param_env_offset_env(plugin.host.usr, plugin.index, param_idx, env_idx);
}

inline
auto override_env(const Plugin& plugin, blink_ParamIdx param_idx, blink_EnvIdx env_idx) -> void {
	plugin.host.write_param_env_override_env(plugin.host.usr, plugin.index, param_idx, env_idx);
}

inline
auto uuid(const Plugin& plugin, blink_ParamIdx param_idx, blink_UUID uuid) -> void {
	plugin.host.write_param_uuid(plugin.host.usr, plugin.index, param_idx, uuid);
}

} // param
} // write

[[nodiscard]] inline
auto make_chord_data(const Plugin& plugin, const blink_ParamData* param_data, blink_ParamIdx param_idx) -> ChordData {
	ChordData out;
	out.data = &param_data[param_idx.value].chord;
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
		out.data = &param_data[param_idx.value].envelope;
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
		out.data = &param_data[param_idx.value].option;
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
		out.data = &param_data[param_idx.value].slider_int;
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
		out.data = &param_data[param_idx.value].slider_real;
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
