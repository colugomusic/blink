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
		blink_UUID,
		PluginType,
		PluginTypeIdx,
		PluginInterface,
		PluginParams
	>;
	using PluginSampler = ent::static_store<
		SamplerInfo
	>;
	using Instance = ent::dynamic_store<
		InstanceProcess,
		UnitVec
	>;
	using Unit = ent::dynamic_store<
		UnitProcess
	>;
	using Param = ent::static_store<
		blink_PluginIdx,
		blink_UUID,
		ManipDelegate,
		ParamFlags,
		ParamIcon,
		ParamStrings,
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
		DefaultValue<float>,
		EnvFns,
		EnvSnapSettings,
		MaxSliderIdx,
		MinSliderIdx,
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
auto env_idx(const Host& host, ParamEnvIdx param_env_idx) -> blink_EnvIdx {
	return host.param_env.get<EnvIdx>(param_env_idx.value).value;
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
	const auto pred = [uuid_str = std::string_view{*uuid_str}](const blink_UUID& uuid) {
		return uuid.value == uuid_str;
	};
	const auto idx = host.plugin.find<blink_UUID>(pred);
	if (!idx) {
		return std::nullopt;
	}
	return blink_PluginIdx{*idx};
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
auto slider_idx(const Host& host, ParamSliderIntIdx param_env_idx) -> blink_SliderIntIdx {
	return host.param_slider_int.get<blink_SliderIntIdx>(param_env_idx.value);
}

[[nodiscard]] inline
auto slider_idx(const Host& host, ParamSliderRealIdx param_env_idx) -> blink_SliderRealIdx {
	return host.param_slider_real.get<blink_SliderRealIdx>(param_env_idx.value);
}

[[nodiscard]] inline
auto tweaker(const Host& host, blink_SliderIntIdx sld_idx) -> const blink_TweakerInt& {
	return host.slider_int.get<TweakerInt>(sld_idx.value).value;
}

[[nodiscard]] inline
auto type_idx(const Host& host, ParamGlobalIdx param_idx) -> size_t {
	return host.param.get<ParamTypeIdx>(param_idx.value).value;
}

} // read

namespace write {

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
auto clamp_range(Host* host, ParamSliderRealIdx sld_idx, ClampRange value) -> void {
	host->param_slider_real.set(sld_idx.value, value);
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
	host->plugin_sampler.set(plugin_idx.value, SamplerInfo{info});
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
auto snap_settings(Host* host, blink_EnvIdx env_idx, EnvSnapSettings value) -> void {
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
	host->param.set(param_idx.value, type_idx.value);
}

inline
auto type_idx(Host* host, ParamGlobalIdx param_idx, ParamOptionIdx type_idx) -> void {
	host->param.set(param_idx.value, type_idx.value);
}

inline
auto type_idx(Host* host, ParamGlobalIdx param_idx, ParamSliderIntIdx type_idx) -> void {
	host->param.set(param_idx.value, type_idx.value);
}

inline
auto type_idx(Host* host, ParamGlobalIdx param_idx, ParamSliderRealIdx type_idx) -> void {
	host->param.set(param_idx.value, type_idx.value);
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
auto sample_offset(Host* host) -> blink_SliderIntIdx {
	const auto idx = add::slider::empty_int(host);
	write::default_value(host, idx, {0});
	write::tweaker(host, idx, {tweak::sample_offset::tweaker()});
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
	write::default_value(host, idx, {1.0f});
	write::fns(host, idx, fns);
	write::max_slider(host, idx, {add::slider::amp(host)});
	write::value_slider(host, idx, {add::slider::amp(host)});
	return idx;
}

} // env

namespace param {

struct NewParam {
	blink_ParamIdx local_idx;
	ParamGlobalIdx global_idx;
};

[[nodiscard]] inline
auto empty(Host* host, blink_PluginIdx plugin_idx) -> NewParam {
	auto& param_list = host->plugin.get<PluginParams>(plugin_idx.value).global_indices;
	const auto local_idx  = param_list.size();
	const auto global_idx = host->param.push_back();
	host->param.get<blink_PluginIdx>(global_idx) = plugin_idx;
	param_list[local_idx] = {global_idx};
	return {local_idx, {global_idx}};
}

namespace env {

[[nodiscard]] inline
auto empty(Host* host) -> ParamEnvIdx {
	return {host->param_env.push_back()};
}

[[nodiscard]] inline
auto amp(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx);
	const auto param_env_idx = add::param::env::empty(host);
	const auto flags = blink_ParamFlags_CanManipulate | blink_ParamFlags_MovesDisplay;
	const auto apply_offset_fn = [](float value, float offset) -> float { return value * offset; };
	write::type_idx(host, param.global_idx, param_env_idx);
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
	const auto param         = add::param::empty(host, plugin_idx);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param.local_idx;
}

[[nodiscard]] inline
auto dry(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param.local_idx;
}

[[nodiscard]] inline
auto feedback(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param.local_idx;
}

[[nodiscard]] inline
auto filter_frequency(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param.local_idx;
}

[[nodiscard]] inline
auto filter_resonance(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param.local_idx;
}

[[nodiscard]] inline
auto formant(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param.local_idx;
}

[[nodiscard]] inline
auto mix(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param.local_idx;
}

[[nodiscard]] inline
auto noise_amount(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param.local_idx;
}

[[nodiscard]] inline
auto noise_color(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param.local_idx;
}

[[nodiscard]] inline
auto noise_width(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param.local_idx;
}

[[nodiscard]] inline
auto pan(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param.local_idx;
}

[[nodiscard]] inline
auto pitch(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param.local_idx;
}

[[nodiscard]] inline
auto scale(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param.local_idx;
}

[[nodiscard]] inline
auto speed(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param.local_idx;
}

[[nodiscard]] inline
auto wet(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param.local_idx;
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
		case StdEnv::SCALE:            { return scale(host, plugin_idx); }
		case StdEnv::SPEED:            { return speed(host, plugin_idx); }
		case StdEnv::WET:              { return wet(host, plugin_idx); }
	}
	assert (false);
	return {0};
}

[[nodiscard]] inline
auto custom(Host* host, blink_PluginIdx plugin_idx, blink_UUID uuid) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx);
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
	const auto param      = add::param::empty(host, plugin_idx);
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
	const auto param      = add::param::empty(host, plugin_idx);
	const auto option_idx = add::param::option::empty(host);
	write::type_idx(host, param.global_idx, option_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_NOISE_MODE});
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate);
	write::name(host, param.global_idx, {"Noise Mode"});
	write::strings(host, option_idx, {{"Multiply", "Mix"}});
	return param.local_idx;
}

