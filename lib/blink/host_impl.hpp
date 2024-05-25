#pragma once

#include <blink.h>
#include <blink_std.h>
#include <cassert>
#include "common_impl.hpp"
#include "entity_store.hpp"
#include "math.hpp"
#include "tweak.hpp"
#include "types.hpp"

namespace blink {

namespace ent {
	using Plugin = StaticEntityStore<
		blink_UUID,
		PluginType,
		PluginInterface
	>;
	using Instance = DynamicEntityStore<
		InstanceProcess
	>;
	using Unit = DynamicEntityStore<
		UnitProcess
	>;
	using Param = StaticEntityStore<
		blink_UUID,
		ManipDelegate,
		ParamFlags,
		ParamGroup,
		ParamIcon,
		ParamStrings,
		ParamTypeIdx,
		SubParams
	>;
	using ParamEnv = StaticEntityStore<
		ApplyOffsetFn,
		ClampRange,
		EnvIdx,
		OffsetEnvIdx,
		OverrideEnvIdx
	>;
	using ParamOption = StaticEntityStore<
		DefaultValue<int64_t>,
		StringVec
	>;
	using ParamSliderInt = StaticEntityStore<
		blink_SliderIntIdx
	>;
	using ParamSliderReal = StaticEntityStore<
		ApplyOffsetFn,
		blink_SliderRealIdx,
		ClampRange,
		OffsetEnvIdx,
		OverrideEnvIdx
	>;
	using Env = StaticEntityStore<
		DefaultValue<float>,
		EnvFns,
		EnvSnapSettings,
		MaxSliderIdx,
		MinSliderIdx,
		ValueSliderIdx
	>;
	using SliderInt = StaticEntityStore<
		DefaultValue<int64_t>,
		TweakerInt
	>;
	using SliderReal = StaticEntityStore<
		DefaultValue<float>,
		TweakerReal
	>;
} // ent

struct Host {
	ent::Plugin plugin;
	ent::Instance instance;
	ent::Unit unit;
	ent::Param param;
	ent::ParamEnv param_env;
	ent::ParamOption param_option;
	ent::ParamSliderInt param_slider_int;
	ent::ParamSliderReal param_slider_real;
	ent::Env env;
	ent::SliderInt slider_int;
	ent::SliderReal slider_real;
	blink_HostFns fns;
};

// Define this somewhere in your host application.
extern Host the_host_instance;

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
auto iface(const Host& host, blink_PluginIdx plugin_idx) -> const PluginInterface& {
	return host.plugin.get<PluginInterface>(plugin_idx.value);
}

[[nodiscard]] inline
auto process(Host* host, blink_InstanceIdx instance_idx) -> InstanceProcess& {
	return host->instance.get<InstanceProcess>(instance_idx.value);
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
auto type_idx(const Host& host, blink_ParamIdx param_idx) -> size_t {
	return host.param.get<ParamTypeIdx>(param_idx.value).value;
}

} // read

namespace write {

inline
auto add_flags(Host* host, blink_ParamIdx param_idx, int flags) -> void {
	host->param.get<ParamFlags>(param_idx.value).value.value |= flags;
}

inline
auto add_subparam(Host* host, blink_ParamIdx param_idx, blink_ParamIdx subparam_idx) -> void {
	host->param.get<SubParams>(param_idx.value).value.push_back(subparam_idx);
}

inline
auto apply_offset_fn(Host* host, ParamEnvIdx param_env_idx, blink_ApplyOffsetFn fn) -> void {
	host->param_env.get<ApplyOffsetFn>(param_env_idx.value).fn = fn;
}

inline
auto clamp_range(Host* host, ParamSliderRealIdx sld_idx, ClampRange value) -> void {
	host->param_slider_real.get<ClampRange>(sld_idx.value) = value;
}

inline
auto default_value(Host* host, blink_EnvIdx env_idx, DefaultValue<float> value) -> void {
	host->env.get<DefaultValue<float>>(env_idx.value) = value;
}

inline
auto default_value(Host* host, ParamOptionIdx option_idx, DefaultValue<int64_t> value) -> void {
	host->param_option.get<DefaultValue<int64_t>>(option_idx.value) = value;
}

inline
auto default_value(Host* host, blink_SliderIntIdx sld_idx, DefaultValue<int64_t> value) -> void {
	host->slider_int.get<DefaultValue<int64_t>>(sld_idx.value) = value;
}

inline
auto default_value(Host* host, blink_SliderRealIdx sld_idx, DefaultValue<float> value) -> void {
	host->slider_real.get<DefaultValue<float>>(sld_idx.value) = value;
}

inline
auto env(Host* host, ParamEnvIdx param_env_idx, blink_EnvIdx value) -> void {
	host->param_env.get<EnvIdx>(param_env_idx.value).value = value;
}

inline
auto fns(Host* host, blink_EnvIdx env_idx, EnvFns fns) -> void {
	host->env.get<EnvFns>(env_idx.value) = fns;
}

inline
auto group(Host* host, blink_ParamIdx param_idx, blink_StaticString group) -> void {
	host->param.get<ParamGroup>(param_idx.value).value = group;
}

inline
auto icon(Host* host, blink_ParamIdx param_idx, blink_StdIcon icon) -> void {
	host->param.get<ParamIcon>(param_idx.value).value = icon;
}

inline
auto manip_delegate(Host* host, blink_ParamIdx param_idx, blink_ParamIdx delegate) -> void {
	host->param.get<ManipDelegate>(param_idx.value).value = delegate;
}

inline
auto max_slider(Host* host, blink_EnvIdx env_idx, MaxSliderIdx value) -> void {
	host->env.get<MaxSliderIdx>(env_idx.value) = value;
}

inline
auto min_slider(Host* host, blink_EnvIdx env_idx, MinSliderIdx value) -> void {
	host->env.get<MinSliderIdx>(env_idx.value) = value;
}

inline
auto offset_env(Host* host, ParamEnvIdx env_idx, OffsetEnvIdx value) -> void {
	host->param_env.get<OffsetEnvIdx>(env_idx.value) = value;
}

inline
auto offset_env(Host* host, ParamSliderRealIdx sld_idx, OffsetEnvIdx value) -> void {
	host->param_slider_real.get<OffsetEnvIdx>(sld_idx.value) = value;
}

inline
auto override_env(Host* host, ParamEnvIdx env_idx, OverrideEnvIdx value) -> void {
	host->env.get<OverrideEnvIdx>(env_idx.value) = value;
}

inline
auto override_env(Host* host, ParamSliderRealIdx sld_idx, OverrideEnvIdx value) -> void {
	host->param_slider_real.get<OverrideEnvIdx>(sld_idx.value) = value;
}

inline
auto name(Host* host, blink_ParamIdx param_idx, std::string_view name) -> void {
	host->param.get<ParamStrings>(param_idx.value).value.name = {name.data()};
}

inline
auto short_name(Host* host, blink_ParamIdx param_idx, std::string_view name) -> void {
	host->param.get<ParamStrings>(param_idx.value).value.short_name = {name.data()};
}

inline
auto short_name(Host* host, blink_ParamIdx param_idx, blink_ParamStrings strings) -> void {
	host->param.get<ParamStrings>(param_idx.value).value = strings;
}

inline
auto slider(Host* host, ParamSliderIntIdx sld_idx, blink_SliderIntIdx value) -> void {
	host->param_slider_int.get<blink_SliderIntIdx>(sld_idx.value) = value;
}

inline
auto slider(Host* host, ParamSliderRealIdx sld_idx, blink_SliderRealIdx value) -> void {
	host->param_slider_real.get<blink_SliderRealIdx>(sld_idx.value) = value;
}

inline
auto snap_settings(Host* host, blink_EnvIdx env_idx, EnvSnapSettings value) -> void {
	host->env.get<EnvSnapSettings>(env_idx.value) = value;
}

inline
auto strings(Host* host, blink_ParamIdx param_idx, blink_ParamStrings strings) -> void {
	host->param.get<ParamStrings>(param_idx.value).value = strings;
}

inline
auto strings(Host* host, ParamOptionIdx option_idx, StringVec strings) -> void {
	host->param_option.get<StringVec>(option_idx.value) = std::move(strings);
}

inline
auto tweaker(Host* host, blink_SliderIntIdx sld_idx, TweakerInt value) -> void {
	host->slider_int.get<TweakerInt>(sld_idx.value) = value;
}

inline
auto tweaker(Host* host, blink_SliderRealIdx sld_idx, TweakerReal value) -> void {
	host->slider_real.get<TweakerReal>(sld_idx.value) = value;
}

inline
auto type_idx(Host* host, blink_ParamIdx param_idx, ParamEnvIdx type_idx) -> void {
	host->param.get<ParamTypeIdx>(param_idx.value).value = type_idx.value;
}

inline
auto type_idx(Host* host, blink_ParamIdx param_idx, ParamOptionIdx type_idx) -> void {
	host->param.get<ParamTypeIdx>(param_idx.value).value = type_idx.value;
}

inline
auto type_idx(Host* host, blink_ParamIdx param_idx, ParamSliderIntIdx type_idx) -> void {
	host->param.get<ParamTypeIdx>(param_idx.value).value = type_idx.value;
}

inline
auto type_idx(Host* host, blink_ParamIdx param_idx, ParamSliderRealIdx type_idx) -> void {
	host->param.get<ParamTypeIdx>(param_idx.value).value = type_idx.value;
}

inline
auto uuid(Host* host, blink_ParamIdx param_idx, blink_UUID uuid) -> void {
	host->param.get<blink_UUID>(param_idx.value) = uuid;
}

inline
auto value_slider(Host* host, blink_EnvIdx env_idx, ValueSliderIdx value) -> void {
	host->env.get<ValueSliderIdx>(env_idx.value) = value;
}

} // write

namespace add {

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

[[nodiscard]] inline
auto empty(Host* host) -> blink_ParamIdx {
	return {host->param.push_back()};
}

namespace env {

[[nodiscard]] inline
auto empty(Host* host) -> ParamEnvIdx {
	return {host->env.push_back()};
}

[[nodiscard]] inline
auto amp(Host* host) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_env_idx = add::param::env::empty(host);
	const auto flags = blink_ParamFlags_CanManipulate | blink_ParamFlags_MovesDisplay;
	const auto apply_offset_fn = [](float value, float offset) -> float { return value * offset; };
	write::type_idx(host, param_idx, param_env_idx);
	write::name(host, param_idx, "Amp");
	write::add_flags(host, param_idx, flags);
	write::apply_offset_fn(host, param_env_idx, apply_offset_fn);
	write::env(host, param_env_idx, add::env::amp(host));
	write::offset_env(host, param_env_idx, {add::env::amp(host)});
	write::override_env(host, param_env_idx, {add::env::amp(host)});
	return param_idx;
}

[[nodiscard]] inline
auto delay_time(Host* host) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param_idx;
}

[[nodiscard]] inline
auto dry(Host* host) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param_idx;
}

