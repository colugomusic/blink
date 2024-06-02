#pragma once

#include <blink.h>
#include <blink_std.h>
#include <cassert>
#include <ent.hpp>
#include "common_impl.hpp"
#include "math.hpp"
#include "tweak.hpp"
#include "types.hpp"
#include "uuids.h"

namespace blink {

namespace e {
	using Plugin = ent::static_store<
		blink_PluginInfo,
		PluginType,
		PluginTypeIdx,
		PluginInterface,
		PluginParams,
		std::vector<GroupInfo>
	>;
	using PluginSampler = ent::static_store<
		SamplerInfo
	>;
	using Instance = ent::dynamic_store<
		blink_PluginIdx,
		InstanceProcess,
		UnitVec
	>;
	using Unit = ent::dynamic_store<
		blink_PluginIdx,
		UnitProcess
	>;
	using Param = ent::static_store<
		blink_PluginIdx,
		blink_UUID,
		ManipDelegate,
		ParamFlags,
		ParamIcon,
		ParamStrings,
		ParamType,
		ParamTypeIdx,
		SubParams
	>;
	using ParamEnv = ent::static_store<
		ApplyOffsetFn,
		ClampRange,
		EnvIdx,
		OffsetEnvIdx,
		OverrideEnvIdx
	>;
	using ParamOption = ent::static_store<
		DefaultValue<int64_t>,
		StringVec
	>;
	using ParamSliderInt = ent::static_store<
		blink_SliderIntIdx
	>;
	using ParamSliderReal = ent::static_store<
		ApplyOffsetFn,
		blink_SliderRealIdx,
		ClampRange,
		OffsetEnvIdx,
		OverrideEnvIdx
	>;
	using Env = ent::static_store<
		DefaultMax<float>,
		DefaultMin<float>,
		DefaultValue<float>,
		DefaultSnapAmount,
		EnvFlags,
		EnvFns,
		MaxSliderIdx,
		MinSliderIdx,
		StepSizeSliderIdx,
		ValueSliderIdx
	>;
	using SliderInt = ent::static_store<
		DefaultValue<int64_t>,
		TweakerInt
	>;
	using SliderReal = ent::static_store<
		DefaultValue<float>,
		TweakerReal
	>;
} // e

struct Host {
	e::Plugin plugin;
	e::PluginSampler plugin_sampler;
	e::Instance instance;
	e::Unit unit;
	e::Param param;
	e::ParamEnv param_env;
	e::ParamOption param_option;
	e::ParamSliderInt param_slider_int;
	e::ParamSliderReal param_slider_real;
	e::Env env;
	e::SliderInt slider_int;
	e::SliderReal slider_real;
	std::optional<blink_PluginIdx> default_sampler;
	blink_HostFns fns;
};

// Just for convenience. You could define this in your application somewhere and have it
// auto declared anywhere that this header is included.
[[nodiscard]] extern auto host() -> Host&;

namespace read {

[[nodiscard]] inline
auto apply_offset_fn(const Host& host, ParamEnvIdx param_env_idx) -> ApplyOffsetFn {
	return host.param_env.get<ApplyOffsetFn>(param_env_idx.value);
}

[[nodiscard]] inline
auto apply_offset_fn(const Host& host, ParamSliderRealIdx param_sld_idx) -> ApplyOffsetFn {
	return host.param_slider_real.get<ApplyOffsetFn>(param_sld_idx.value);
}

[[nodiscard]] inline
auto clamp_range(const Host& host, ParamEnvIdx param_env_idx) -> std::optional<blink_Range> {
	return host.param_env.get<ClampRange>(param_env_idx.value).value;
}

[[nodiscard]] inline
auto clamp_range(const Host& host, ParamSliderRealIdx param_sld_idx) -> std::optional<blink_Range> {
	return host.param_slider_real.get<ClampRange>(param_sld_idx.value).value;
}

[[nodiscard]] inline
auto default_max(const Host& host, blink_EnvIdx env_idx) -> float {
	return host.env.get<DefaultMax<float>>(env_idx.value).value;
}

[[nodiscard]] inline
auto default_min(const Host& host, blink_EnvIdx env_idx) -> float {
	return host.env.get<DefaultMin<float>>(env_idx.value).value;
}

[[nodiscard]] inline
auto default_snap_amount(const Host& host, blink_EnvIdx env_idx) -> float {
	return host.env.get<DefaultSnapAmount>(env_idx.value).value;
}

[[nodiscard]] inline
auto default_value(const Host& host, blink_EnvIdx env_idx) -> float {
	return host.env.get<DefaultValue<float>>(env_idx.value).value;
}

[[nodiscard]] inline
auto default_value(const Host& host, blink_SliderIntIdx sld_idx) -> int64_t {
	return host.slider_int.get<DefaultValue<int64_t>>(sld_idx.value).value;
}

[[nodiscard]] inline
auto default_value(const Host& host, blink_SliderRealIdx sld_idx) -> float {
	return host.slider_real.get<DefaultValue<float>>(sld_idx.value).value;
}

[[nodiscard]] inline
auto default_value(const Host& host, ParamOptionIdx option_idx) -> int64_t {
	return host.param_option.get<DefaultValue<int64_t>>(option_idx.value).value;
}

[[nodiscard]] inline
auto env(const Host& host, ParamEnvIdx param_env_idx) -> blink_EnvIdx {
	return host.param_env.get<EnvIdx>(param_env_idx.value).value;
}

[[nodiscard]] inline
auto env_offset(const Host& host, ParamEnvIdx param_env_idx) -> blink_EnvIdx {
	return host.param_env.get<OffsetEnvIdx>(param_env_idx.value).value.value;
}

[[nodiscard]] inline
auto env_offset(const Host& host, ParamSliderRealIdx param_sld_idx) -> blink_EnvIdx {
	return host.param_slider_real.get<OffsetEnvIdx>(param_sld_idx.value).value.value;
}

[[nodiscard]] inline
auto env_override(const Host& host, ParamEnvIdx param_env_idx) -> blink_EnvIdx {
	return host.param_env.get<OverrideEnvIdx>(param_env_idx.value).value.value;
}

[[nodiscard]] inline
auto env_override(const Host& host, ParamSliderRealIdx param_sld_idx) -> blink_EnvIdx {
	return host.param_slider_real.get<OverrideEnvIdx>(param_sld_idx.value).value.value;
}

[[nodiscard]] inline
auto find_param(const Host& host, blink_PluginIdx plugin, uuids::t uuid) -> std::optional<blink_ParamIdx> {
	const auto param_list = host.plugin.get<PluginParams>(plugin.value).global_indices;
	const auto uuid_str   = uuids::to<std::string>(uuid);
	if (!uuid_str) {
		return std::nullopt;
	}
	for (const auto param_idx : param_list) {
		const auto uuid = host.param.get<blink_UUID>(param_idx.value).value;
		if (uuid == *uuid_str) {
			return blink_ParamIdx{param_idx.value};
		}
	}
	return std::nullopt;
}

[[nodiscard]] inline
auto find_plugin(const Host& host, uuids::t uuid) -> std::optional<blink_PluginIdx> {
	const auto uuid_str = uuids::to<std::string>(uuid);
	if (!uuid_str) {
		return std::nullopt;
	}
	const auto pred = [uuid_str = std::string_view{*uuid_str}](const blink_PluginInfo& info) {
		return info.uuid.value && std::string_view{info.uuid.value} == uuid_str;
	};
	const auto idx = host.plugin.find<blink_PluginInfo>(pred);
	if (!idx) {
		return std::nullopt;
	}
	return blink_PluginIdx{*idx};
}

[[nodiscard]] inline
auto flags(const Host& host, blink_EnvIdx env_idx) -> int {
	return host.env.get<EnvFlags>(env_idx.value).value;
}

[[nodiscard]] inline
auto flags(const Host& host, blink_ParamIdx param_idx) -> int {
	return host.param.get<ParamFlags>(param_idx.value).value.value;
}

[[nodiscard]] inline
auto fns(const Host& host, blink_EnvIdx env_idx) -> blink_EnvFns {
	return host.env.get<EnvFns>(env_idx.value).value;
}

[[nodiscard]] inline
auto global_to_local(const Host& host, ParamGlobalIdx global_idx) -> ParamLocalIdx {
	const auto plugin_idx = host.param.get<blink_PluginIdx>(global_idx.value);
	const auto& all_params = host.plugin.get<PluginParams>(plugin_idx.value).global_indices;
	for (size_t idx = 0; idx < all_params.size(); ++idx) {
		if (all_params[idx] == global_idx) {
			return ParamLocalIdx{idx};
		}
	}
	assert (false);
	return {};
}

[[nodiscard]] inline
auto group_name(const Host& host, blink_ParamIdx param_idx) -> std::string_view {
	const auto& strings = host.param.get<ParamStrings>(param_idx.value);
	if (strings.group.value) {
		return strings.group.value;
	}
	return {};
}

[[nodiscard]] inline
auto icon(const Host& host, blink_ParamIdx param_idx) -> blink_StdIcon {
	return host.param.get<ParamIcon>(param_idx.value).value;
}

[[nodiscard]] inline
auto iface(const Host& host, blink_PluginIdx plugin_idx) -> const PluginInterface& {
	return host.plugin.get<PluginInterface>(plugin_idx.value);
}

[[nodiscard]] inline
auto info(const Host& host, blink_PluginIdx plugin_idx) -> const blink_PluginInfo& {
	return host.plugin.get<blink_PluginInfo>(plugin_idx.value);
}

[[nodiscard]] inline
auto local_to_global(const Host& host, blink_PluginIdx plugin_idx, blink_ParamIdx local_idx) -> ParamGlobalIdx {
	const auto& param_list = host.plugin.get<PluginParams>(plugin_idx.value).global_indices;
	return param_list[local_idx.value];
}

[[nodiscard]] inline
auto long_desc(const Host& host, blink_ParamIdx param_idx) -> std::string_view {
	const auto& strings = host.param.get<ParamStrings>(param_idx.value);
	if (strings.long_desc.value) {
		return strings.long_desc.value;
	}
	return {};
}

[[nodiscard]] inline
auto manip_delegate(const Host& host, blink_ParamIdx param_idx) -> std::optional<ParamGlobalIdx> {
	return host.param.get<ManipDelegate>(param_idx.value).value;
}

[[nodiscard]] inline
auto max_slider(const Host& host, blink_EnvIdx env_idx) -> std::optional<blink_SliderRealIdx> {
	return host.env.get<MaxSliderIdx>(env_idx.value).value;
}

[[nodiscard]] inline
auto min_slider(const Host& host, blink_EnvIdx env_idx) -> std::optional<blink_SliderRealIdx> {
	return host.env.get<MinSliderIdx>(env_idx.value).value;
}

[[nodiscard]] inline
auto name(const Host& host, blink_ParamIdx param_idx) -> std::string_view {
	const auto& strings = host.param.get<ParamStrings>(param_idx.value);
	if (strings.name.value) {
		return strings.name.value;
	}
	return {};
}

[[nodiscard]] inline
auto name(const Host& host, blink_PluginIdx plugin_idx) -> std::string_view {
	return host.plugin.get<blink_PluginInfo>(plugin_idx.value).name.value;
}

[[nodiscard]] inline
auto plugin_count(const Host& host) -> size_t {
	return host.plugin.size();
}

[[nodiscard]] inline
auto process(Host* host, blink_InstanceIdx instance_idx) -> InstanceProcess& {
	return host->instance.get<InstanceProcess>(instance_idx.value);
}

[[nodiscard]] inline
auto type(const Host& host, blink_PluginIdx plugin_idx) -> PluginType {
	return host.plugin.get<PluginType>(plugin_idx.value);
}

[[nodiscard]] inline
auto sampler_baked_waveform_could_be_different(const Host& host, blink_PluginIdx plugin_idx) -> bool {
	const auto type_idx     = host.plugin.get<PluginTypeIdx>(plugin_idx.value).value;
	const auto sampler_info = host.plugin_sampler.get<SamplerInfo>(type_idx);
	return sampler_info.value.baked_waveform_could_be_different.value;
}

[[nodiscard]] inline
auto sampler_requires_preprocessing(const Host& host, blink_PluginIdx plugin_idx) -> bool {
	const auto type_idx     = host.plugin.get<PluginTypeIdx>(plugin_idx.value).value;
	const auto sampler_info = host.plugin_sampler.get<SamplerInfo>(type_idx);
	return sampler_info.value.requires_preprocessing.value;
}

[[nodiscard]] inline
auto short_name(const Host& host, blink_ParamIdx param_idx) -> std::string_view {
	const auto& strings = host.param.get<ParamStrings>(param_idx.value);
	if (strings.short_name.value) {
		return strings.short_name.value;
	}
	return {};
}

[[nodiscard]] inline
auto slider(const Host& host, ParamSliderIntIdx param_env_idx) -> blink_SliderIntIdx {
	return host.param_slider_int.get<blink_SliderIntIdx>(param_env_idx.value);
}

[[nodiscard]] inline
auto slider(const Host& host, ParamSliderRealIdx param_env_idx) -> blink_SliderRealIdx {
	return host.param_slider_real.get<blink_SliderRealIdx>(param_env_idx.value);
}

[[nodiscard]] inline
auto step_size_slider(const Host& host, blink_EnvIdx env_idx) -> std::optional<blink_SliderRealIdx> {
	return host.env.get<StepSizeSliderIdx>(env_idx.value).value;
}

[[nodiscard]] inline
auto strings(const Host& host, ParamOptionIdx option_idx) -> const std::vector<std::string>& {
	return host.param_option.get<StringVec>(option_idx.value).value;
}

[[nodiscard]] inline
auto subparams(const Host& host, ParamGlobalIdx param_idx) -> const std::vector<ParamGlobalIdx>& {
	return host.param.get<SubParams>(param_idx.value).value;
}

[[nodiscard]] inline
auto tweaker(const Host& host, blink_SliderIntIdx sld_idx) -> const blink_TweakerInt& {
	return host.slider_int.get<TweakerInt>(sld_idx.value).value;
}

[[nodiscard]] inline
auto tweaker(const Host& host, blink_SliderRealIdx sld_idx) -> const blink_TweakerReal& {
	return host.slider_real.get<TweakerReal>(sld_idx.value).value;
}

[[nodiscard]] inline
auto type(const Host& host, ParamGlobalIdx param_idx) -> ParamType {
	return host.param.get<ParamType>(param_idx.value);
}

[[nodiscard]] inline
auto type_idx(const Host& host, ParamGlobalIdx param_idx) -> size_t {
	return host.param.get<ParamTypeIdx>(param_idx.value).value;
}

[[nodiscard]] inline
auto uuid(const Host& host, ParamGlobalIdx param_idx) -> blink_UUID {
	return host.param.get<blink_UUID>(param_idx.value);
}

[[nodiscard]] inline
auto value_slider(const Host& host, blink_EnvIdx env_idx) -> blink_SliderRealIdx {
	return host.env.get<ValueSliderIdx>(env_idx.value).value;
}

[[nodiscard]] inline
auto version(const Host& host, blink_PluginIdx plugin_idx) -> std::string_view {
	return host.plugin.get<blink_PluginInfo>(plugin_idx.value).version.value;
}

} // read

namespace write {

inline
auto add_flags(Host* host, blink_EnvIdx env_idx, int flags) -> void {
	host->env.get<EnvFlags>(env_idx.value).value |= flags;
}

inline
auto add_flags(Host* host, ParamGlobalIdx param_idx, int flags) -> void {
	host->param.get<ParamFlags>(param_idx.value).value.value |= flags;
}

inline
auto add_subparam(Host* host, ParamGlobalIdx param_idx, ParamGlobalIdx subparam_idx) -> void {
	host->param.get<SubParams>(param_idx.value).value.push_back(subparam_idx);
}

inline
auto apply_offset_fn(Host* host, ParamEnvIdx param_env_idx, blink_ApplyOffsetFn fn) -> void {
	host->param_env.set(param_env_idx.value, ApplyOffsetFn{fn});
}

inline
auto clamp_range(Host* host, ParamEnvIdx param_env_idx, ClampRange value) -> void {
	host->param_env.set(param_env_idx.value, value);
}

inline
auto clamp_range(Host* host, ParamSliderRealIdx param_sld_idx, ClampRange value) -> void {
	host->param_slider_real.set(param_sld_idx.value, value);
}

inline
auto default_max(Host* host, blink_EnvIdx env_idx, DefaultMax<float> value) -> void {
	host->env.set(env_idx.value, value);
}

inline
auto default_min(Host* host, blink_EnvIdx env_idx, DefaultMin<float> value) -> void {
	host->env.set(env_idx.value, value);
}

inline
auto default_snap_amount(Host* host, blink_EnvIdx env_idx, DefaultSnapAmount value) -> void {
	host->env.set(env_idx.value, value);
}

inline
auto default_value(Host* host, blink_EnvIdx env_idx, DefaultValue<float> value) -> void {
	host->env.set(env_idx.value, value);
}

inline
auto default_value(Host* host, ParamOptionIdx option_idx, DefaultValue<int64_t> value) -> void {
	host->param_option.set(option_idx.value, value);
}

inline
auto default_value(Host* host, blink_SliderIntIdx sld_idx, DefaultValue<int64_t> value) -> void {
	host->slider_int.set(sld_idx.value, value);
}

inline
auto default_value(Host* host, blink_SliderRealIdx sld_idx, DefaultValue<float> value) -> void {
	host->slider_real.set(sld_idx.value, value);
}

inline
auto env(Host* host, ParamEnvIdx param_env_idx, blink_EnvIdx value) -> void {
	host->param_env.set(param_env_idx.value, EnvIdx{value});
}

inline
auto fns(Host* host, blink_EnvIdx env_idx, EnvFns fns) -> void {
	host->env.set(env_idx.value, fns);
}

inline
auto group(Host* host, ParamGlobalIdx param_idx, blink_StaticString group) -> void {
	const auto plugin_idx = host->param.get<blink_PluginIdx>(param_idx.value);
	auto& group_info_vec = host->plugin.get<std::vector<GroupInfo>>(plugin_idx.value);
	auto find_group = [&group](const GroupInfo& info) {
		return std::string_view(info.name.value) == std::string_view(group.value);
	};
	if (const auto pos = std::find_if(group_info_vec.begin(), group_info_vec.end(), find_group); pos == group_info_vec.end()) {
		GroupInfo info;
		info.name = group;
		info.params.push_back({param_idx.value});
		group_info_vec.push_back(info);
	} else {
		pos->params.push_back({param_idx.value});
	}
	host->param.get<ParamStrings>(param_idx.value).group = group;
}

inline
auto icon(Host* host, ParamGlobalIdx param_idx, blink_StdIcon icon) -> void {
	host->param.set(param_idx.value, ParamIcon{icon});
}

inline
auto info(Host* host, blink_PluginIdx plugin_idx, blink_PluginInfo info) -> void {
	host->plugin.set(plugin_idx.value, info);
}

inline
auto long_desc(Host* host, ParamGlobalIdx param_idx, blink_StaticString desc) -> void {
	host->param.get<ParamStrings>(param_idx.value).long_desc = desc;
}

inline
auto manip_delegate(Host* host, ParamGlobalIdx param_idx, ParamGlobalIdx delegate) -> void {
	host->param.set(param_idx.value, ManipDelegate{delegate});
}

inline
auto max_slider(Host* host, blink_EnvIdx env_idx, MaxSliderIdx value) -> void {
	host->env.set(env_idx.value, value);
}

inline
auto min_slider(Host* host, blink_EnvIdx env_idx, MinSliderIdx value) -> void {
	host->env.set(env_idx.value, value);
}

inline
auto offset_env(Host* host, ParamEnvIdx env_idx, OffsetEnvIdx value) -> void {
	host->param_env.set(env_idx.value, value);
}

inline
auto offset_env(Host* host, ParamSliderRealIdx sld_idx, OffsetEnvIdx value) -> void {
	host->param_slider_real.set(sld_idx.value, value);
}

inline
auto override_env(Host* host, ParamEnvIdx env_idx, OverrideEnvIdx value) -> void {
	host->param_env.set(env_idx.value, value);
}

inline
auto override_env(Host* host, ParamSliderRealIdx sld_idx, OverrideEnvIdx value) -> void {
	host->param_slider_real.set(sld_idx.value, value);
}

inline
auto plugin_interface(Host* host, blink_PluginIdx plugin_idx, PluginInterface iface) -> void {
	host->plugin.set(plugin_idx.value, iface);
}

inline
auto name(Host* host, ParamGlobalIdx param_idx, blink_StaticString name) -> void {
	host->param.get<ParamStrings>(param_idx.value).name = name;
}

inline
auto sampler_info(Host* host, blink_PluginIdx plugin_idx, blink_SamplerInfo info) -> void {
	const auto sub_idx = host->plugin.get<PluginTypeIdx>(plugin_idx.value).value;
	host->plugin_sampler.set(sub_idx, SamplerInfo{info});
}

inline
auto short_name(Host* host, ParamGlobalIdx param_idx, blink_StaticString name) -> void {
	host->param.get<ParamStrings>(param_idx.value).short_name = name;
}

inline
auto slider(Host* host, ParamSliderIntIdx sld_idx, blink_SliderIntIdx value) -> void {
	host->param_slider_int.set(sld_idx.value, value);
}

inline
auto slider(Host* host, ParamSliderRealIdx sld_idx, blink_SliderRealIdx value) -> void {
	host->param_slider_real.set(sld_idx.value, value);
}

inline
auto step_size_slider(Host* host, blink_EnvIdx env_idx, StepSizeSliderIdx value) -> void {
	host->env.set(env_idx.value, value);
}

inline
auto strings(Host* host, ParamOptionIdx option_idx, StringVec strings) -> void {
	host->param_option.get<StringVec>(option_idx.value) = std::move(strings);
}

inline
auto tweaker(Host* host, blink_SliderIntIdx sld_idx, TweakerInt value) -> void {
	host->slider_int.set(sld_idx.value, value);
}

inline
auto tweaker(Host* host, blink_SliderRealIdx sld_idx, TweakerReal value) -> void {
	host->slider_real.set(sld_idx.value, value);
}

inline
auto type_idx(Host* host, ParamGlobalIdx param_idx, ParamEnvIdx type_idx) -> void {
	host->param.set(param_idx.value, ParamTypeIdx{type_idx.value});
}

inline
auto type_idx(Host* host, ParamGlobalIdx param_idx, ParamOptionIdx type_idx) -> void {
	host->param.set(param_idx.value, ParamTypeIdx{type_idx.value});
}

inline
auto type_idx(Host* host, ParamGlobalIdx param_idx, ParamSliderIntIdx type_idx) -> void {
	host->param.set(param_idx.value, ParamTypeIdx{type_idx.value});
}

inline
auto type_idx(Host* host, ParamGlobalIdx param_idx, ParamSliderRealIdx type_idx) -> void {
	host->param.set(param_idx.value, ParamTypeIdx{type_idx.value});
}

inline
auto uuid(Host* host, ParamGlobalIdx param_idx, blink_UUID uuid) -> void {
	host->param.set(param_idx.value, uuid);
}

inline
auto value_slider(Host* host, blink_EnvIdx env_idx, ValueSliderIdx value) -> void {
	host->env.set(env_idx.value, value);
}

} // write

namespace add {

[[nodiscard]] inline
auto plugin(Host* host, PluginType type) -> blink_PluginIdx {
	const auto plugin_idx = host->plugin.push_back();
	host->plugin.get<PluginType>(plugin_idx) = type;
	if (type == PluginType::sampler) {
		const auto sampler_idx = host->plugin_sampler.push_back();
		host->plugin.get<PluginTypeIdx>(plugin_idx) = {sampler_idx};
		if (!host->default_sampler) {
			host->default_sampler = blink_PluginIdx{plugin_idx};
		}
	}
	return {plugin_idx};
}

namespace slider {

[[nodiscard]] inline
auto empty_int(Host* host) -> blink_SliderIntIdx {
	return {host->slider_int.push_back()};
}

[[nodiscard]] inline
auto empty_real(Host* host) -> blink_SliderRealIdx {
	return {host->slider_real.push_back()};
}

[[nodiscard]] inline
auto amp(Host* host) -> blink_SliderRealIdx {
	const auto idx = add::slider::empty_real(host);
	write::tweaker(host, idx, {tweak::amp::tweaker()});
	write::default_value(host, idx, {1.0f});
	return idx;
}

[[nodiscard]] inline
auto delay_time(Host* host) -> blink_SliderRealIdx {
	const auto idx = add::slider::empty_real(host);
	write::default_value(host, idx, {math::convert::delay_time_ms_to_linear(100.0f)});
	write::tweaker(host, idx, {tweak::delay_time::tweaker()});
	return idx;
}

[[nodiscard]] inline
auto filter_frequency(Host* host) -> blink_SliderRealIdx {
	const auto idx = add::slider::empty_real(host);
	write::default_value(host, idx, {tweak::filter_frequency::DEFAULT_VALUE});
	write::tweaker(host, idx, {tweak::filter_frequency::tweaker()});
	return idx;
}

[[nodiscard]] inline
auto pan(Host* host) -> blink_SliderRealIdx {
	const auto idx = add::slider::empty_real(host);
	write::default_value(host, idx, {0.0f});
	write::tweaker(host, idx, {tweak::pan::tweaker()});
	return idx;
}

[[nodiscard]] inline
auto pitch(Host* host) -> blink_SliderRealIdx {
	const auto idx = add::slider::empty_real(host);
	write::default_value(host, idx, {0.0f});
	write::tweaker(host, idx, {tweak::pitch::tweaker()});
	return idx;
}

[[nodiscard]] inline
auto sample_offset(Host* host) -> blink_SliderIntIdx {
	const auto idx = add::slider::empty_int(host);
	write::default_value(host, idx, {0});
	write::tweaker(host, idx, {tweak::sample_offset::tweaker()});
	return idx;
}

[[nodiscard]] inline
auto speed(Host* host) -> blink_SliderRealIdx {
	const auto idx = add::slider::empty_real(host);
	write::default_value(host, idx, {tweak::speed::NORMAL});
	write::tweaker(host, idx, {tweak::speed::tweaker()});
	return idx;
}

} // slider

namespace env {

[[nodiscard]] inline
auto empty(Host* host) -> blink_EnvIdx {
	return {host->env.push_back()};
}

[[nodiscard]] inline
auto amp(Host* host) -> blink_EnvIdx {
	const auto idx = add::env::empty(host);
	EnvFns fns;
	fns.value.get_grid_line = [](int index, float* out) -> blink_Bool {
		*out = math::convert::linear_to_speed(float(index));
		return {true};
	};
	fns.value.stepify   = tweak::amp::stepify;
	fns.value.to_string = tweak::amp::to_string;
	write::default_max(host, idx, {1.0f});
	write::default_min(host, idx, {0.0f});
	write::default_value(host, idx, {1.0f});
	write::fns(host, idx, fns);
	write::max_slider(host, idx, {add::slider::amp(host)});
	write::value_slider(host, idx, {add::slider::amp(host)});
	return idx;
}

[[nodiscard]] inline
auto delay_time(Host* host) -> blink_EnvIdx {
	const auto idx = add::env::empty(host);
	EnvFns fns;
	fns.value.snap_value = tweak::snap_value;
	fns.value.stepify    = tweak::delay_time::stepify;
	fns.value.to_string  = tweak::delay_time::to_string;
	write::default_max(host, idx, {1.0f});
	write::default_min(host, idx, {0.0f});
	write::default_value(host, idx, {math::convert::delay_time_ms_to_linear(100.0f)});
	write::fns(host, idx, fns);
	write::value_slider(host, idx, {add::slider::delay_time(host)});
	return idx;
}

[[nodiscard]] inline
auto feedback(Host* host) -> blink_EnvIdx {
	const auto idx = add::env::percentage(host->fns);
	write::default_value(host, idx, {0.5f});
	return idx;
}

[[nodiscard]] inline
auto filter_frequency(Host* host) -> blink_EnvIdx {
	const auto idx = add::env::empty(host);
	EnvFns fns;
	fns.value.to_string = tweak::filter_frequency::to_string;
	write::default_max(host, idx, {1.0f});
	write::default_min(host, idx, {0.0f});
	write::default_value(host, idx, {tweak::filter_frequency::DEFAULT_VALUE});
	write::value_slider(host, idx, {add::slider::filter_frequency(host)});
	write::fns(host, idx, fns);
	return idx;
}

[[nodiscard]] inline
auto formant(Host* host) -> blink_EnvIdx {
	const auto idx = add::env::empty(host);
	EnvFns fns;
	fns.value.stepify   = tweak::percentage::stepify;
	fns.value.to_string = tweak::percentage::to_string;
	write::default_max(host, idx, {1.0f});
	write::default_min(host, idx, {-1.0f});
	write::default_value(host, idx, {0.0f});
	write::fns(host, idx, fns);
	write::value_slider(host, idx, {add::slider::percentage_bipolar(host->fns)});
	return idx;
}

[[nodiscard]] inline
auto pan(Host* host) -> blink_EnvIdx {
	const auto idx = add::env::empty(host);
	EnvFns fns;
	fns.value.stepify   = tweak::pan::stepify;
	fns.value.to_string = tweak::pan::to_string;
	write::default_max(host, idx, {1.0f});
	write::default_min(host, idx, {-1.0f});
	write::default_value(host, idx, {0.0f});
	write::fns(host, idx, fns);
	write::value_slider(host, idx, {add::slider::pan(host)});
	return idx;
}

[[nodiscard]] inline
auto pitch(Host* host) -> blink_EnvIdx {
	const auto idx = add::env::empty(host);
	const auto slider_value     = add::slider::pitch(host);
	const auto slider_max       = add::slider::pitch(host);
	const auto slider_min       = add::slider::pitch(host);
	const auto slider_step_size = add::slider::empty_real(host);
	EnvFns fns;
	fns.value.stepify    = tweak::pitch::stepify;
	fns.value.snap_value = tweak::snap_value;
	fns.value.to_string  = tweak::to_string;
	fns.value.get_grid_line = [](int index, float* out) -> blink_Bool {
		*out = float(index * 12);
		return {true};
	};
	fns.value.get_step_line = [](int index, float step_size) -> float {
		return step_size * index;
	};
	write::default_max(host, idx, {24.0f});
	write::default_min(host, idx, {-24.0f});
	write::default_snap_amount(host, idx, {1.0f});
	write::default_value(host, idx, {0.0f});
	write::fns(host, idx, fns);
	write::max_slider(host, idx, {slider_max});
	write::min_slider(host, idx, {slider_min});
	write::value_slider(host, idx, {slider_value});
	write::default_value(host, slider_max, {24.0f});
	write::default_value(host, slider_min, {-24.0f});
	write::default_value(host, slider_step_size, {1.0f});
	write::tweaker(host, slider_step_size, {tweak::pitch::step_size_tweaker()});
	return idx;
}

[[nodiscard]] inline
auto speed(Host* host) -> blink_EnvIdx {
	const auto idx          = add::env::empty(host);
	const auto slider_max   = add::slider::speed(host);
	const auto slider_min   = add::slider::speed(host);
	const auto slider_value = add::slider::speed(host);
	EnvFns fns;
	fns.value.get_grid_line = [](int index, float* out) -> blink_Bool {
		*out = math::convert::linear_to_speed(float(index));
		return {true};
	};
	fns.value.to_string = tweak::speed::to_string;
	write::default_max(host, idx, {tweak::speed::DOUBLE});
	write::default_min(host, idx, {tweak::speed::FREEZE});
	write::default_value(host, idx, {tweak::speed::NORMAL});
	write::fns(host, idx, fns);
	write::max_slider(host, idx, {slider_max});
	write::min_slider(host, idx, {slider_min});
	write::value_slider(host, idx, {slider_value});
	write::default_value(host, slider_max, {2.0f});
	write::default_value(host, slider_min, {tweak::speed::FREEZE});
	return idx;
}

} // env

namespace param {

struct NewParam {
	blink_ParamIdx local_idx;
	ParamGlobalIdx global_idx;
};

[[nodiscard]] inline
auto default_active(ParamType type) -> bool {
	return type == ParamType::option || type == ParamType::slider_int || type == ParamType::slider_real;
}

[[nodiscard]] inline
auto empty(Host* host, blink_PluginIdx plugin_idx, ParamType type) -> NewParam {
	auto& param_list = host->plugin.get<PluginParams>(plugin_idx.value).global_indices;
	const auto local_idx  = param_list.size();
	const auto global_idx = host->param.push_back();
	host->param.set(global_idx, type);
	host->param.set(global_idx, plugin_idx);
	param_list.push_back({global_idx});
	if (default_active(type)) {
		write::add_flags(host, blink::ParamGlobalIdx{global_idx}, blink_ParamFlags_DefaultActive);
	}
	return {local_idx, {global_idx}};
}

namespace chord {

[[nodiscard]] inline
auto scale(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param = add::param::empty(host, plugin_idx, ParamType::chord);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_SCALE});
	write::name(host, param.global_idx, {"Scale"});
	write::icon(host, param.global_idx, blink_StdIcon_PianoRoll);
	return param.local_idx;
}

[[nodiscard]] inline
auto std(Host* host, blink_PluginIdx plugin_idx, StdChord std_chord) -> blink_ParamIdx {
	switch (std_chord) {
		case StdChord::SCALE: { return scale(host, plugin_idx); }
	}
	assert (false);
	return {0};
}

[[nodiscard]] inline
auto custom(Host* host, blink_PluginIdx plugin_idx, blink_UUID uuid) -> blink_ParamIdx {
	const auto param = add::param::empty(host, plugin_idx, ParamType::chord);
	write::uuid(host, param.global_idx, uuid);
	return param.local_idx;
}

} // chord

namespace env {

[[nodiscard]] inline
auto empty(Host* host) -> ParamEnvIdx {
	return {host->param_env.push_back()};
}

[[nodiscard]] inline
auto mix(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::env);
	const auto param_env_idx = add::param::env::empty(host);
	const auto env_idx       = add::env::percentage(host->fns);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_MIX});
	write::type_idx(host, param.global_idx, param_env_idx);
	write::name(host, param.global_idx, {"Mix"});
	write::env(host, param_env_idx, env_idx);
	write::default_value(host, env_idx, {1.0f});
	write::clamp_range(host, param_env_idx, {blink_Range{0.0f, 1.0f}});
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_HostClamp);
	write::offset_env(host, param_env_idx, {add::env::percentage_bipolar(host->fns)});
	write::override_env(host, param_env_idx, {env_idx});
	return param.local_idx;
}

