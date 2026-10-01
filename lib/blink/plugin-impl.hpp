#pragma once

#include <ent.hpp>
#include "blink.h"
#include "block_positions.hpp"
#include "common_impl.hpp"
#include "data.hpp"
#include "resource_store.hpp"
#include "types.hpp"

namespace blink {

// I have no idea how this should be tuned.
static constexpr auto BLOCK_SIZE = 100;

template <typename... Ts>
using Instance = ent::table<
	"blink:plugin:instance",
	BLOCK_SIZE,
	UnitVec,
	Ts...
>;

template <typename... Ts>
using Unit = ent::table<
	"blink:plugin:unit",
	BLOCK_SIZE,
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

auto init(Plugin* plugin, blink_PluginIdx plugin_index, blink_HostFns host_fns) -> void;
auto init(Plugin* plugin, blink_PluginIdx plugin_index, blink_HostFns host_fns, blink_SamplerInfo sampler_info) -> void;

template <typename Instance, typename Unit> [[nodiscard]]
auto terminate(Entities<Instance, Unit>* ents) -> blink_Error {
	ents->instance.clear(ent::lock);
	ents->unit.clear(ent::lock);
	return BLINK_OK;
}

template <typename Instance, typename Unit> [[nodiscard]]
auto add_unit(Entities<Instance, Unit>* ents, blink_InstanceIdx instance_idx) -> blink_UnitIdx {
	const auto index = blink_UnitIdx{ents->unit.acquire(ent::lock)};
	ents->unit.template get<blink_InstanceIdx>(index.value) = instance_idx;
	auto& units = ents->instance.template get<UnitVec>(instance_idx.value);
	units.value.push_back(index);
	return index;
}

template <typename Instance, typename Unit> [[nodiscard]]
auto make_instance(Entities<Instance, Unit>* ents) -> blink_InstanceIdx {
	return {ents->instance.acquire(ent::lock)};
}

template <typename Instance, typename Unit> [[nodiscard]]
auto destroy_instance(Entities<Instance, Unit>* ents, blink_InstanceIdx instance_idx) -> blink_Error {
	for (auto unit_idx : ents->instance.template get<UnitVec>(instance_idx.value).value) {
		ents->unit.release(ent::lock, unit_idx.value);
	}
	ents->instance.release(ent::lock, instance_idx.value);
	return BLINK_OK;
}

auto get_std_error_string(blink_StdError error) -> const char*;

} // blink

namespace blink::read {

auto env(const Plugin& plugin, blink_ParamIdx param_idx) -> blink_EnvIdx;
auto slider_real(const Plugin& plugin, blink_ParamIdx param_idx) -> blink_SliderRealIdx;

} // blink::read

namespace blink::add {

auto frequency_response(const Plugin& plugin, const blink_FrequencyResponseInfo& info) -> blink_FrequencyResponseIdx;

} // blink::add

namespace blink::add::param {

auto chord(const Plugin& plugin, blink_UUID uuid) -> blink_ParamIdx;
auto env(const Plugin& plugin, blink_UUID uuid) -> blink_ParamIdx;
auto option(const Plugin& plugin, blink_UUID uuid) -> blink_ParamIdx;
auto slider_int(const Plugin& plugin, blink_UUID uuid) -> blink_ParamIdx;
auto slider_real(const Plugin& plugin, blink_UUID uuid) -> blink_ParamIdx;

} // blink::add::param

namespace blink::write::env {

auto add_flags(const Plugin& plugin, blink_EnvIdx env_idx, int flags) -> void;
auto default_max(const Plugin& plugin, blink_EnvIdx env_idx, float value) -> void;
auto default_min(const Plugin& plugin, blink_EnvIdx env_idx, float value) -> void;
auto default_value(const Plugin& plugin, blink_EnvIdx env_idx, float value) -> void;
auto fns(const Plugin& plugin, blink_EnvIdx env_idx, blink_EnvFns value) -> void;
auto max_slider(const Plugin& plugin, blink_EnvIdx env_idx, blink_SliderRealIdx sld_idx) -> void;
auto min_slider(const Plugin& plugin, blink_EnvIdx env_idx, blink_SliderRealIdx sld_idx) -> void;
auto value_slider(const Plugin& plugin, blink_EnvIdx env_idx, blink_SliderRealIdx sld_idx) -> void;

} // blink::write::env