[[nodiscard]] inline
auto feedback(Host* host) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param_idx;
}

[[nodiscard]] inline
auto filter_frequency(Host* host) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param_idx;
}

[[nodiscard]] inline
auto filter_resonance(Host* host) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param_idx;
}

[[nodiscard]] inline
auto formant(Host* host) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param_idx;
}

[[nodiscard]] inline
auto mix(Host* host) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param_idx;
}

[[nodiscard]] inline
auto noise_amount(Host* host) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param_idx;
}

[[nodiscard]] inline
auto noise_color(Host* host) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param_idx;
}

[[nodiscard]] inline
auto noise_width(Host* host) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param_idx;
}

[[nodiscard]] inline
auto pan(Host* host) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param_idx;
}

[[nodiscard]] inline
auto pitch(Host* host) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param_idx;
}

[[nodiscard]] inline
auto scale(Host* host) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param_idx;
}

[[nodiscard]] inline
auto speed(Host* host) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param_idx;
}

[[nodiscard]] inline
auto wet(Host* host) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_env_idx = add::param::env::empty(host);
	// TODO:
	return param_idx;
}

[[nodiscard]] inline
auto std(Host* host, StdEnv std_env) -> blink_ParamIdx {
	switch (std_env) {
		case StdEnv::AMP:              { return amp(host); }
		case StdEnv::DELAY_TIME:       { return delay_time(host); }
		case StdEnv::DRY:              { return dry(host); }
		case StdEnv::FEEDBACK:         { return feedback(host); }
		case StdEnv::FILTER_FREQUENCY: { return filter_frequency(host); }
		case StdEnv::FILTER_RESONANCE: { return filter_resonance(host); }
		case StdEnv::FORMANT:          { return formant(host); }
		case StdEnv::MIX:              { return mix(host); }
		case StdEnv::NOISE_AMOUNT:     { return noise_amount(host); }
		case StdEnv::NOISE_COLOR:      { return noise_color(host); }
		case StdEnv::NOISE_WIDTH:      { return noise_width(host); }
		case StdEnv::PAN:              { return pan(host); }
		case StdEnv::PITCH:            { return pitch(host); }
		case StdEnv::SCALE:            { return scale(host); }
		case StdEnv::SPEED:            { return speed(host); }
		case StdEnv::WET:              { return wet(host); }
	}
	assert (false);
	return {0};
}