[[nodiscard]] inline
auto amp(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::env);
	const auto param_env_idx = add::param::env::empty(host);
	const auto flags = blink_ParamFlags_CanManipulate | blink_ParamFlags_MovesDisplay;
	const auto apply_offset_fn = [](float value, float offset) -> float { return value * offset; };
	write::type_idx(host, param.global_idx, param_env_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_AMP});
	write::name(host, param.global_idx, {"Amp"});
	write::add_flags(host, param.global_idx, flags);
	write::apply_offset_fn(host, param_env_idx, apply_offset_fn);
	write::env(host, param_env_idx, add::env::amp(host));
	write::offset_env(host, param_env_idx, {add::env::amp(host)});
	write::override_env(host, param_env_idx, {add::env::amp(host)});
	return param.local_idx;
}

[[nodiscard]] inline
auto delay_time(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::env);
	const auto param_env_idx = add::param::env::empty(host);
	const auto env_idx       = add::env::delay_time(host);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_DELAY_TIME});
	write::type_idx(host, param.global_idx, param_env_idx);
	write::name(host, param.global_idx, {"Time"});
	write::long_desc(host, param.global_idx, {"Delay Time"});
	write::env(host, param_env_idx, env_idx);
	write::clamp_range(host, param_env_idx, {blink_Range{0.0f, 1.0f}});
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_HostClamp);
	write::offset_env(host, param_env_idx, {add::env::percentage_bipolar(host->fns)});
	write::override_env(host, param_env_idx, {env_idx});
	return param.local_idx;
}

