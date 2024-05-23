#pragma once

#include <blink.h>
#include "entity_store.hpp"
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

[[nodiscard]] inline
auto from_string(const Host& host, blink_SliderIntIdx sld_idx, std::string_view str, int64_t* out) -> blink_Bool {
	if (const auto value = tweak::from_string(get_tweaker(plugin, sld_idx), str.data())) {
		*out = value.value();
		return {true};
	}
	return {false};
}

[[nodiscard]] inline
auto to_string(const Host& host, blink_SliderIntIdx sld_idx, int64_t value) -> blink_TempString {
	static auto str = tweak::to_string(get_tweaker(plugin, sld_idx), value);
	return {str.c_str()};
}

[[nodiscard]] inline 
auto constrain(const Host& host, blink_SliderIntIdx sld_idx, int64_t value) -> int64_t {
	return tweak::constrain(get_tweaker(plugin, sld_idx), value);
}

[[nodiscard]] inline 
auto decrement(const Host& host, blink_SliderIntIdx sld_idx, int64_t value, blink_Bool precise) -> int64_t {
	return tweak::decrement(get_tweaker(plugin, sld_idx), value, precise.value > 0);
}

[[nodiscard]] inline 
auto drag(const Host& host, blink_SliderIntIdx sld_idx, int64_t start_value, int64_t amount, blink_Bool precise) -> int64_t {
	return tweak::drag(get_tweaker(plugin, sld_idx), start_value, amount, precise.value > 0);
}

[[nodiscard]] inline 
auto get_default_value(const Host& host, blink_SliderIntIdx sld_idx) -> int64_t {
	return host.slider_int.get<DefaultValue<int64_t>>(sld_idx.value).value;
}

[[nodiscard]] inline 
auto increment(const Host& host, blink_SliderIntIdx sld_idx, int64_t value, blink_Bool precise) -> int64_t {
	return tweak::increment(get_tweaker(plugin, sld_idx), value, precise.value > 0);
}

namespace add {

namespace env {

template <int MIN = 0, int MAX = 100> [[nodiscard]]
auto percentage(Host* host) -> blink_EnvelopeIdx {
	const auto idx = host->env.push_back();
	write::env::default_value(plugin, idx, {0.0f});
	write::env::searcher(plugin, idx, make_default_real_searcher({0.0f});
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

namespace slider {

template <int MIN = 0, int MAX = 100> [[nodiscard]]
auto percentage(Host* host) -> blink_SliderRealIdx {
	const auto idx = host->slider_real.push_back();
	tweak::Spec<float> tweaker;
	tweaker.constrain   = [](float v) { return std::clamp(v, float(MIN) / 100.0f, float(MAX) / 100.0f); };
	tweaker.decrement   = tweak::std::percentage::decrement;
	tweaker.drag        = tweak::std::percentage::drag;
	tweaker.from_string = tweak::std::percentage::from_string;
	tweaker.increment   = tweak::std::percentage::increment;
	tweaker.stepify     = tweak::std::percentage::stepify;
	tweaker.to_string   = tweak::std::percentage::to_string;
	write::slider::default_value(plugin, {idx}, {0});
	write::slider::searcher(plugin, {idx}, make_default_real_searcher({0}));
	write::slider::tweaker(plugin, {idx}, std::move(tweaker));
	return {idx};
}

} // slider

namespace param {

[[nodiscard]] inline
auto push_back(Host* host, blink_UUID uuid, bool custom) -> blink_ParamIdx {
	const auto param_idx = host->param.push_back();
	host->param.get<blink_UUID>(param_idx) = uuid;
	if (custom) {
		const auto custom_idx = host->param.push_back();
		host->param.get<ParamCustomIdx>(param_idx).value = custom_idx;
	}
	return {param_idx};
}

namespace option {

inline
auto push_back(Host* host, blink_UUID uuid, bool custom) -> blink_ParamIdx {
	const auto param_idx = add::param::push_back(plugin, uuid, custom);
	if (custom) {
		const auto custom_idx       = read::custom_idx(*plugin, param_idx).value.value();
		const auto param_option_idx = host->param_option.push_back();
		host->param.get<ParamTypeIdx>(custom_idx).value = param_option_idx;
	}
	return param_idx;
}

inline
auto noise_mode(Host* host, bool custom) -> blink_ParamIdx {
	const auto param_idx = add::param::push_back(plugin, {BLINK_STD_UUID_NOISE_MODE}, custom);
	if (custom) {
		const auto custom_idx = read::custom_idx(*plugin, param_idx).value.value();
		const auto type_idx   = read::type_idx(*plugin, custom_idx);
		write::param::add_flags(plugin, custom_idx, blink_ParamFlags_CanManipulate);
		write::param::name(plugin, custom_idx, "Noise Mode");
		write::param::option::strings(plugin, type_idx, {{"Multiply", "Mix"}});
	}
	return param_idx;
}

inline
auto reverse(Host* host, bool custom) -> blink_ParamIdx {
	const auto param_idx = add::param::push_back(plugin, {BLINK_STD_UUID_REVERSE_MODE}, custom);
	if (custom) {
		const auto custom_idx = read::custom_idx(*plugin, param_idx).value.value();
		const auto type_idx   = read::type_idx(*plugin, custom_idx);
		write::param::name(plugin, custom_idx, "Reverse");
		write::param::add_flags(plugin, custom_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_Hidden | blink_ParamFlags_MovesDisplay);
		write::param::option::default_value(plugin, type_idx, {-1});
		write::param::option::searcher(plugin, type_idx, make_default_int_searcher({-1}));
		write::param::option::strings(plugin, type_idx, {{"Mirror", "Tape", "Slip"}});
	}
	return param_idx;
}

} // option

namespace slider_real {

inline
auto noise_width(Host* host, bool custom) -> blink_ParamIdx {
	const auto param_idx = add::param::push_back(plugin, {BLINK_STD_UUID_NOISE_WIDTH}, custom);
	if (custom) {
		const auto custom_idx = read::custom_idx(*plugin, param_idx).value.value();
		const auto type_idx   = read::type_idx(*plugin, custom_idx);
		write::param::add_flags(plugin, custom_idx, blink_ParamFlags_CanManipulate | blink_ParamFlags_HostClamp);
		write::param::name(plugin, custom_idx, "Noise Width");
		write::param::short_name(plugin, custom_idx, "Width");
		write::param::slider_real::clamp_range(plugin, type_idx, {{0.0f, 1.0f}});
		write::param::slider_real::slider(plugin, type_idx, add::slider::percentage(plugin));
		write::param::slider_real::offset_env(plugin, type_idx, add::env::percentage_bipolar(plugin));
		write::param::slider_real::override_env(plugin, type_idx, add::env::percentage(plugin));
	}
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