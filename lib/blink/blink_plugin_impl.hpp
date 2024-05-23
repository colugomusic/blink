#pragma once

#include "blink.h"
#include "block_positions.hpp"
#include "entity_store.hpp"
#include "types.hpp"

namespace blink {

namespace ent {
	template <typename... Ts>
	using Instance = DynamicEntityStore<
		UnitVec,
		Ts...
	>;
	template <typename... Ts>
	using Unit = DynamicEntityStore<
		Ts...
	>;
} // ent

/* TODO: move to host
namespace ent {
	using Instance = DynamicEntityStore<
		SR,
		UnitVec,
		InstanceProcess
	>;
	using Unit = DynamicEntityStore<
		UnitProcess
	>;
} // ent
*/

struct Plugin {
	blink_PluginIdx index;
	blink_HostFns host;
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

/* TODO: move this logic to host
template <typename InstanceResetFn>
auto begin_process(Plugin* plugin, blink_InstanceIdx instance_idx, blink_BufferID buffer_id, InstanceResetFn&& instance_reset_fn) -> void {
	// instance is reset at the start of the buffer if we have gone
	// at least one buffer with no units being processed
	auto& instance_process = plugin->instance.get<InstanceProcess>(instance_idx.value);
	if (buffer_id.value > instance_process.buffer_id.value.value) {
		if (buffer_id.value > instance_process.buffer_id.value.value + 1 || instance_process.active_buffer_units == 0) {
			instance_reset_fn(instance_idx);
		}
		instance_process.buffer_id.value     = buffer_id;
		instance_process.active_buffer_units = 0;
	}
	instance_process.active_buffer_units++;
}
*/

namespace add {
namespace param {

inline
auto env(Plugin* plugin, blink_UUID uuid) -> blink_ParamIdx {
	return plugin->host.add_param_env(plugin->index, uuid);
}

inline
auto option(Plugin* plugin, blink_UUID uuid) -> blink_ParamIdx {
	return plugin->host.add_param_option(plugin->index, uuid);
}

inline
auto slider_int(Plugin* plugin, blink_UUID uuid) -> blink_ParamIdx {
	return plugin->host.add_param_slider_int(plugin->index, uuid);
}

inline
auto slider_real(Plugin* plugin, blink_UUID uuid) -> blink_ParamIdx {
	return plugin->host.add_param_slider_real(plugin->index, uuid);
}

} // param
} // add

namespace read {
namespace env {

[[nodiscard]] inline
auto default_value(const Plugin& plugin, blink_EnvIdx env_idx) -> float {
	return plugin.host.read_env_default_value(plugin.index, env_idx);
}

} // env

namespace slider_int {

[[nodiscard]] inline
auto default_value(const Plugin& plugin, blink_SliderIntIdx slider_int_idx) -> int64_t {
	return plugin.host.read_slider_int_default_value(plugin.index, slider_int_idx);
}

} // slider_int

namespace slider_real {

[[nodiscard]] inline
auto default_value(const Plugin& plugin, blink_SliderRealIdx slider_real_idx) -> float {
	return plugin.host.read_slider_real_default_value(plugin.index, slider_real_idx);
}

} // slider_real

namespace param {
namespace env {

[[nodiscard]] inline
auto env_idx(const Plugin& plugin, blink_ParamIdx param_idx) -> blink_EnvIdx {
	return plugin.host.read_param_env_env_idx(plugin.index, param_idx);
}

[[nodiscard]] inline
auto default_value(const Plugin& plugin, blink_ParamIdx param_idx) -> float {
	return read::env::default_value(plugin, env_idx(plugin, param_idx));
}

} // env
namespace option {

[[nodiscard]] inline
auto default_value(const Plugin& plugin, blink_ParamIdx param_idx) -> int64_t {
	return plugin.host.read_param_option_default_value(plugin.index, param_idx);
}

} // option
namespace slider_int {

[[nodiscard]] inline
auto slider_int_idx(const Plugin& plugin, blink_ParamIdx param_idx) -> blink_SliderIntIdx {
	return plugin.host.read_param_slider_int_slider_idx(plugin.index, param_idx);
}

[[nodiscard]] inline
auto default_value(const Plugin& plugin, blink_ParamIdx param_idx) -> int64_t {
	return read::slider_int::default_value(plugin, slider_int_idx(plugin, param_idx));
}

} // slider_int
namespace slider_real {

[[nodiscard]] inline
auto slider_real_idx(const Plugin& plugin, blink_ParamIdx param_idx) -> blink_SliderRealIdx {
	return plugin.host.read_param_slider_real_slider_idx(plugin.index, param_idx);
}

[[nodiscard]] inline
auto default_value(const Plugin& plugin, blink_ParamIdx param_idx) -> float {
	return read::slider_real::default_value(plugin, slider_real_idx(plugin, param_idx));
}

} // slider_real
} // param
} // read

namespace write {
namespace param {

inline
auto add_flags(Plugin* plugin, blink_ParamIdx param_idx, blink_ParamFlags flags) -> void {
	plugin->host.write_param_add_flags(plugin->index, param_idx, flags);
}

inline
auto manip_delegate(Plugin* plugin, blink_ParamIdx param_idx, blink_ParamIdx delegate_idx) -> void {
	plugin->host.write_param_delegate(plugin->index, param_idx, delegate_idx);
}

inline
auto add_subparam(Plugin* plugin, blink_ParamIdx param_idx, blink_ParamIdx subparam_idx) -> void {
	plugin->host.write_param_add_subparam(plugin->index, param_idx, subparam_idx);
}

inline
auto group(Plugin* plugin, blink_ParamIdx param_idx, blink_StaticString group_name) -> void {
	plugin->host.write_param_group(plugin->index, param_idx, group_name);
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
	out.default_value = read::param::env::default_value(plugin, param_idx);
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
	out.default_value = read::param::option::default_value(plugin, param_idx);
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
	out.default_value = read::param::slider_int::default_value(plugin, param_idx);
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
	out.default_value = read::param::slider_real::default_value(plugin, param_idx);
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

} // blink