[[nodiscard]] inline
auto dry(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param = add::param::env::mix(host, plugin_idx);
	const auto global_idx = read::local_to_global(*host, plugin_idx, param);
	write::uuid(host, global_idx, {BLINK_STD_UUID_DRY});
	write::name(host, global_idx, {"Dry"});
	return param;
}

[[nodiscard]] inline
auto feedback(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::env);
	const auto param_env_idx = add::param::env::empty(host);
	const auto env_idx       = add::env::empty(host);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_FEEDBACK});
	write::name(host, param.global_idx, {"Feedback"});
	write::type_idx(host, param.global_idx, param_env_idx);
	write::env(host, param_env_idx, env_idx);
	write::clamp_range(host, param_env_idx, {blink_Range{0.0f, 1.0f}});
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_HostClamp);
	write::offset_env(host, param_env_idx, {add::env::percentage_bipolar(host->fns)});
	write::override_env(host, param_env_idx, {env_idx});
	return param.local_idx;
}

[[nodiscard]] inline
auto filter_frequency(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::env);
	const auto param_env_idx = add::param::env::empty(host);
	const auto env_idx       = add::env::filter_frequency(host);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_FILTER_FREQUENCY});
	write::name(host, param.global_idx, {"Frequency"});
	write::type_idx(host, param.global_idx, param_env_idx);
	write::env(host, param_env_idx, env_idx);
	write::clamp_range(host, param_env_idx, {blink_Range{0.0f, 1.0f}});
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_HostClamp);
	write::offset_env(host, param_env_idx, {add::env::percentage_bipolar(host->fns)});
	write::override_env(host, param_env_idx, {env_idx});
	return param.local_idx;
}