[[nodiscard]] inline
auto custom(Host* host, blink_UUID uuid) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_env_idx = add::param::env::empty(host);
	const auto env_idx       = add::env::empty(host->fns);
	write::type_idx(host, param_idx, param_env_idx);
	write::env(host, param_env_idx, env_idx);
	write::uuid(host, param_idx, uuid);
	return param_idx;
}

} // env

namespace option {

[[nodiscard]] inline
auto empty(Host* host) -> ParamOptionIdx {
	return {host->param_option.push_back()};
}

[[nodiscard]] inline
auto loop(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto option_idx = add::param::option::empty(host);
	const auto flags = 
		blink_ParamFlags_IsToggle |
		blink_ParamFlags_ShowInContextMenu |
		blink_ParamFlags_ShowButton |
		blink_ParamFlags_MovesDisplay |
		blink_ParamFlags_IconOnly;
	write::type_idx(host, param_idx, option_idx);
	write::icon(host, param_idx, blink_StdIcon_Loop);
	write::name(host, param_idx, "Loop");
	write::uuid(host, param_idx, {BLINK_STD_UUID_LOOP});
	write::add_flags(host, param_idx, flags);
	write::default_value(host, option_idx, {0});
	return param_idx;
}

[[nodiscard]] inline
auto noise_mode(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto option_idx = add::param::option::empty(host);
	write::type_idx(host, param_idx, option_idx);
	write::uuid(host, param_idx, {BLINK_STD_UUID_NOISE_MODE});
	write::add_flags(host, param_idx, blink_ParamFlags_CanManipulate);
	write::name(host, param_idx, "Noise Mode");
	write::strings(host, option_idx, {{"Multiply", "Mix"}});
	return param_idx;
}

[[nodiscard]] inline
auto reverse_mode(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto option_idx = add::param::option::empty(host);
	write::type_idx(host, param_idx, option_idx);
	write::uuid(host, param_idx, {BLINK_STD_UUID_REVERSE_MODE});
	write::name(host, param_idx, "Reverse");
	write::add_flags(host, param_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_Hidden | blink_ParamFlags_MovesDisplay);
	write::default_value(host, option_idx, {-1});
	write::strings(host, option_idx, {{"Mirror", "Tape", "Slip"}});
	return param_idx;
}

[[nodiscard]] inline
auto reverse_toggle(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto option_idx = add::param::option::empty(host);
	const auto flags =
		blink_ParamFlags_CanManipulate |
		blink_ParamFlags_IsToggle |
		blink_ParamFlags_ShowInContextMenu |
		blink_ParamFlags_ShowButton |
		blink_ParamFlags_MovesDisplay |
		blink_ParamFlags_IconOnly;
	write::type_idx(host, param_idx, option_idx);
	write::uuid(host, param_idx, {BLINK_STD_UUID_REVERSE_TOGGLE});
	write::name(host, param_idx, "Reverse");
	write::icon(host, param_idx, blink_StdIcon_Reverse);
	write::default_value(host, option_idx, {0});
	return param_idx;
}

[[nodiscard]] inline
auto std(Host* host, StdOption std_option) -> blink_ParamIdx {
	switch (std_option) {
		case StdOption::LOOP:           { return loop(host); }
		case StdOption::NOISE_MODE:     { return noise_mode(host); }
		case StdOption::REVERSE_MODE:   { return reverse_mode(host); }
		case StdOption::REVERSE_TOGGLE: { return reverse_toggle(host); }
	}
	assert (false);
	return {0};
}

[[nodiscard]] inline
auto custom(Host* host, blink_UUID uuid) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto option_idx = add::param::option::empty(host);
	write::type_idx(host, param_idx, option_idx);
	write::uuid(host, param_idx, uuid);
	return param_idx;
}

} // option

