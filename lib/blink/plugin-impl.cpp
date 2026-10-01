#include "plugin-impl.hpp"

namespace blink {

auto init(Plugin* plugin, blink_PluginIdx plugin_index, blink_HostFns host_fns) -> void {
	plugin->index = plugin_index;
	plugin->host = host_fns;
}

auto init(Plugin* plugin, blink_PluginIdx plugin_index, blink_HostFns host_fns, blink_SamplerInfo sampler_info) -> void {
	init(plugin, plugin_index, host_fns);
	host_fns.write_sampler_info(host_fns.usr, plugin_index, sampler_info);
}

auto get_std_error_string(blink_StdError error) -> const char* {
	switch (error) {
		case blink_StdError_AlreadyInitialized: return "already initialized";
		case blink_StdError_NotInitialized: return "not initialized";
		case blink_StdError_NotImplemented: return "not implemented";
		case blink_StdError_InvalidInstance: return "invalid instance";
		default: return "unknown error";
	}
}

} // blink

namespace blink::read {

auto env(const Plugin& plugin, blink_ParamIdx param_idx) -> blink_EnvIdx {
	return plugin.host.read_param_env_env(plugin.host.usr, plugin.index, param_idx);
}

auto slider_real(const Plugin& plugin, blink_ParamIdx param_idx) -> blink_SliderRealIdx {
	return plugin.host.read_param_slider_real_slider(plugin.host.usr, plugin.index, param_idx);
}

} // blink::read

namespace blink::add {

auto frequency_response(const Plugin& plugin, const blink_FrequencyResponseInfo& info) -> blink_FrequencyResponseIdx {
	return plugin.host.add_frequency_response(plugin.host.usr, plugin.index, &info);
}

} // blink::add

namespace blink::add::param {

auto chord(const Plugin& plugin, blink_UUID uuid) -> blink_ParamIdx {
	return plugin.host.add_param_chord(plugin.host.usr, plugin.index, uuid);
}

auto env(const Plugin& plugin, blink_UUID uuid) -> blink_ParamIdx {
	return plugin.host.add_param_env(plugin.host.usr, plugin.index, uuid);
}

auto option(const Plugin& plugin, blink_UUID uuid) -> blink_ParamIdx {
	return plugin.host.add_param_option(plugin.host.usr, plugin.index, uuid);
}

auto slider_int(const Plugin& plugin, blink_UUID uuid) -> blink_ParamIdx {
	return plugin.host.add_param_slider_int(plugin.host.usr, plugin.index, uuid);
}

auto slider_real(const Plugin& plugin, blink_UUID uuid) -> blink_ParamIdx {
	return plugin.host.add_param_slider_real(plugin.host.usr, plugin.index, uuid);
}

} // blink::add::param

namespace blink::write::env {

auto add_flags(const Plugin& plugin, blink_EnvIdx env_idx, int flags) -> void {
	plugin.host.write_env_add_flags(plugin.host.usr, env_idx, flags);
}

auto default_max(const Plugin& plugin, blink_EnvIdx env_idx, float value) -> void {
	plugin.host.write_env_default_max(plugin.host.usr, env_idx, value);
}

auto default_min(const Plugin& plugin, blink_EnvIdx env_idx, float value) -> void {
	plugin.host.write_env_default_min(plugin.host.usr, env_idx, value);
}

auto default_value(const Plugin& plugin, blink_EnvIdx env_idx, float value) -> void {
	plugin.host.write_env_default_value(plugin.host.usr, env_idx, value);
}

auto fns(const Plugin& plugin, blink_EnvIdx env_idx, blink_EnvFns value) -> void {
	plugin.host.write_env_fns(plugin.host.usr, env_idx, value);
}

auto max_slider(const Plugin& plugin, blink_EnvIdx env_idx, blink_SliderRealIdx sld_idx) -> void {
	plugin.host.write_env_max_slider(plugin.host.usr, env_idx, sld_idx);
}

auto min_slider(const Plugin& plugin, blink_EnvIdx env_idx, blink_SliderRealIdx sld_idx) -> void {
	plugin.host.write_env_min_slider(plugin.host.usr, env_idx, sld_idx);
}

auto value_slider(const Plugin& plugin, blink_EnvIdx env_idx, blink_SliderRealIdx sld_idx) -> void {
	plugin.host.write_env_value_slider(plugin.host.usr, env_idx, sld_idx);
}

} // blink::write::env