[[nodiscard]] inline
auto filter_resonance(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::env);
	const auto param_env_idx = add::param::env::empty(host);
	const auto env_idx       = add::env::percentage(host->fns);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_FILTER_RESONANCE});
	write::name(host, param.global_idx, {"Resonance"});
	write::type_idx(host, param.global_idx, param_env_idx);
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate);
	write::env(host, param_env_idx, env_idx);
	write::offset_env(host, param_env_idx, {add::env::percentage_bipolar(host->fns)});
	write::override_env(host, param_env_idx, {env_idx});
	return param.local_idx;
}

[[nodiscard]] inline
auto formant(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::env);
	const auto param_env_idx = add::param::env::empty(host);
	const auto env_idx       = add::env::formant(host);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_FORMANT});
	write::name(host, param.global_idx, {"Formant"});
	write::type_idx(host, param.global_idx, param_env_idx);
	write::env(host, param_env_idx, add::env::formant(host));
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate);
	write::offset_env(host, param_env_idx, {env_idx});
	write::override_env(host, param_env_idx, {env_idx});
	return param.local_idx;
}

[[nodiscard]] inline
auto noise_amount(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::env);
	const auto param_env_idx = add::param::env::empty(host);
	const auto env_idx       = add::env::percentage(host->fns);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_NOISE_AMOUNT});
	write::name(host, param.global_idx, {"Noise Amount"});
	write::short_name(host, param.global_idx, {"Amount"});
	write::group(host, param.global_idx, {"Noise"});
	write::type_idx(host, param.global_idx, param_env_idx);
	write::clamp_range(host, param_env_idx, {blink_Range{0.0f, 1.0f}});
	write::env(host, param_env_idx, env_idx);
	write::add_flags(host, param.global_idx, blink_ParamFlags_HostClamp | blink_ParamFlags_CanManipulate);
	write::offset_env(host, param_env_idx, {add::env::percentage_bipolar(host->fns)});
	write::override_env(host, param_env_idx, {env_idx});
	return param.local_idx;
}

[[nodiscard]] inline
auto noise_color(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::env);
	const auto param_env_idx = add::param::env::empty(host);
	const auto env_idx       = add::env::percentage_bipolar(host->fns);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_NOISE_COLOR});
	write::name(host, param.global_idx, {"Noise Color"});
	write::short_name(host, param.global_idx, {"Color"});
	write::group(host, param.global_idx, {"Noise"});
	write::type_idx(host, param.global_idx, param_env_idx);
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_HostClamp);
	write::env(host, param_env_idx, env_idx);
	write::clamp_range(host, param_env_idx, {blink_Range{-1.0f, 1.0f}});
	write::offset_env(host, param_env_idx, {env_idx});
	write::override_env(host, param_env_idx, {env_idx});
	return param.local_idx;
}

[[nodiscard]] inline
auto noise_width(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::env);
	const auto param_env_idx = add::param::env::empty(host);
	const auto env_idx       = add::env::percentage(host->fns);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_NOISE_WIDTH});
	write::name(host, param.global_idx, {"Noise Width"});
	write::short_name(host, param.global_idx, {"Width"});
	write::group(host, param.global_idx, {"Noise"});
	write::type_idx(host, param.global_idx, param_env_idx);
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_HostClamp);
	write::clamp_range(host, param_env_idx, {blink_Range{0.0f, 1.0f}});
	write::env(host, param_env_idx, env_idx);
	write::offset_env(host, param_env_idx, {add::env::percentage_bipolar(host->fns)});
	write::override_env(host, param_env_idx, {env_idx});
	return param.local_idx;
}

[[nodiscard]] inline
auto pan(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::env);
	const auto param_env_idx = add::param::env::empty(host);
	const auto env_idx       = add::env::pan(host);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_PAN});
	write::name(host, param.global_idx, {"Pan"});
	write::type_idx(host, param.global_idx, param_env_idx);
	write::env(host, param_env_idx, env_idx);
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate);
	write::offset_env(host, param_env_idx, {env_idx});
	write::override_env(host, param_env_idx, {env_idx});
	return param.local_idx;
}