namespace slider_int {

[[nodiscard]] inline
auto empty(Host* host) -> ParamSliderIntIdx {
	return {host->param_slider_int.push_back()};
}

[[nodiscard]] inline
auto sample_offset(Host* host) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto sld_param_idx = add::param::slider_int::empty(host);
	const auto sld_idx       = add::slider::sample_offset(host);
	write::type_idx(host, param_idx, sld_param_idx);
	write::icon(host, param_idx, blink_StdIcon_SampleOffset);
	write::add_flags(host, param_idx, blink_ParamFlags_MovesDisplay);
	write::name(host, param_idx, "Sample Offset");
	write::slider(host, sld_param_idx, sld_idx);
	write::uuid(host, param_idx, {BLINK_STD_UUID_SAMPLE_OFFSET});
	return param_idx;
}

[[nodiscard]] inline
auto std(Host* host, StdSliderInt std_sld) -> blink_ParamIdx {
	switch (std_sld) {
		case StdSliderInt::SAMPLE_OFFSET: { return sample_offset(host); }
	}
	assert (false);
	return {0};
}

[[nodiscard]] inline
auto custom(Host* host, blink_UUID uuid) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_sld_idx = add::param::slider_int::empty(host);
	const auto sld_idx       = add::slider::empty_int(host->fns);
	write::type_idx(host, param_idx, param_sld_idx);
	write::slider(host, param_sld_idx, sld_idx);
	write::uuid(host, param_idx, uuid);
	return param_idx;
}

} // slider_int
namespace slider_real {

[[nodiscard]] inline
auto empty(Host* host) -> ParamSliderRealIdx {
	return {host->param_slider_real.push_back()};
}

inline
auto amp(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param_idx, sld_idx);
	write::uuid(host, param_idx, {/*TODO:*/});
	// TODO:
	return param_idx;
}

