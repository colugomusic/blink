#pragma once

#include <blink.h>
#include <blink_std.h>
#include "entity_store.hpp"
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
		InstanceProcess,
		LocalInstanceIdx
	>;
	using Unit = DynamicEntityStore<
		UnitProcess,
		LocalUnitIdx
	>;
	using Param = StaticEntityStore<
		blink_UUID,
		ParamFlags,
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
};

namespace read {

[[nodiscard]] inline
auto type_idx(const Host& host, blink_ParamIdx param_idx) -> size_t {
	return host.param.get<ParamTypeIdx>(param_idx.value).value;
}

} // read

namespace write {
namespace env {

inline
auto default_value(Host* host, blink_EnvIdx env_idx, DefaultValue<float> value) -> void {
	host->env.get<DefaultValue<float>>(env_idx.value) = value;
}

inline
auto fns(Host* host, blink_EnvIdx env_idx, EnvFns fns) -> void {
	host->env.get<EnvFns>(env_idx.value) = fns;
}

} // env

namespace slider {

inline
auto default_value(Host* host, blink_SliderRealIdx sld_idx, DefaultValue<float> value) -> void {
	host->slider_real.get<DefaultValue<float>>(sld_idx.value) = value;
}

inline
auto tweaker(Host* host, blink_SliderRealIdx sld_idx, blink_TweakerReal value) -> void {
	host->slider_real.get<blink_TweakerReal>(sld_idx.value) = value;
}

} // slider

namespace param {

inline
auto add_flags(Host* host, blink_ParamIdx param_idx, int flags) -> void {
	host->param.get<ParamFlags>(param_idx.value).value.value |= flags;
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
auto uuid(Host* host, blink_ParamIdx param_idx, blink_UUID uuid) -> void {
	host->param.get<blink_UUID>(param_idx.value) = uuid;
}

namespace option {

inline
auto default_value(Host* host, size_t type_idx, DefaultValue<int64_t> value) -> void {
	host->param_option.get<DefaultValue<int64_t>>(type_idx) = value;
}

inline
auto strings(Host* host, size_t type_idx, StringVec strings) -> void {
	host->param_option.get<StringVec>(type_idx) = std::move(strings);
}

} // option

namespace slider_real {

inline
auto clamp_range(Host* host, size_t type_idx, ClampRange value) -> void {
	host->param_slider_real.get<ClampRange>(type_idx) = value;
}

inline
auto offset_env(Host* host, size_t type_idx, OffsetEnvIdx value) -> void {
	host->param_slider_real.get<OffsetEnvIdx>(type_idx) = value;
}

inline
auto override_env(Host* host, size_t type_idx, OverrideEnvIdx value) -> void {
	host->param_slider_real.get<OverrideEnvIdx>(type_idx) = value;
}

inline
auto slider(Host* host, size_t type_idx, blink_SliderRealIdx value) -> void {
	host->param_slider_real.get<blink_SliderRealIdx>(type_idx) = value;
}

} // slider_real

} // param;
} // write

[[nodiscard]] inline
auto get_tweaker(const Host& host, blink_SliderIntIdx sld_idx) -> const blink_TweakerInt& {
	return host.slider_int.get<blink_TweakerInt>(sld_idx.value);
}

namespace add {

namespace slider {

[[nodiscard]] inline
auto basic_int(Host* host) -> blink_SliderIntIdx {
	return {host->slider_int.push_back()};
}

[[nodiscard]] inline
auto basic_real(Host* host) -> blink_SliderRealIdx {
	return {host->slider_real.push_back()};
}

template <int MIN = 0, int MAX = 100> [[nodiscard]]
auto percentage(Host* host) -> blink_SliderRealIdx {
	const auto idx = blink_SliderRealIdx{host->slider_real.push_back()};
	// TODO:
	tweak::Spec<float> tweaker;
	tweaker.constrain   = [](float v) { return std::clamp(v, float(MIN) / 100.0f, float(MAX) / 100.0f); };
	tweaker.decrement   = tweak::std::percentage::decrement;
	tweaker.drag        = tweak::std::percentage::drag;
	tweaker.from_string = tweak::std::percentage::from_string;
	tweaker.increment   = tweak::std::percentage::increment;
	tweaker.stepify     = tweak::std::percentage::stepify;
	tweaker.to_string   = tweak::std::percentage::to_string;
	write::slider::default_value(plugin, {idx}, {0});
	write::slider::tweaker(plugin, {idx}, std::move(tweaker));
	return {idx};
}

} // slider

namespace env {

[[nodiscard]] inline
auto basic(Host* host) -> blink_EnvIdx {
	return {host->env.push_back()};
}

template <int MIN = 0, int MAX = 100> [[nodiscard]]
auto percentage(Host* host) -> blink_EnvIdx {
	const auto idx = blink_EnvIdx{host->env.push_back()};
	EnvFns fns;
	fns.value.stepify   = tweak::percentage::stepify;
	fns.value.to_string = tweak::percentage::to_string;
	write::env::default_value(host, idx, {0.0f});
	write::env::fns(host, idx, fns);
	write::env::value_slider(host, idx, add::slider::percentage_value<MIN, MAX>(host));
	const auto max_slider = add::slider::basic_real();
	const auto min_slider = add::slider::basic_real();
	write::slider::default_value(host, max_slider, {float(MAX) / 100.0f});
	write::slider::default_value(host, min_slider, {float(MIN) / 100.0f});
	write::env::max_slider(host, idx, max_slider);
	write::env::min_slider(host, idx, min_slider);
	// TODO:
	//write::env::set_max_slider(plugin, idx, add::slider::
		//DefaultValue<float>,
		//EnvSnapSettings,
		//MaxSliderIdx,
		//MinSliderIdx,
		//RealSearcher,
		//ValueSliderIdx,
    out.default_value = 0.0f;
    out.searcher.binary = search::float_points_binary;
    out.searcher.forward = search::float_points_forward;
    out.stepify = tweak::std::percentage::stepify;
    out.value_slider = percentage::slider<MIN, MAX>();
    out.range.min.default_value = float(MIN) / 100.0f;
    out.range.min.to_string = tweak::std::percentage::to_string;
    out.range.max.default_value = float(MAX) / 100.0f;
    out.range.max.to_string = tweak::std::percentage::to_string;
    out.to_string = tweak::std::percentage::to_string;

    return out;
}

} // env

namespace param {

[[nodiscard]] inline
auto push_back(Host* host, blink_UUID uuid) -> blink_ParamIdx {
	const auto param_idx = host->param.push_back();
	write::param::uuid(host, {param_idx}, uuid);
	return {param_idx};
}

namespace option {

inline
auto push_back(Host* host, blink_UUID uuid) -> blink_ParamIdx {
	const auto param_idx        = add::param::push_back(host, uuid);
	const auto param_option_idx = host->param_option.push_back();
	host->param.get<ParamTypeIdx>(param_idx.value).value = param_option_idx;
	return param_idx;
}

inline
auto noise_mode(Host* host) -> blink_ParamIdx {
	const auto param_idx = add::param::push_back(host, {BLINK_STD_UUID_NOISE_MODE});
	const auto type_idx  = read::type_idx(*host, param_idx);
	write::param::add_flags(host, param_idx, blink_ParamFlags_CanManipulate);
	write::param::name(host, param_idx, "Noise Mode");
	write::param::option::strings(host, type_idx, {{"Multiply", "Mix"}});
	return param_idx;
}

inline
auto reverse(Host* host) -> blink_ParamIdx {
	const auto param_idx = add::param::push_back(host, {BLINK_STD_UUID_REVERSE_MODE});
	const auto type_idx   = read::type_idx(*host, param_idx);
	write::param::name(host, param_idx, "Reverse");
	write::param::add_flags(host, param_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_Hidden | blink_ParamFlags_MovesDisplay);
	write::param::option::default_value(host, type_idx, {-1});
	write::param::option::strings(host, type_idx, {{"Mirror", "Tape", "Slip"}});
	return param_idx;
}

} // option

namespace slider_real {

inline
auto noise_width(Host* host) -> blink_ParamIdx {
	const auto param_idx = add::param::push_back(host, {BLINK_STD_UUID_NOISE_WIDTH});
	const auto type_idx   = read::type_idx(*host, param_idx);
	write::param::add_flags(host, param_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_HostClamp);
	write::param::name(host, param_idx, "Noise Width");
	write::param::short_name(host, param_idx, "Width");
	write::param::slider_real::clamp_range(host, type_idx, {{0.0f, 1.0f}});
	write::param::slider_real::slider(host, type_idx, add::slider::percentage(host));
	write::param::slider_real::offset_env(host, type_idx, add::env::percentage_bipolar(host));
	write::param::slider_real::override_env(host, type_idx, add::env::percentage(host));
	return param_idx;
}

} // slider_real

} // param
} // add

/* TODO: move this logic to host
template <typename InstanceResetFn>
auto begin_process(Host* host, blink_InstanceIdx instance_idx, blink_BufferID buffer_id, InstanceResetFn&& instance_reset_fn) -> void {
	// instance is reset at the start of the buffer if we have gone
	// at least one buffer with no units being processed
	auto& instance_process = host->instance.get<InstanceProcess>(instance_idx.value);
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

/* TODO: move this logic to the host
template <typename UnitProcessFn, typename UnitResetFn, typename InstanceResetFn> [[nodiscard]]
auto sampler_process(Host* host, blink_UnitIdx unit_idx, const blink_SamplerBuffer& buffer, const blink_SamplerUnitState& unit_state, float* out, UnitProcessFn&& process_fn, UnitResetFn&& unit_reset_fn, InstanceResetFn&& instance_reset_fn) -> blink_Error {
	auto& unit_proc = host->unit.get<UnitProcess>(unit_idx.value);
	begin_process(plugin, unit_proc.instance_idx, buffer.buffer_id, instance_reset_fn);
	if (buffer.buffer_id.value > unit_proc.buffer_id.value.value + 1) {
		// unit is reset at the start of the buffer if we have gone
		// at least one buffer without processing this unit
		unit_reset_fn(unit_idx);
	}
	unit_proc.buffer_id.value = buffer.buffer_id;
	unit_proc.block_positions(buffer.positions, kFloatsPerDSPVector);
	return process_fn(buffer, unit_state, out);
}
*/

} // blink