[[nodiscard]] inline
auto pitch(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::env);
	const auto param_env_idx = add::param::env::empty(host);
	const auto env_idx       = add::env::pitch(host);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_PITCH});
	write::name(host, param.global_idx, {"Pitch"});
	write::type_idx(host, param.global_idx, param_env_idx);
	write::env(host, param_env_idx, env_idx);
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_MovesDisplay);
	write::offset_env(host, param_env_idx, {env_idx});
	write::override_env(host, param_env_idx, {env_idx});
	return param.local_idx;
}

[[nodiscard]] inline
auto speed(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::env);
	const auto param_env_idx = add::param::env::empty(host);
	const auto env_idx       = add::env::speed(host);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_SPEED});
	write::name(host, param.global_idx, {"Speed"}); write::env(host, param_env_idx, env_idx);
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_MovesDisplay);
	write::type_idx(host, param.global_idx, param_env_idx);
	write::offset_env(host, param_env_idx, {env_idx});
	write::override_env(host, param_env_idx, {env_idx});
	write::apply_offset_fn(host, param_env_idx, [](float value, float offset) -> float { return value * offset; });
	return param.local_idx;
}

[[nodiscard]] inline
auto wet(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param = add::param::env::mix(host, plugin_idx);
	const auto global_idx = read::local_to_global(*host, plugin_idx, param);
	write::uuid(host, global_idx, {BLINK_STD_UUID_WET});
	write::name(host, global_idx, {"Wet"});
	return param;
}

[[nodiscard]] inline
auto std(Host* host, blink_PluginIdx plugin_idx, StdEnv std_env) -> blink_ParamIdx {
	switch (std_env) {
		case StdEnv::AMP:              { return amp(host, plugin_idx); }
		case StdEnv::DELAY_TIME:       { return delay_time(host, plugin_idx); }
		case StdEnv::DRY:              { return dry(host, plugin_idx); }
		case StdEnv::FEEDBACK:         { return feedback(host, plugin_idx); }
		case StdEnv::FILTER_FREQUENCY: { return filter_frequency(host, plugin_idx); }
		case StdEnv::FILTER_RESONANCE: { return filter_resonance(host, plugin_idx); }
		case StdEnv::FORMANT:          { return formant(host, plugin_idx); }
		case StdEnv::MIX:              { return mix(host, plugin_idx); }
		case StdEnv::NOISE_AMOUNT:     { return noise_amount(host, plugin_idx); }
		case StdEnv::NOISE_COLOR:      { return noise_color(host, plugin_idx); }
		case StdEnv::NOISE_WIDTH:      { return noise_width(host, plugin_idx); }
		case StdEnv::PAN:              { return pan(host, plugin_idx); }
		case StdEnv::PITCH:            { return pitch(host, plugin_idx); }
		case StdEnv::SPEED:            { return speed(host, plugin_idx); }
		case StdEnv::WET:              { return wet(host, plugin_idx); }
	}
	assert (false);
	return {0};
}

[[nodiscard]] inline
auto custom(Host* host, blink_PluginIdx plugin_idx, blink_UUID uuid) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::env);
	const auto param_env_idx = add::param::env::empty(host);
	const auto env_idx       = add::env::empty(host->fns);
	write::type_idx(host, param.global_idx, param_env_idx);
	write::env(host, param_env_idx, env_idx);
	write::uuid(host, param.global_idx, uuid);
	return param.local_idx;
}

} // env

namespace option {

[[nodiscard]] inline
auto empty(Host* host) -> ParamOptionIdx {
	return {host->param_option.push_back()};
}

[[nodiscard]] inline
auto loop(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx, ParamType::option);
	const auto option_idx = add::param::option::empty(host);
	const auto flags = 
		blink_ParamFlags_IsToggle |
		blink_ParamFlags_ShowInContextMenu |
		blink_ParamFlags_ShowButton |
		blink_ParamFlags_MovesDisplay |
		blink_ParamFlags_IconOnly;
	write::type_idx(host, param.global_idx, option_idx);
	write::icon(host, param.global_idx, blink_StdIcon_Loop);
	write::name(host, param.global_idx, {"Loop"});
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_LOOP});
	write::add_flags(host, param.global_idx, flags);
	write::default_value(host, option_idx, {0});
	return param.local_idx;
}

[[nodiscard]] inline
auto noise_mode(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx, ParamType::option);
	const auto option_idx = add::param::option::empty(host);
	write::type_idx(host, param.global_idx, option_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_NOISE_MODE});
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate);
	write::group(host, param.global_idx, {"Noise"});
	write::name(host, param.global_idx, {"Noise Mode"});
	write::short_name(host, param.global_idx, {"Mode"});
	write::strings(host, option_idx, {{"Multiply", "Mix"}});
	return param.local_idx;
}

[[nodiscard]] inline
auto reverse_mode(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx, ParamType::option);
	const auto option_idx = add::param::option::empty(host);
	write::type_idx(host, param.global_idx, option_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_REVERSE_MODE});
	write::name(host, param.global_idx, {"Reverse"});
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_Hidden | blink_ParamFlags_MovesDisplay);
	write::default_value(host, option_idx, {-1});
	write::strings(host, option_idx, {{"Mirror", "Tape", "Slip"}});
	return param.local_idx;
}

[[nodiscard]] inline
auto reverse_toggle(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx, ParamType::option);
	const auto option_idx = add::param::option::empty(host);
	const auto flags =
		blink_ParamFlags_CanManipulate |
		blink_ParamFlags_IsToggle |
		blink_ParamFlags_ShowInContextMenu |
		blink_ParamFlags_ShowButton |
		blink_ParamFlags_MovesDisplay |
		blink_ParamFlags_IconOnly;
	write::type_idx(host, param.global_idx, option_idx);
	write::add_flags(host, param.global_idx, flags);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_REVERSE_TOGGLE});
	write::name(host, param.global_idx, {"Reverse"});
	write::icon(host, param.global_idx, blink_StdIcon_Reverse);
	write::default_value(host, option_idx, {0});
	return param.local_idx;
}

[[nodiscard]] inline
auto std(Host* host, blink_PluginIdx plugin_idx, StdOption std_option) -> blink_ParamIdx {
	switch (std_option) {
		case StdOption::LOOP:           { return loop(host, plugin_idx); }
		case StdOption::NOISE_MODE:     { return noise_mode(host, plugin_idx); }
		case StdOption::REVERSE_MODE:   { return reverse_mode(host, plugin_idx); }
		case StdOption::REVERSE_TOGGLE: { return reverse_toggle(host, plugin_idx); }
	}
	assert (false);
	return {0};
}

[[nodiscard]] inline
auto custom(Host* host, blink_PluginIdx plugin_idx, blink_UUID uuid) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx, ParamType::option);
	const auto option_idx = add::param::option::empty(host);
	write::type_idx(host, param.global_idx, option_idx);
	write::uuid(host, param.global_idx, uuid);
	return param.local_idx;
}

} // option

namespace slider_int {

[[nodiscard]] inline
auto empty(Host* host) -> ParamSliderIntIdx {
	return {host->param_slider_int.push_back()};
}

[[nodiscard]] inline
auto sample_offset(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::slider_int);
	const auto sld_param_idx = add::param::slider_int::empty(host);
	const auto sld_idx       = add::slider::sample_offset(host);
	write::type_idx(host, param.global_idx, sld_param_idx);
	write::icon(host, param.global_idx, blink_StdIcon_SampleOffset);
	write::add_flags(host, param.global_idx, blink_ParamFlags_MovesDisplay);
	write::name(host, param.global_idx, {"Sample Offset"});
	write::slider(host, sld_param_idx, sld_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_SAMPLE_OFFSET});
	return param.local_idx;
}

[[nodiscard]] inline
auto std(Host* host, blink_PluginIdx plugin_idx, StdSliderInt std_sld) -> blink_ParamIdx {
	switch (std_sld) {
		case StdSliderInt::SAMPLE_OFFSET: { return sample_offset(host, plugin_idx); }
	}
	assert (false);
	return {0};
}

[[nodiscard]] inline
auto custom(Host* host, blink_PluginIdx plugin_idx, blink_UUID uuid) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::slider_int);
	const auto param_sld_idx = add::param::slider_int::empty(host);
	const auto sld_idx       = add::slider::empty_int(host->fns);
	write::type_idx(host, param.global_idx, param_sld_idx);
	write::slider(host, param_sld_idx, sld_idx);
	write::uuid(host, param.global_idx, uuid);
	return param.local_idx;
}

} // slider_int

namespace slider_real {

[[nodiscard]] inline
auto empty(Host* host) -> ParamSliderRealIdx {
	return {host->param_slider_real.push_back()};
}

inline
auto mix(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::slider_real);
	const auto param_sld_idx = add::param::slider_real::empty(host);
	const auto sld_idx       = add::slider::percentage(host->fns);
	write::type_idx(host, param.global_idx, param_sld_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_MIX});
	write::name(host, param.global_idx, {"Mix"});
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate);
	write::slider(host, param_sld_idx, sld_idx);
	write::offset_env(host, param_sld_idx, {add::env::percentage_bipolar(host->fns)});
	write::override_env(host, param_sld_idx, {add::env::percentage(host->fns)});
	write::default_value(host, sld_idx, {1.0f});
	return param.local_idx;
}

inline
auto amp(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::slider_real);
	const auto sld_param_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_param_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_AMP});
	write::name(host, param.global_idx, {"Amp"});
	write::slider(host, sld_param_idx, add::slider::amp(host));
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_MovesDisplay);
	return param.local_idx;
}

inline
auto delay_time(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param   = add::param::empty(host, plugin_idx, ParamType::slider_real);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_DELAY_TIME});
	write::name(host, param.global_idx, {"Time"});
	write::long_desc(host, param.global_idx, {"Delay Time"});
	write::clamp_range(host, sld_idx, {blink_Range{0.0f, 1.0f}});
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_HostClamp);
	write::slider(host, sld_idx, add::slider::delay_time(host));
	return param.local_idx;
}

inline
auto dry(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param = add::param::slider_real::mix(host, plugin_idx);
	const auto global_idx = read::local_to_global(*host, plugin_idx, param);
	write::uuid(host, global_idx, {BLINK_STD_UUID_DRY});
	write::name(host, global_idx, {"Dry"});
	return param;
}