[[nodiscard]] inline
auto reverse_mode(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx);
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
	const auto param      = add::param::empty(host, plugin_idx);
	const auto option_idx = add::param::option::empty(host);
	const auto flags =
		blink_ParamFlags_CanManipulate |
		blink_ParamFlags_IsToggle |
		blink_ParamFlags_ShowInContextMenu |
		blink_ParamFlags_ShowButton |
		blink_ParamFlags_MovesDisplay |
		blink_ParamFlags_IconOnly;
	write::type_idx(host, param.global_idx, option_idx);
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
	const auto param      = add::param::empty(host, plugin_idx);
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
	const auto param         = add::param::empty(host, plugin_idx);
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
	const auto param         = add::param::empty(host, plugin_idx);
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
auto amp(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {/*TODO:*/});
	// TODO:
	return param.local_idx;
}

inline
auto delay_time(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {/*TODO:*/});
	// TODO:
	return param.local_idx;
}

inline
auto dry(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {/*TODO:*/});
	// TODO:
	return param.local_idx;
}

inline
auto feedback(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {/*TODO:*/});
	// TODO:
	return param.local_idx;
}

inline
auto filter_frequency(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {/*TODO:*/});
	// TODO:
	return param.local_idx;
}

inline
auto filter_resonance(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {/*TODO:*/});
	// TODO:
	return param.local_idx;
}

inline
auto formant(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {/*TODO:*/});
	// TODO:
	return param.local_idx;
}

inline
auto mix(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {/*TODO:*/});
	// TODO:
	return param.local_idx;
}

inline
auto noise_amount(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {/*TODO:*/});
	// TODO:
	return param.local_idx;
}

inline
auto noise_color(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {/*TODO:*/});
	// TODO:
	return param.local_idx;
}

inline
auto noise_width(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {BLINK_STD_UUID_NOISE_WIDTH});
	write::add_flags(host, param.global_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_HostClamp);
	write::name(host, param.global_idx, {"Noise Width"});
	write::short_name(host, param.global_idx, {"Width"});
	write::clamp_range(host, sld_idx, {{0.0f, 1.0f}});
	write::slider(host, sld_idx, add::slider::percentage(host->fns));
	write::offset_env(host, sld_idx, {add::env::percentage_bipolar(host->fns)});
	write::override_env(host, sld_idx, {add::env::percentage(host->fns)});
	return param.local_idx;
}