namespace blink::write::slider {

auto default_value(const Plugin& plugin, blink_SliderIntIdx sld_idx, int64_t value) -> void {
	plugin.host.write_slider_int_default_value(plugin.host.usr, sld_idx, value);
}

auto default_value(const Plugin& plugin, blink_SliderRealIdx sld_idx, float value) -> void {
	plugin.host.write_slider_real_default_value(plugin.host.usr, sld_idx, value);
}

auto tweaker(const Plugin& plugin, blink_SliderIntIdx sld_idx, blink_TweakerInt value) -> void {
	plugin.host.write_slider_int_tweaker(plugin.host.usr, sld_idx, value);
}

auto tweaker(const Plugin& plugin, blink_SliderRealIdx sld_idx, blink_TweakerReal value) -> void {
	plugin.host.write_slider_real_tweaker(plugin.host.usr, sld_idx, value);
}

} // blink::write::slider

namespace blink::write::param {

auto add_flags(const Plugin& plugin, blink_ParamIdx param_idx, int flags) -> void {
	plugin.host.write_param_add_flags(plugin.host.usr, plugin.index, param_idx, flags);
}

auto apply_offset_fn(const Plugin& plugin, blink_ParamIdx param_idx, blink_ApplyOffsetFn fn) -> void {
	plugin.host.write_param_env_apply_offset_fn(plugin.host.usr, plugin.index, param_idx, fn);
}

auto manip_delegate(const Plugin& plugin, blink_ParamIdx param_idx, blink_ParamIdx delegate_idx) -> void {
	plugin.host.write_param_manip_delegate(plugin.host.usr, plugin.index, param_idx, delegate_idx);
}

auto add_subparam(const Plugin& plugin, blink_ParamIdx param_idx, blink_ParamIdx subparam_idx) -> void {
	plugin.host.write_param_add_subparam(plugin.host.usr, plugin.index, param_idx, subparam_idx);
}

auto group(const Plugin& plugin, blink_ParamIdx param_idx, blink_StaticString group_name) -> void {
	plugin.host.write_param_group(plugin.host.usr, plugin.index, param_idx, group_name);
}

auto long_desc(const Plugin& plugin, blink_ParamIdx param_idx, blink_StaticString long_desc) -> void {
	plugin.host.write_param_long_desc(plugin.host.usr, plugin.index, param_idx, long_desc);
}

auto option_default_value(const Plugin& plugin, blink_ParamIdx option_idx, int64_t value) -> void {
	plugin.host.write_param_option_default_value(plugin.host.usr, plugin.index, option_idx, value);
}

auto name(const Plugin& plugin, blink_ParamIdx param_idx, blink_StaticString name) -> void {
	plugin.host.write_param_name(plugin.host.usr, plugin.index, param_idx, name);
}

auto short_name(const Plugin& plugin, blink_ParamIdx param_idx, blink_StaticString name) -> void {
	plugin.host.write_param_short_name(plugin.host.usr, plugin.index, param_idx, name);
}

auto slider(const Plugin& plugin, blink_ParamIdx param_idx, blink_SliderRealIdx sld_idx) -> void {
	plugin.host.write_param_slider_real_slider(plugin.host.usr, plugin.index, param_idx, sld_idx);
}

auto strings(const Plugin& plugin, blink_ParamIdx option_idx, StringVec strings) -> void {
	for (const auto string : strings.value) {
		plugin.host.write_param_option_add_string(plugin.host.usr, plugin.index, option_idx, {string.c_str()});
	}
}

auto env(const Plugin& plugin, blink_ParamIdx param_idx, blink_EnvIdx env_idx) -> void {
	plugin.host.write_param_env_env(plugin.host.usr, plugin.index, param_idx, env_idx);
}

auto clamp_range(const Plugin& plugin, blink_ParamIdx param_idx, blink_Range range) -> void {
	plugin.host.write_param_env_clamp_range(plugin.host.usr, plugin.index, param_idx, range);
}

auto offset_env(const Plugin& plugin, blink_ParamIdx param_idx, blink_EnvIdx env_idx) -> void {
	plugin.host.write_param_env_offset_env(plugin.host.usr, plugin.index, param_idx, env_idx);
}

auto override_env(const Plugin& plugin, blink_ParamIdx param_idx, blink_EnvIdx env_idx) -> void {
	plugin.host.write_param_env_override_env(plugin.host.usr, plugin.index, param_idx, env_idx);
}

auto uuid(const Plugin& plugin, blink_ParamIdx param_idx, blink_UUID uuid) -> void {
	plugin.host.write_param_uuid(plugin.host.usr, plugin.index, param_idx, uuid);
}

} // blink::write::param