inline
auto feedback(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param   = add::param::empty(host, plugin_idx, ParamType::slider_real);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_FEEDBACK});
	write::name(host, param.global_idx, {"Feedback"});
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_HostClamp);
	write::clamp_range(host, sld_idx, {blink_Range{0.0f, 1.0f}});
	write::offset_env(host, sld_idx, {add::env::percentage_bipolar(host->fns)});
	write::override_env(host, sld_idx, {add::env::feedback(host)});
	write::slider(host, sld_idx, add::slider::percentage(host->fns));
	return param.local_idx;
}

inline
auto filter_frequency(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param   = add::param::empty(host, plugin_idx, ParamType::slider_real);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_FILTER_FREQUENCY});
	write::name(host, param.global_idx, {"Frequency"});
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_HostClamp);
	write::clamp_range(host, sld_idx, {blink_Range{0.0f, 1.0f}});
	write::offset_env(host, sld_idx, {add::env::percentage_bipolar(host->fns)});
	write::override_env(host, sld_idx, {add::env::filter_frequency(host)});
	write::slider(host, sld_idx, add::slider::filter_frequency(host));
	return param.local_idx;
}

inline
auto filter_resonance(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param   = add::param::empty(host, plugin_idx, ParamType::slider_real);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_FILTER_RESONANCE});
	write::name(host, param.global_idx, {"Resonance"});
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate);
	write::offset_env(host, sld_idx, {add::env::percentage_bipolar(host->fns)});
	write::override_env(host, sld_idx, {add::env::percentage(host->fns)});
	write::slider(host, sld_idx, add::slider::percentage(host->fns));
	return param.local_idx;
}

inline
auto formant(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx, ParamType::slider_real);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_FORMANT});
	write::name(host, param.global_idx, {"Formant"});
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate);
	write::offset_env(host, sld_idx, {add::env::formant(host)});
	write::override_env(host, sld_idx, {add::env::formant(host)});
	write::slider(host, sld_idx, add::slider::percentage_bipolar(host->fns));
	return param.local_idx;
}

inline
auto noise_amount(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param   = add::param::empty(host, plugin_idx, ParamType::slider_real);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_NOISE_AMOUNT});
	write::name(host, param.global_idx, {"Noise Amount"});
	write::short_name(host, param.global_idx, {"Amount"});
	write::group(host, param.global_idx, {"Noise"});
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_HostClamp);
	write::clamp_range(host, sld_idx, {blink_Range{0.0f, 1.0f}});
	write::slider(host, sld_idx, add::slider::percentage(host->fns));
	write::offset_env(host, sld_idx, {add::env::percentage_bipolar(host->fns)});
	write::override_env(host, sld_idx, {add::env::percentage(host->fns)});
	return param.local_idx;
}

inline
auto noise_color(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param   = add::param::empty(host, plugin_idx, ParamType::slider_real);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_NOISE_COLOR});
	write::name(host, param.global_idx, {"Noise Color"});
	write::short_name(host, param.global_idx, {"Color"});
	write::group(host, param.global_idx, {"Noise"});
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_HostClamp);
	write::clamp_range(host, sld_idx, {blink_Range{-1.0f, 1.0f}});
	write::slider(host, sld_idx, add::slider::percentage_bipolar(host->fns));
	write::offset_env(host, sld_idx, {add::env::percentage_bipolar(host->fns)});
	write::override_env(host, sld_idx, {add::env::percentage_bipolar(host->fns)});
	return param.local_idx;
}

inline
auto noise_width(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx, ParamType::slider_real);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_NOISE_WIDTH});
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_HostClamp);
	write::name(host, param.global_idx, {"Noise Width"});
	write::short_name(host, param.global_idx, {"Width"});
	write::group(host, param.global_idx, {"Noise"});
	write::clamp_range(host, sld_idx, {blink_Range{0.0f, 1.0f}});
	write::slider(host, sld_idx, add::slider::percentage(host->fns));
	write::offset_env(host, sld_idx, {add::env::percentage_bipolar(host->fns)});
	write::override_env(host, sld_idx, {add::env::percentage(host->fns)});
	return param.local_idx;
}

inline
auto pan(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param   = add::param::empty(host, plugin_idx, ParamType::slider_real);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_PAN});
	write::name(host, param.global_idx, {"Pan"});
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate);
	write::icon(host, param.global_idx, blink_StdIcon_Pan);
	write::slider(host, sld_idx, add::slider::pan(host));
	return param.local_idx;
}

inline
auto pitch(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param   = add::param::empty(host, plugin_idx, ParamType::slider_real);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_PITCH});
	write::name(host, param.global_idx, {"Pitch"});
	write::slider(host, sld_idx, add::slider::pitch(host));
	write::icon(host, param.global_idx, blink_StdIcon_Pitch);
	write::add_flags(host, param.global_idx, blink_ParamFlags_MovesDisplay);
	return param.local_idx;
}

inline
auto speed(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx, ParamType::slider_real);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_SPEED});
	write::name(host, param.global_idx, {"Speed"});
	write::slider(host, sld_idx, add::slider::speed(host));
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_MovesDisplay);
	return param.local_idx;
}

inline
auto wet(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param = add::param::slider_real::mix(host, plugin_idx);
	const auto global_idx = read::local_to_global(*host, plugin_idx, param);
	write::uuid(host, global_idx, {BLINK_STD_UUID_WET});
	write::name(host, global_idx, {"Wet"});
	return param;
}

[[nodiscard]] inline
auto std(Host* host, blink_PluginIdx plugin_idx, StdSliderReal std_sld) -> blink_ParamIdx {
	switch (std_sld) {
		case StdSliderReal::AMP:              { return amp(host, plugin_idx); }
		case StdSliderReal::DELAY_TIME:       { return delay_time(host, plugin_idx); }
		case StdSliderReal::DRY:              { return dry(host, plugin_idx); }
		case StdSliderReal::FEEDBACK:         { return feedback(host, plugin_idx); }
		case StdSliderReal::FILTER_FREQUENCY: { return filter_frequency(host, plugin_idx); }
		case StdSliderReal::FILTER_RESONANCE: { return filter_resonance(host, plugin_idx); }
		case StdSliderReal::FORMANT:          { return formant(host, plugin_idx); }
		case StdSliderReal::MIX:              { return mix(host, plugin_idx); }
		case StdSliderReal::NOISE_AMOUNT:     { return noise_amount(host, plugin_idx); }
		case StdSliderReal::NOISE_COLOR:      { return noise_color(host, plugin_idx); }
		case StdSliderReal::NOISE_WIDTH:      { return noise_width(host, plugin_idx); }
		case StdSliderReal::PAN:              { return pan(host, plugin_idx); }
		case StdSliderReal::PITCH:            { return pitch(host, plugin_idx); }
		case StdSliderReal::SPEED:            { return speed(host, plugin_idx); }
		case StdSliderReal::WET:              { return wet(host, plugin_idx); }
	}
	assert (false);
	return {0};
}

[[nodiscard]] inline
auto custom(Host* host, blink_PluginIdx plugin_idx, blink_UUID uuid) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx, ParamType::slider_real);
	const auto param_sld_idx = add::param::slider_real::empty(host);
	const auto sld_idx       = add::slider::empty_real(host->fns);
	write::type_idx(host, param.global_idx, param_sld_idx);
	write::slider(host, param_sld_idx, sld_idx);
	write::uuid(host, param.global_idx, uuid);
	return param.local_idx;
}

} // slider_real

} // param
} // add

inline
auto begin_unit_process(Host* host, const PluginInterface& plugin, blink_InstanceIdx instance_idx, blink_BufferID buffer_id) -> void {
	auto& process = read::process(host, instance_idx);
	if (buffer_id.value > process.buffer_id.value.value) {
		if (buffer_id.value > process.buffer_id.value.value + 1 || process.active_buffer_units == 0) {
			// instance is reset at the start of the buffer if we have gone
			// at least one buffer with no units being processed
			plugin.instance_reset(process.local_idx.value);
		}
		process.buffer_id.value     = buffer_id;
		process.active_buffer_units = 0;
	}
	process.active_buffer_units++;
}

template <typename ProcessFn> inline
auto unit_process(Host* host, blink_UnitIdx unit_idx, const blink_UnitBuffer& buffer, const blink_UnitState& unit_state, ProcessFn&& process_fn) -> blink_Error {
	auto& process      = host->unit.get<UnitProcess>(unit_idx.value);
	const auto& plugin = read::iface(*host, process.plugin_idx.value);
	begin_unit_process(host, plugin, process.instance_idx, buffer.buffer_id);
	if (buffer.buffer_id.value > process.buffer_id.value.value + 1) {
		// unit is reset at the start of the buffer if we have gone
		// at least one buffer without processing this unit
		plugin.unit_reset(process.local_idx.value);
	}
	process.buffer_id.value = buffer.buffer_id;
	return process_fn(plugin, process.local_idx);
}

inline
auto effect_process(Host* host, blink_UnitIdx unit_idx, const blink_EffectBuffer& buffer, const blink_EffectUnitState& unit_state, const float* in, float* out) -> blink_Error {
	auto process_fn = [&buffer, &unit_state, in, out](const PluginInterface& plugin_iface, blink::LocalUnitIdx local_idx) -> blink_Error {
		return plugin_iface.effect.process(local_idx.value, &buffer, &unit_state, in, out);
	};
	return unit_process(host, unit_idx, buffer.unit, unit_state.unit, std::move(process_fn));
}

inline
auto sampler_draw(const Host& host, blink_PluginIdx plugin_idx, const blink_SamplerBuffer& buffer, const blink_SamplerUnitState& unit_state, blink_FrameCount n, blink_SamplerDrawInfo* out) -> blink_Error {
	const auto& plugin = read::iface(host, plugin_idx);
	return plugin.sampler.draw(&buffer, &unit_state, n, out);
}

inline
auto sampler_process(Host* host, blink_UnitIdx unit_idx, const blink_SamplerBuffer& buffer, const blink_SamplerUnitState& unit_state, float* out) -> blink_Error {
	auto process_fn = [&buffer, &unit_state, out](const PluginInterface& plugin_iface, blink::LocalUnitIdx local_idx) -> blink_Error {
		return plugin_iface.sampler.process(local_idx.value, &buffer, &unit_state, out);
	};
	return unit_process(host, unit_idx, buffer.unit, unit_state.unit, std::move(process_fn));
}

inline
auto synth_process(Host* host, blink_UnitIdx unit_idx, const blink_SynthBuffer& buffer, const blink_SynthUnitState& unit_state, float* out) -> blink_Error {
	auto process_fn = [&buffer, &unit_state, out](const PluginInterface& plugin_iface, blink::LocalUnitIdx local_idx) -> blink_Error {
		return plugin_iface.synth.process(local_idx.value, &buffer, &unit_state, out);
	};
	return unit_process(host, unit_idx, buffer.unit, unit_state.unit, std::move(process_fn));
}