inline
auto delay_time(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param_idx, sld_idx);
	write::uuid(host, param_idx, {/*TODO:*/});
	// TODO:
	return param_idx;
}

inline
auto dry(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param_idx, sld_idx);
	write::uuid(host, param_idx, {/*TODO:*/});
	// TODO:
	return param_idx;
}

inline
auto feedback(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param_idx, sld_idx);
	write::uuid(host, param_idx, {/*TODO:*/});
	// TODO:
	return param_idx;
}

inline
auto filter_frequency(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param_idx, sld_idx);
	write::uuid(host, param_idx, {/*TODO:*/});
	// TODO:
	return param_idx;
}

inline
auto filter_resonance(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param_idx, sld_idx);
	write::uuid(host, param_idx, {/*TODO:*/});
	// TODO:
	return param_idx;
}

inline
auto formant(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param_idx, sld_idx);
	write::uuid(host, param_idx, {/*TODO:*/});
	// TODO:
	return param_idx;
}

inline
auto mix(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param_idx, sld_idx);
	write::uuid(host, param_idx, {/*TODO:*/});
	// TODO:
	return param_idx;
}

inline
auto noise_amount(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param_idx, sld_idx);
	write::uuid(host, param_idx, {/*TODO:*/});
	// TODO:
	return param_idx;
}