namespace blink::write::slider {

auto default_value(const Plugin& plugin, blink_SliderIntIdx sld_idx, int64_t value) -> void;
auto default_value(const Plugin& plugin, blink_SliderRealIdx sld_idx, float value) -> void;
auto tweaker(const Plugin& plugin, blink_SliderIntIdx sld_idx, blink_TweakerInt value) -> void;
auto tweaker(const Plugin& plugin, blink_SliderRealIdx sld_idx, blink_TweakerReal value) -> void;

} // blink::write::slider

namespace blink::write::param {

auto add_flags(const Plugin& plugin, blink_ParamIdx param_idx, int flags) -> void;
auto apply_offset_fn(const Plugin& plugin, blink_ParamIdx param_idx, blink_ApplyOffsetFn fn) -> void;
auto manip_delegate(const Plugin& plugin, blink_ParamIdx param_idx, blink_ParamIdx delegate_idx) -> void;
auto add_subparam(const Plugin& plugin, blink_ParamIdx param_idx, blink_ParamIdx subparam_idx) -> void;
auto group(const Plugin& plugin, blink_ParamIdx param_idx, blink_StaticString group_name) -> void;
auto long_desc(const Plugin& plugin, blink_ParamIdx param_idx, blink_StaticString long_desc) -> void;
auto option_default_value(const Plugin& plugin, blink_ParamIdx option_idx, int64_t value) -> void;
auto name(const Plugin& plugin, blink_ParamIdx param_idx, blink_StaticString name) -> void;
auto short_name(const Plugin& plugin, blink_ParamIdx param_idx, blink_StaticString name) -> void;
auto slider(const Plugin& plugin, blink_ParamIdx param_idx, blink_SliderRealIdx sld_idx) -> void;
auto strings(const Plugin& plugin, blink_ParamIdx option_idx, StringVec strings) -> void;
auto env(const Plugin& plugin, blink_ParamIdx param_idx, blink_EnvIdx env_idx) -> void;
auto clamp_range(const Plugin& plugin, blink_ParamIdx param_idx, blink_Range range) -> void;
auto offset_env(const Plugin& plugin, blink_ParamIdx param_idx, blink_EnvIdx env_idx) -> void;
auto override_env(const Plugin& plugin, blink_ParamIdx param_idx, blink_EnvIdx env_idx) -> void;
auto uuid(const Plugin& plugin, blink_ParamIdx param_idx, blink_UUID uuid) -> void;

} // blink::write::param

namespace blink {

[[nodiscard]] auto make_int_value(const blink_IntPoints& points, int64_t default_value) -> int64_t;
[[nodiscard]] auto make_real_value(const blink_RealPoints& points, float default_value) -> float;
[[nodiscard]] auto make_chord_data(const Plugin& plugin, const blink_UniformParamData* param_data, blink_ParamIdx param_idx) -> uniform::Chord;
[[nodiscard]] auto make_env_data(const Plugin& plugin, const blink_UniformParamData* param_data, blink_ParamIdx param_idx) -> uniform::Env;
[[nodiscard]] auto make_option_data(const Plugin& plugin, const blink_UniformParamData* param_data, blink_ParamIdx param_idx) -> uniform::Option;
[[nodiscard]] auto make_slider_int_data(const Plugin& plugin, const blink_UniformParamData* param_data, blink_ParamIdx param_idx) -> uniform::SliderInt;
[[nodiscard]] auto make_slider_real_data(const Plugin& plugin, const blink_UniformParamData* param_data, blink_ParamIdx param_idx) -> uniform::SliderReal;

template <class FileSystem> [[nodiscard]]
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