inline
auto terminate(const Host& host, blink_PluginIdx plugin_idx) -> blink_Error {
	const auto& plugin = read::iface(host, plugin_idx);
	return plugin.terminate();
}

[[nodiscard]] static
auto get_plugins_of_type(const Host& host, PluginType type) -> std::vector<blink_PluginIdx> {
	std::vector<blink_PluginIdx> out;
	size_t index = 0;
	for (const auto type_ : host.plugin.get<PluginType>()) {
		if (type_ == type) {
			out.push_back({index});
		}
		index++;
	}
	return out;
}

[[nodiscard]] inline
auto get_effect_plugins(const Host& host) -> std::vector<blink_PluginIdx> {
	return get_plugins_of_type(host, PluginType::effect);
}

[[nodiscard]] inline
auto get_sampler_plugins(const Host& host) -> std::vector<blink_PluginIdx> {
	return get_plugins_of_type(host, PluginType::sampler);
}

[[nodiscard]] inline
auto get_synth_plugins(const Host& host) -> std::vector<blink_PluginIdx> {
	return get_plugins_of_type(host, PluginType::synth);
}

[[nodiscard]] inline
auto get_std_chord(blink_UUID uuid) -> std::optional<StdChord> {
	const auto uuid_str = std::string_view{uuid.value};
	if (uuid_str.compare(BLINK_STD_UUID_SCALE) == 0) { return StdChord::SCALE; }
	return std::nullopt;
}

[[nodiscard]] inline
auto get_std_env(blink_UUID uuid) -> std::optional<StdEnv> {
	const auto uuid_str = std::string_view{uuid.value};
	if (uuid_str.compare(BLINK_STD_UUID_AMP) == 0) { return StdEnv::AMP; }
	if (uuid_str.compare(BLINK_STD_UUID_DELAY_TIME) == 0) { return StdEnv::DELAY_TIME; }
	if (uuid_str.compare(BLINK_STD_UUID_DRY) == 0) { return StdEnv::DRY; }
	if (uuid_str.compare(BLINK_STD_UUID_FEEDBACK) == 0) { return StdEnv::FEEDBACK; }
	if (uuid_str.compare(BLINK_STD_UUID_FILTER_FREQUENCY) == 0) { return StdEnv::FILTER_FREQUENCY; }
	if (uuid_str.compare(BLINK_STD_UUID_FILTER_RESONANCE) == 0) { return StdEnv::FILTER_RESONANCE; }
	if (uuid_str.compare(BLINK_STD_UUID_FORMANT) == 0) { return StdEnv::FORMANT; }
	if (uuid_str.compare(BLINK_STD_UUID_MIX) == 0) { return StdEnv::MIX; }
	if (uuid_str.compare(BLINK_STD_UUID_NOISE_AMOUNT) == 0) { return StdEnv::NOISE_AMOUNT; }
	if (uuid_str.compare(BLINK_STD_UUID_NOISE_COLOR) == 0) { return StdEnv::NOISE_COLOR; }
	if (uuid_str.compare(BLINK_STD_UUID_NOISE_WIDTH) == 0) { return StdEnv::NOISE_WIDTH; }
	if (uuid_str.compare(BLINK_STD_UUID_PAN) == 0) { return StdEnv::PAN; }
	if (uuid_str.compare(BLINK_STD_UUID_PITCH) == 0) { return StdEnv::PITCH; }
	if (uuid_str.compare(BLINK_STD_UUID_SPEED) == 0) { return StdEnv::SPEED; }
	if (uuid_str.compare(BLINK_STD_UUID_WET) == 0) { return StdEnv::WET; }
	return std::nullopt;
}

[[nodiscard]] inline
auto get_std_option(blink_UUID uuid) -> std::optional<StdOption> {
	const auto uuid_str = std::string_view{uuid.value};
	if (uuid_str.compare(BLINK_STD_UUID_LOOP) == 0) { return StdOption::LOOP; }
	if (uuid_str.compare(BLINK_STD_UUID_NOISE_MODE) == 0) { return StdOption::NOISE_MODE; }
	if (uuid_str.compare(BLINK_STD_UUID_REVERSE_MODE) == 0) { return StdOption::REVERSE_MODE; }
	if (uuid_str.compare(BLINK_STD_UUID_REVERSE_TOGGLE) == 0) { return StdOption::REVERSE_TOGGLE; }
	return std::nullopt;
}

[[nodiscard]] inline
auto get_std_slider_int(blink_UUID uuid) -> std::optional<StdSliderInt> {
	const auto uuid_str = std::string_view{uuid.value};
	if (uuid_str.compare(BLINK_STD_UUID_SAMPLE_OFFSET) == 0) { return StdSliderInt::SAMPLE_OFFSET; }
	return std::nullopt;
}

[[nodiscard]] inline
auto get_std_slider_real(blink_UUID uuid) -> std::optional<StdSliderReal> {
	const auto uuid_str = std::string_view{uuid.value};
	if (uuid_str.compare(BLINK_STD_UUID_AMP) == 0) { return StdSliderReal::AMP; }
	if (uuid_str.compare(BLINK_STD_UUID_DELAY_TIME) == 0) { return StdSliderReal::DELAY_TIME; }
	if (uuid_str.compare(BLINK_STD_UUID_DRY) == 0) { return StdSliderReal::DRY; }
	if (uuid_str.compare(BLINK_STD_UUID_FEEDBACK) == 0) { return StdSliderReal::FEEDBACK; }
	if (uuid_str.compare(BLINK_STD_UUID_FILTER_FREQUENCY) == 0) { return StdSliderReal::FILTER_FREQUENCY; }
	if (uuid_str.compare(BLINK_STD_UUID_FILTER_RESONANCE) == 0) { return StdSliderReal::FILTER_RESONANCE; }
	if (uuid_str.compare(BLINK_STD_UUID_FORMANT) == 0) { return StdSliderReal::FORMANT; }
	if (uuid_str.compare(BLINK_STD_UUID_MIX) == 0) { return StdSliderReal::MIX; }
	if (uuid_str.compare(BLINK_STD_UUID_NOISE_AMOUNT) == 0) { return StdSliderReal::NOISE_AMOUNT; }
	if (uuid_str.compare(BLINK_STD_UUID_NOISE_COLOR) == 0) { return StdSliderReal::NOISE_COLOR; }
	if (uuid_str.compare(BLINK_STD_UUID_NOISE_WIDTH) == 0) { return StdSliderReal::NOISE_WIDTH; }
	if (uuid_str.compare(BLINK_STD_UUID_PAN) == 0) { return StdSliderReal::PAN; }
	if (uuid_str.compare(BLINK_STD_UUID_PITCH) == 0) { return StdSliderReal::PITCH; }
	if (uuid_str.compare(BLINK_STD_UUID_SPEED) == 0) { return StdSliderReal::SPEED; }
	if (uuid_str.compare(BLINK_STD_UUID_WET) == 0) { return StdSliderReal::WET; }
	return std::nullopt;
}

[[nodiscard]] inline
auto add_unit(Host* host, blink_PluginIdx plugin_idx, blink_InstanceIdx instance_idx, blink_SR SR) -> blink_UnitIdx {
	const auto& plugin_iface  = read::iface(*host, plugin_idx);
	const auto& instance_proc = host->instance.get<InstanceProcess>(instance_idx.value);
	const auto local_inst_idx = instance_proc.local_idx;
	const auto idx            = host->unit.add();
	auto& instance_units      = host->instance.get<UnitVec>(instance_idx.value).value;
	auto& unit_proc           = host->unit.get<UnitProcess>(idx);
	unit_proc.instance_idx    = instance_idx;
	unit_proc.local_idx.value = plugin_iface.unit_add(local_inst_idx.value);
	unit_proc.plugin_idx      = {plugin_idx};
	instance_units.push_back({idx});
	plugin_iface.unit_stream_init(unit_proc.local_idx.value, SR);
	return {idx};
}

inline
auto destroy_instance(Host* host, blink_PluginIdx plugin_idx, blink_InstanceIdx instance_idx) -> void {
	const auto& plugin_iface  = read::iface(*host, plugin_idx);
	const auto& instance_proc = host->instance.get<InstanceProcess>(instance_idx.value);
	const auto local_inst_idx = instance_proc.local_idx;
	const auto& units         = host->instance.get<UnitVec>(instance_idx.value).value;
	for (auto unit_idx : units) {
		host->unit.erase(unit_idx.value);
	}
	host->instance.erase(instance_idx.value);
	plugin_iface.instance_destroy(local_inst_idx.value);
}

[[nodiscard]] inline
auto get_effect_info(const Host& host, blink_PluginIdx plugin_idx, blink_InstanceIdx instance_idx) -> blink_EffectInstanceInfo {
	return read::iface(host, plugin_idx).effect.get_info(instance_idx);
};

[[nodiscard]] inline
auto make_instance(Host* host, blink_PluginIdx plugin_idx, blink_SR SR) -> blink_InstanceIdx {
	const auto& plugin_iface = read::iface(*host, plugin_idx);
	const auto idx       = host->instance.add();
	auto& proc           = host->instance.get<InstanceProcess>(idx);
	proc.local_idx.value = plugin_iface.instance_make();
	plugin_iface.instance_stream_init(proc.local_idx.value, SR);
	return {idx};
}

inline
auto sampler_preprocess(const Host& host, blink_PluginIdx plugin_idx, void* usr, blink_PreprocessCallbacks callbacks, const blink_SampleInfo& info) -> void {
	const auto& plugin_iface = read::iface(host, plugin_idx);
	plugin_iface.sampler.preprocess_sample(usr, callbacks, &info);
}

inline
auto sampler_sample_deleted(const Host& host, blink_PluginIdx plugin_idx, blink_ID sample_id) -> void {
	const auto& plugin_iface = read::iface(host, plugin_idx);
	plugin_iface.sampler.sample_deleted(sample_id);
}