inline
auto noise_color(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param_idx, sld_idx);
	write::uuid(host, param_idx, {/*TODO:*/});
	// TODO:
	return param_idx;
}

inline
auto noise_width(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param_idx, sld_idx);
	write::uuid(host, param_idx, {BLINK_STD_UUID_NOISE_WIDTH});
	write::add_flags(host, param_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_HostClamp);
	write::name(host, param_idx, "Noise Width");
	write::short_name(host, param_idx, "Width");
	write::clamp_range(host, sld_idx, {{0.0f, 1.0f}});
	write::slider(host, sld_idx, add::slider::percentage(host->fns));
	write::offset_env(host, sld_idx, {add::env::percentage_bipolar(host->fns)});
	write::override_env(host, sld_idx, {add::env::percentage(host->fns)});
	return param_idx;
}

inline
auto pan(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param_idx, sld_idx);
	write::uuid(host, param_idx, {/*TODO:*/});
	// TODO:
	return param_idx;
}

inline
auto pitch(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param_idx, sld_idx);
	write::uuid(host, param_idx, {/*TODO:*/});
	// TODO:
	return param_idx;
}

inline
auto scale(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param_idx, sld_idx);
	write::uuid(host, param_idx, {/*TODO:*/});
	// TODO:
	return param_idx;
}

inline
auto speed(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param_idx, sld_idx);
	write::uuid(host, param_idx, {/*TODO:*/});
	// TODO:
	return param_idx;
}

inline
auto wet(Host* host) -> blink_ParamIdx {
	const auto param_idx  = add::param::empty(host);
	const auto sld_idx = add::param::slider_real::empty(host);
	write::type_idx(host, param_idx, sld_idx);
	write::uuid(host, param_idx, {/*TODO:*/});
	// TODO:
	return param_idx;
}

[[nodiscard]] inline
auto std(Host* host, StdSliderReal std_sld) -> blink_ParamIdx {
	switch (std_sld) {
		case StdSliderReal::AMP:              { return amp(host); }
		case StdSliderReal::DELAY_TIME:       { return delay_time(host); }
		case StdSliderReal::DRY:              { return dry(host); }
		case StdSliderReal::FEEDBACK:         { return feedback(host); }
		case StdSliderReal::FILTER_FREQUENCY: { return filter_frequency(host); }
		case StdSliderReal::FILTER_RESONANCE: { return filter_resonance(host); }
		case StdSliderReal::FORMANT:          { return formant(host); }
		case StdSliderReal::MIX:              { return mix(host); }
		case StdSliderReal::NOISE_AMOUNT:     { return noise_amount(host); }
		case StdSliderReal::NOISE_COLOR:      { return noise_color(host); }
		case StdSliderReal::NOISE_WIDTH:      { return noise_width(host); }
		case StdSliderReal::PAN:              { return pan(host); }
		case StdSliderReal::PITCH:            { return pitch(host); }
		case StdSliderReal::SCALE:            { return scale(host); }
		case StdSliderReal::SPEED:            { return speed(host); }
		case StdSliderReal::WET:              { return wet(host); }
	}
	assert (false);
	return {0};
}