inline
auto pan(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {/*TODO:*/});
	// TODO:
	return param.local_idx;
}

inline
auto pitch(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {/*TODO:*/});
	// TODO:
	return param.local_idx;
}

inline
auto scale(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {/*TODO:*/});
	// TODO:
	return param.local_idx;
}

inline
auto speed(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {/*TODO:*/});
	// TODO:
	return param.local_idx;
}

inline
auto wet(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx {
	const auto param      = add::param::empty(host, plugin_idx);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param.global_idx, sld_idx);
	write::uuid(host, param.global_idx, {/*TODO:*/});
	// TODO:
	return param.local_idx;
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
		case StdSliderReal::SCALE:            { return scale(host, plugin_idx); }
		case StdSliderReal::SPEED:            { return speed(host, plugin_idx); }
		case StdSliderReal::WET:              { return wet(host, plugin_idx); }
	}
	assert (false);
	return {0};
}

[[nodiscard]] inline
auto custom(Host* host, blink_PluginIdx plugin_idx, blink_UUID uuid) -> blink_ParamIdx {
	const auto param         = add::param::empty(host, plugin_idx);
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
	return process_fn(plugin);
}

inline
auto effect_process(Host* host, blink_UnitIdx unit_idx, const blink_EffectBuffer& buffer, const blink_EffectUnitState& unit_state, const float* in, float* out) -> blink_Error {
	auto process_fn = [unit_idx, &buffer, &unit_state, in, out](const PluginInterface& plugin_iface) -> blink_Error {
		return plugin_iface.effect.process(unit_idx, &buffer, &unit_state, in, out);
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
	auto process_fn = [unit_idx, &buffer, &unit_state, out](const PluginInterface& plugin_iface) -> blink_Error {
		return plugin_iface.sampler.process(unit_idx, &buffer, &unit_state, out);
	};
	return unit_process(host, unit_idx, buffer.unit, unit_state.unit, std::move(process_fn));
}

inline
auto synth_process(Host* host, blink_UnitIdx unit_idx, const blink_SynthBuffer& buffer, const blink_SynthUnitState& unit_state, float* out) -> blink_Error {
	auto process_fn = [unit_idx, &buffer, &unit_state, out](const PluginInterface& plugin_iface) -> blink_Error {
		return plugin_iface.synth.process(unit_idx, &buffer, &unit_state, out);
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
	if (uuid_str.compare(BLINK_STD_UUID_SCALE) == 0) { return StdEnv::SCALE; }
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
	if (uuid_str.compare(BLINK_STD_UUID_SCALE) == 0) { return StdSliderReal::SCALE; }
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
		const auto env_idx       = read::env_idx(*host_ptr(usr), param_env_idx);
		return read::default_value(*host_ptr(usr), env_idx);
	};
	host->fns.read_param_option_default_value = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		const auto type_idx = read::type_idx(*host_ptr(usr), param_global_idx);
		return read::default_value(*host_ptr(usr), ParamOptionIdx{type_idx});
	};
	host->fns.read_param_slider_int_default_value = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		const auto param_slider_idx = ParamSliderIntIdx{read::type_idx(*host_ptr(usr), param_global_idx)};
		const auto slider_idx       = read::slider_idx(*host_ptr(usr), param_slider_idx);
		return read::default_value(*host_ptr(usr), slider_idx);
	};
	host->fns.read_param_slider_real_default_value = [](void* usr, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx) {
		const auto param_global_idx = read::local_to_global(*host_ptr(usr), plugin_idx, param_idx);
		const auto param_slider_idx = ParamSliderRealIdx{read::type_idx(*host_ptr(usr), param_global_idx)};
		const auto slider_idx       = read::slider_idx(*host_ptr(usr), param_slider_idx);
		return read::default_value(*host_ptr(usr), slider_idx);
	};
	host->fns.write_env_default_value = [](void* usr, blink_EnvIdx env_idx, float value) {
		write::default_value(host_ptr(usr), env_idx, {value});
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
	host->fns.write_env_snap_settings = [](void* usr, blink_EnvIdx env_idx, blink_EnvSnapSettings settings) {
		write::snap_settings(host_ptr(usr), env_idx, {settings});
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