namespace blink {

auto make_int_value(const blink_IntPoints& points, int64_t default_value) -> int64_t {
	return points.count > 0 ? points.data[0].y : default_value;
}

auto make_real_value(const blink_RealPoints& points, float default_value) -> float {
	return points.count > 0 ? points.data[0].y : default_value;
}

auto make_chord_data(const Plugin& plugin, const blink_UniformParamData* param_data, blink_ParamIdx param_idx) -> uniform::Chord {
	uniform::Chord out;
	if (param_data) {
		out.data = &param_data[param_idx.value].chord;
	}
	else {
		out.data = nullptr;
	}
	return out;
}

auto make_env_data(const Plugin& plugin, const blink_UniformParamData* param_data, blink_ParamIdx param_idx) -> uniform::Env {
	uniform::Env out;
	if (param_data) {
		out.data = &param_data[param_idx.value].env;
		out.default_value = out.data->default_value;
		out.value = make_real_value(out.data->points, out.default_value);
	}
	else {
		out.data = nullptr;
		out.default_value = plugin.host.read_param_env_default_value(plugin.host.usr, plugin.index, param_idx);
		out.value = out.default_value;
	}
	return out;
}

auto make_option_data(const Plugin& plugin, const blink_UniformParamData* param_data, blink_ParamIdx param_idx) -> uniform::Option {
	uniform::Option out;
	if (param_data) {
		out.data = &param_data[param_idx.value].option;
		out.default_value = out.data->default_value;
		out.value = make_int_value(out.data->points, out.default_value);
	}
	else {
		out.data = nullptr;
		out.default_value = plugin.host.read_param_option_default_value(plugin.host.usr, plugin.index, param_idx);
		out.value = out.default_value;
	}
	return out;
}

auto make_slider_int_data(const Plugin& plugin, const blink_UniformParamData* param_data, blink_ParamIdx param_idx) -> uniform::SliderInt {
	uniform::SliderInt out;
	if (param_data) {
		out.data = &param_data[param_idx.value].slider_int;
		out.default_value = out.data->default_value;
		out.value = make_int_value(out.data->points, out.default_value);
	}
	else {
		out.data = nullptr;
		out.default_value = plugin.host.read_param_slider_int_default_value(plugin.host.usr, plugin.index, param_idx);
		out.value = out.default_value;
	}
	return out;
}

auto make_slider_real_data(const Plugin& plugin, const blink_UniformParamData* param_data, blink_ParamIdx param_idx) -> uniform::SliderReal {
	uniform::SliderReal out;
	if (param_data) {
		out.data = &param_data[param_idx.value].slider_real;
		out.default_value = out.data->default_value;
		out.value = make_real_value(out.data->points, out.default_value);
	}
	else {
		out.data = nullptr;
		out.default_value = plugin.host.read_param_slider_real_default_value(plugin.host.usr, plugin.index, param_idx);
		out.value = out.default_value;
	}
	return out;
}

} // blink