[[nodiscard]] inline
auto custom(Host* host, blink_UUID uuid) -> blink_ParamIdx {
	const auto param_idx     = add::param::empty(host);
	const auto param_sld_idx = add::param::slider_real::empty(host);
	const auto sld_idx       = add::slider::empty_real(host->fns);
	write::type_idx(host, param_idx, param_sld_idx);
	write::slider(host, param_sld_idx, sld_idx);
	write::uuid(host, param_idx, uuid);
	return param_idx;
}

} // slider_real

} // param
} // add

inline
auto begin_unit_process(Host* host, const PluginInterface& plugin, blink_InstanceIdx instance_idx, blink_BufferID buffer_id, blink_SR SR) -> void {
	auto& process = read::process(host, instance_idx);
	if (buffer_id.value > process.buffer_id.value.value) {
		if (buffer_id.value > process.buffer_id.value.value + 1 || process.active_buffer_units == 0) {
			// instance is reset at the start of the buffer if we have gone
			// at least one buffer with no units being processed
			plugin.blink_instance_reset(process.local_idx.value, SR);
		}
		process.buffer_id.value     = buffer_id;
		process.active_buffer_units = 0;
	}
	process.active_buffer_units++;
}

[[nodiscard]] inline
auto sampler_process(Host* host, blink_UnitIdx unit_idx, const blink_SamplerBuffer& buffer, const blink_SamplerUnitState& unit_state, float* out) -> blink_Error {
	auto& process      = host->unit.get<UnitProcess>(unit_idx.value);
	const auto& plugin = read::iface(*host, process.plugin_idx.value);
	begin_unit_process(host, plugin, process.instance_idx, buffer.buffer_id, buffer.SR);
	if (buffer.buffer_id.value > process.buffer_id.value.value + 1) {
		// unit is reset at the start of the buffer if we have gone
		// at least one buffer without processing this unit
		plugin.blink_unit_reset(process.local_idx.value, buffer.SR);
	}
	process.buffer_id.value = buffer.buffer_id;
	return plugin.sampler.blink_sampler_process(unit_idx, &buffer, &unit_state, out);
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
auto make_host_fns() -> blink_HostFns {
	blink_HostFns fns;
	fns.add_env = []() {
		return add::env::empty(&the_host_instance);
	};
	fns.add_param_env = [](blink_UUID uuid) {
		if (const auto std_param = get_std_env(uuid)) {
			return add::param::env::std(&the_host_instance, *std_param);
		}
		else {
			return add::param::env::custom(&the_host_instance, uuid);
		}
	};
	fns.add_param_option = [](blink_UUID uuid) {
		if (const auto std_param = get_std_option(uuid)) {
			return add::param::option::std(&the_host_instance, *std_param);
		}
		else {
			return add::param::option::custom(&the_host_instance, uuid);
		}
	};
	fns.add_param_slider_int = [](blink_UUID uuid) {
		if (const auto std_param = get_std_slider_int(uuid)) {
			return add::param::slider_int::std(&the_host_instance, *std_param);
		}
		else {
			return add::param::slider_int::custom(&the_host_instance, uuid);
		}
	};
	fns.add_param_slider_real = [](blink_UUID uuid) {
		if (const auto std_param = get_std_slider_real(uuid)) {
			return add::param::slider_real::std(&the_host_instance, *std_param);
		}
		else {
			return add::param::slider_real::custom(&the_host_instance, uuid);
		}
	};
	fns.add_slider_int = []() {
		return add::slider::empty_int(&the_host_instance);
	};
	fns.add_slider_real = []() {
		return add::slider::empty_real(&the_host_instance);
	};
	fns.read_env_default_value = [](blink_EnvIdx env_idx) {
		return read::default_value(the_host_instance, env_idx);
	};
	fns.read_param_env_env_idx = [](blink_ParamIdx param_idx) {
		const auto type_idx = read::type_idx(the_host_instance, param_idx);
		const auto env_idx  = read::env_idx(the_host_instance, ParamEnvIdx{type_idx});
		return env_idx;
	};
	fns.read_param_option_default_value = [](blink_ParamIdx param_idx) {
		const auto type_idx = read::type_idx(the_host_instance, param_idx);
		return read::default_value(the_host_instance, ParamOptionIdx{type_idx});
	};
	fns.read_param_slider_int_slider_idx = [](blink_ParamIdx param_idx) {
		const auto type_idx   = read::type_idx(the_host_instance, param_idx);
		const auto slider_idx = read::slider_idx(the_host_instance, ParamSliderIntIdx{type_idx});
		return slider_idx;
	};
	fns.read_param_slider_real_slider_idx = [](blink_ParamIdx param_idx) {
		const auto type_idx   = read::type_idx(the_host_instance, param_idx);
		const auto slider_idx = read::slider_idx(the_host_instance, ParamSliderRealIdx{type_idx});
		return slider_idx;
	};
	fns.read_slider_int_default_value = [](blink_SliderIntIdx slider_idx) {
		return read::default_value(the_host_instance, slider_idx);
	};
	fns.read_slider_real_default_value = [](blink_SliderRealIdx slider_idx) {
		return read::default_value(the_host_instance, slider_idx);
	};
	fns.write_env_default_value = [](blink_EnvIdx env_idx, float value) {
		write::default_value(&the_host_instance, env_idx, {value});
	};
	fns.write_env_fns = [](blink_EnvIdx env_idx, blink_EnvFns fns) {
		write::fns(&the_host_instance, env_idx, {fns});
	};
	fns.write_env_max_slider = [](blink_EnvIdx env_idx, blink_SliderRealIdx slider_idx) {
		write::max_slider(&the_host_instance, env_idx, {slider_idx});
	};
	fns.write_env_min_slider = [](blink_EnvIdx env_idx, blink_SliderRealIdx slider_idx) {
		write::min_slider(&the_host_instance, env_idx, {slider_idx});
	};
	fns.write_env_snap_settings = [](blink_EnvIdx env_idx, blink_EnvSnapSettings settings) {
		write::snap_settings(&the_host_instance, env_idx, {settings});
	};
	fns.write_env_value_slider = [](blink_EnvIdx env_idx, blink_SliderRealIdx slider_idx) {
		write::value_slider(&the_host_instance, env_idx, {slider_idx});
	};
	fns.write_param_add_flags = [](blink_ParamIdx param_idx, int flags) {
		write::add_flags(&the_host_instance, param_idx, {flags});
	};
	fns.write_param_add_subparam = [](blink_ParamIdx param_idx, blink_ParamIdx subparam_idx) {
		write::add_subparam(&the_host_instance, param_idx, {subparam_idx});
	};
	fns.write_param_group = [](blink_ParamIdx param_idx, blink_StaticString group) {
		write::group(&the_host_instance, param_idx, group);
	};
	fns.write_param_icon = [](blink_ParamIdx param_idx, blink_StdIcon icon) {
		write::icon(&the_host_instance, param_idx, icon);
	};
	fns.write_param_manip_delegate = [](blink_ParamIdx param_idx, blink_ParamIdx delegate_idx) {
		write::manip_delegate(&the_host_instance, param_idx, {delegate_idx});
	};
	fns.write_param_strings = [](blink_ParamIdx param_idx, blink_ParamStrings strings) {
		write::strings(&the_host_instance, param_idx, strings);
	};
	fns.write_slider_int_default_value = [](blink_SliderIntIdx slider_idx, int64_t value) {
		write::default_value(&the_host_instance, slider_idx, {value});
	};
	fns.write_slider_int_tweaker = [](blink_SliderIntIdx slider_idx, blink_TweakerInt tweaker) {
		write::tweaker(&the_host_instance, slider_idx, {tweaker});
	};
	fns.write_slider_real_default_value = [](blink_SliderRealIdx slider_idx, float value) {
		write::default_value(&the_host_instance, slider_idx, {value});
	};
	fns.write_slider_real_tweaker = [](blink_SliderRealIdx slider_idx, blink_TweakerReal tweaker) {
		write::tweaker(&the_host_instance, slider_idx, {tweaker});
	};
	return fns;
}

} // blink