inline
auto stream_init(const Host& host, blink_SR SR) -> void {
	struct InstanceInit {
		blink_InstanceIdx idx;
		PluginInterface::instance_stream_init_fn fn;
	};
	struct UnitInit {
		blink_UnitIdx idx;
		PluginInterface::unit_stream_init_fn fn;
	};
	::std::vector<InstanceInit> instances;
	::std::vector<UnitInit> units;
	for (size_t idx = 0; idx < host.instance.size(); idx++) {
		const auto plugin = host.instance.get<blink_PluginIdx>(idx);
		const auto& iface = host.plugin.get<PluginInterface>(plugin.value);
		instances.push_back({{idx}, iface.instance_stream_init});
	}
	for (size_t idx = 0; idx < host.unit.size(); idx++) {
		const auto plugin = host.unit.get<blink_PluginIdx>(idx);
		const auto& iface = host.plugin.get<PluginInterface>(plugin.value);
		units.push_back({{idx}, iface.unit_stream_init});
	}
	for (const auto& instance : instances) {
		instance.fn(instance.idx, SR);
	}
	for (const auto& unit : units) {
		unit.fn(unit.idx, SR);
	}
}

[[nodiscard]] inline
auto host_ptr(void* user) -> Host* {
	return (Host*)(user);
}

inline
auto init(Host* host) -> void {
	host->fns.usr = host;
	host->fns.add_env = [](void* usr) {
		return add::env::empty(host_ptr(usr));
	};
	host->fns.add_param_chord = [](void* usr, blink_PluginIdx plugin_idx, blink_UUID uuid) {
		if (const auto std_param = get_std_chord(uuid)) {
			return add::param::chord::std(host_ptr(usr), plugin_idx, *std_param);
		}
		else {
			return add::param::chord::custom(host_ptr(usr), plugin_idx, uuid);
		}
	};
	host->fns.add_param_env = [](void* usr, blink_PluginIdx plugin_idx, blink_UUID uuid) {
		if (const auto std_param = get_std_env(uuid)) {
			return add::param::env::std(host_ptr(usr), plugin_idx, *std_param);
		}
		else {
			return add::param::env::custom(host_ptr(usr), plugin_idx, uuid);
		}
	};
	host->fns.add_param_option = [](void* usr, blink_PluginIdx plugin_idx, blink_UUID uuid) {
		if (const auto std_param = get_std_option(uuid)) {
			return add::param::option::std(host_ptr(usr), plugin_idx, *std_param);
		}
		else {
			return add::param::option::custom(host_ptr(usr), plugin_idx, uuid);
		}
	};
	host->fns.add_param_slider_int = [](void* usr, blink_PluginIdx plugin_idx, blink_UUID uuid) {
		if (const auto std_param = get_std_slider_int(uuid)) {
			return add::param::slider_int::std(host_ptr(usr), plugin_idx, *std_param);
		}
		else {
			return add::param::slider_int::custom(host_ptr(usr), plugin_idx, uuid);
		}
	};
	host->fns.add_param_slider_real = [](void* usr, blink_PluginIdx plugin_idx, blink_UUID uuid) {
		if (const auto std_param = get_std_slider_real(uuid)) {
			return add::param::slider_real::std(host_ptr(usr), plugin_idx, *std_param);
		}
		else {
			return add::param::slider_real::custom(host_ptr(usr), plugin_idx, uuid);
		}
	};
	host->fns.add_slider_int = [](void* usr) {
		return add::slider::empty_int(host_ptr(usr));
	};
	host->fns.add_slider_real = [](void* usr) {
		return add::slider::empty_real(host_ptr(usr));
	};
	host->fns.read_param_env_default_value = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		const auto param_env_idx = ParamEnvIdx{read::type_idx(*host_ptr(usr), param_global_idx)};
		const auto env_idx       = read::env(*host_ptr(usr), param_env_idx);
		return read::default_value(*host_ptr(usr), env_idx);
	};
	host->fns.read_param_env_env = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		const auto param_env_idx = ParamEnvIdx{read::type_idx(*host_ptr(usr), param_global_idx)};
		return read::env(*host_ptr(usr), param_env_idx);
	};
	host->fns.read_param_option_default_value = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		const auto type_idx = read::type_idx(*host_ptr(usr), param_global_idx);
		return read::default_value(*host_ptr(usr), ParamOptionIdx{type_idx});
	};
	host->fns.read_param_slider_int_default_value = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		const auto param_slider_idx = ParamSliderIntIdx{read::type_idx(*host_ptr(usr), param_global_idx)};
		const auto slider_idx       = read::slider(*host_ptr(usr), param_slider_idx);
		return read::default_value(*host_ptr(usr), slider_idx);
	};
	host->fns.read_param_slider_real_default_value = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		const auto param_slider_idx = ParamSliderRealIdx{read::type_idx(*host_ptr(usr), param_global_idx)};
		const auto slider_idx       = read::slider(*host_ptr(usr), param_slider_idx);
		return read::default_value(*host_ptr(usr), slider_idx);
	};
	host->fns.write_env_add_flags = [](void* usr, blink_EnvIdx env_idx, int flags) {
		write::add_flags(host_ptr(usr), env_idx, {flags});
	};
	host->fns.write_param_env_apply_offset_fn = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_ApplyOffsetFn fn) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		const auto param_env_idx    = ParamEnvIdx{read::type_idx(*host_ptr(usr), param_global_idx)};
		write::apply_offset_fn(host_ptr(usr), param_env_idx, {fn});
	};
	host->fns.write_param_env_clamp_range = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_Range range) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		const auto param_env_idx    = ParamEnvIdx{read::type_idx(*host_ptr(usr), param_global_idx)};
		write::clamp_range(host_ptr(usr), param_env_idx, {range});
	};
	host->fns.write_env_default_max = [](void* usr, blink_EnvIdx env_idx, float value) {
		write::default_max(host_ptr(usr), env_idx, {value});
	};
	host->fns.write_env_default_min = [](void* usr, blink_EnvIdx env_idx, float value) {
		write::default_min(host_ptr(usr), env_idx, {value});
	};
	host->fns.write_env_default_snap_amount = [](void* usr, blink_EnvIdx env_idx, float value) {
		write::default_snap_amount(host_ptr(usr), env_idx, {value});
	};
	host->fns.write_env_default_value = [](void* usr, blink_EnvIdx env_idx, float value) {
		write::default_value(host_ptr(usr), env_idx, {value});
	};
	host->fns.write_param_env_env = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_EnvIdx env_idx) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		const auto param_env_idx    = ParamEnvIdx{read::type_idx(*host_ptr(usr), param_global_idx)};
		write::env(host_ptr(usr), param_env_idx, env_idx);
	};
	host->fns.write_env_fns = [](void* usr, blink_EnvIdx env_idx, blink_EnvFns fns) {
		write::fns(host_ptr(usr), env_idx, {fns});
	};
	host->fns.write_env_max_slider = [](void* usr, blink_EnvIdx env_idx, blink_SliderRealIdx slider_idx) {
		write::max_slider(host_ptr(usr), env_idx, {slider_idx});
	};
	host->fns.write_env_min_slider = [](void* usr, blink_EnvIdx env_idx, blink_SliderRealIdx slider_idx) {
		write::min_slider(host_ptr(usr), env_idx, {slider_idx});
	};
	host->fns.write_param_env_offset_env = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_EnvIdx env_idx) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		const auto param_env_idx    = ParamEnvIdx{read::type_idx(*host_ptr(usr), param_global_idx)};
		write::offset_env(host_ptr(usr), param_env_idx, {env_idx});
	};
	host->fns.write_param_env_override_env = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_EnvIdx env_idx) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		const auto param_env_idx    = ParamEnvIdx{read::type_idx(*host_ptr(usr), param_global_idx)};
		write::override_env(host_ptr(usr), param_env_idx, {env_idx});
	};
	host->fns.write_env_step_size_slider = [](void* usr, blink_EnvIdx env_idx, blink_SliderRealIdx slider_idx) {
		write::step_size_slider(host_ptr(usr), env_idx, {slider_idx});
	};
	host->fns.write_env_value_slider = [](void* usr, blink_EnvIdx env_idx, blink_SliderRealIdx slider_idx) {
		write::value_slider(host_ptr(usr), env_idx, {slider_idx});
	};
	host->fns.write_param_add_flags = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, int flags) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		write::add_flags(host_ptr(usr), param_global_idx, {flags});
	};
	host->fns.write_param_add_subparam = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_ParamIdx subparam_idx) {
		const auto param_global_idx    = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		const auto subparam_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, subparam_idx);
		write::add_subparam(host_ptr(usr), param_global_idx, {subparam_global_idx});
	};
	host->fns.write_param_group = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_StaticString group) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		write::group(host_ptr(usr), param_global_idx, group);
	};
	host->fns.write_param_icon = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_StdIcon icon) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		write::icon(host_ptr(usr), param_global_idx, icon);
	};
	host->fns.write_param_long_desc = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_StaticString name) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		write::long_desc(host_ptr(usr), param_global_idx, name);
	};
	host->fns.write_param_manip_delegate = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_ParamIdx delegate_idx) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		const auto deleg_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, delegate_idx);
		write::manip_delegate(host_ptr(usr), param_global_idx, {deleg_global_idx});
	};
	host->fns.write_param_name = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_StaticString name) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		write::name(host_ptr(usr), param_global_idx, name);
	};
	host->fns.write_param_short_name = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_StaticString name) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		write::short_name(host_ptr(usr), param_global_idx, name);
	};
	host->fns.write_param_slider_real_offset_env = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_EnvIdx env_idx) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		const auto param_slider_idx = ParamSliderRealIdx{read::type_idx(*host_ptr(usr), param_global_idx)};
		write::offset_env(host_ptr(usr), param_slider_idx, {env_idx});
	};
	host->fns.write_param_slider_real_override_env = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_EnvIdx env_idx) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		const auto param_slider_idx = ParamSliderRealIdx{read::type_idx(*host_ptr(usr), param_global_idx)};
		write::override_env(host_ptr(usr), param_slider_idx, {env_idx});
	};
	host->fns.write_sampler_info = [](void* usr, blink_PluginIdx plugin_idx, blink_SamplerInfo info) {
		write::sampler_info(host_ptr(usr), plugin_idx, info);
	};
	host->fns.write_slider_int_default_value = [](void* usr, blink_SliderIntIdx slider_idx, int64_t value) {
		write::default_value(host_ptr(usr), slider_idx, {value});
	};
	host->fns.write_slider_int_tweaker = [](void* usr, blink_SliderIntIdx slider_idx, blink_TweakerInt tweaker) {
		write::tweaker(host_ptr(usr), slider_idx, {tweaker});
	};
	host->fns.write_slider_real_default_value = [](void* usr, blink_SliderRealIdx slider_idx, float value) {
		write::default_value(host_ptr(usr), slider_idx, {value});
	};
	host->fns.write_slider_real_tweaker = [](void* usr, blink_SliderRealIdx slider_idx, blink_TweakerReal tweaker) {
		write::tweaker(host_ptr(usr), slider_idx, {tweaker});
	};
}

} // blink