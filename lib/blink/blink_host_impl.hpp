#pragma once

#include "blink.h"
#include "entity_store.hpp"
#include "types.hpp"

namespace blink {

//
//struct ChordSearcher {
//	using SearchFn = auto (*)(const blink_ChordData* data, blink_Position block_pos, size_t search_beg_index, size_t* left) -> blink_Scale;
//	SearchFn binary;
//	SearchFn forward;
//};
//
//struct RealSearcher {
//	using SearchFn = auto (*)(const blink_RealPoints* points, float default_value, blink_Position block_pos, size_t search_beg_index, size_t* left) -> float;
//	SearchFn binary;
//	SearchFn forward;
//	DefaultValue<float> default_value;
//};
//
//struct IntSearcher {
//	using SearchFn = auto (*)(const blink_IntPoints* points, int64_t default_value, blink_Position block_pos, size_t search_beg_index, size_t* left) -> int64_t;
//	SearchFn binary;
//	SearchFn forward;
//	DefaultValue<int64_t> default_value;
//};

namespace ent {
	using Plugin = EntityStore<
		blink_UUID
	>;
	using Param = EntityStore<
		blink_UUID,
		ParamCustomIndex
	>;
	using ParamCustom = EntityStore<
		ParamFlags,
		ParamIcon,
		ParamSettings,
		ParamTypeIndex,
		SubParams
	>;
	//using ParamCustomChord = EntityStore<
	//	ChordSearcher
	//>;
	using ParamCustomEnv = EntityStore<
		ApplyOffsetFn,
		ClampRange,
		EnvIndex,
		OffsetEnvIndex,
		OverrideEnvIndex
	>;
	using ParamCustomOption = EntityStore<
		DefaultValue<int64_t>,
		//IntSearcher,
		StringVec
	>;
	using ParamCustomSliderInt = EntityStore<
		blink_IntSliderIndex
	>;
	using ParamCustomSliderReal = EntityStore<
		ApplyOffsetFn,
		blink_RealSliderIndex,
		ClampRange,
		OffsetEnvIndex,
		OverrideEnvIndex
	>;
	using Env = EntityStore<
		DefaultValue<float>,
		EnvSnapSettings,
		MaxSliderIndex,
		MinSliderIndex,
		//RealSearcher,
		ValueSliderIndex
	>;
	using SliderInt = EntityStore<
		DefaultValue<int64_t>,
		blink_TweakerInt
		//tweak::Spec<int64_t>
		//IntSearcher
	>;
	using SliderReal = EntityStore<
		DefaultValue<float>,
		blink_TweakerReal
		//tweak::Spec<float>
		//RealSearcher
	>;
} // ent

struct Host {
	ent::Instance instance;
	ent::Unit unit;
	ent::Param param;
	ent::ParamCustom param_custom;
	ent::ParamCustomEnv param_custom_env;
	ent::ParamCustomOption param_custom_option;
	ent::ParamCustomSliderInt param_custom_slider_int;
	ent::ParamCustomSliderReal param_custom_slider_real;
	ent::Env env;
	ent::SliderInt slider_int;
	ent::SliderReal slider_real;
};

namespace read {

[[nodiscard]] inline
auto custom_idx(const Plugin& plugin, blink_ParamIndex param_idx) -> ParamCustomIndex {
	return plugin.param.get<ParamCustomIndex>(param_idx.value);
}

[[nodiscard]] inline
auto type_idx(const Plugin& plugin, size_t param_custom_idx) -> size_t {
	return plugin.param_custom.get<ParamTypeIndex>(param_custom_idx).value;
}

} // read

namespace write {
namespace slider {

inline
auto default_value(Plugin* plugin, blink_RealSliderIndex sld_idx, DefaultValue<float> value) -> void {
	plugin->slider_real.get<DefaultValue<float>>(sld_idx.value) = value;
}

//inline
//auto searcher(Plugin* plugin, blink_RealSliderIndex sld_idx, RealSearcher value) -> void {
//	plugin->slider_real.get<RealSearcher>(sld_idx.value) = value;
//}

inline
auto tweaker(Plugin* plugin, blink_RealSliderIndex sld_idx, tweak::Spec<float> value) -> void {
	plugin->slider_real.get<tweak::Spec<float>>(sld_idx.value) = value;
}

} // slider

namespace param {

inline
auto add_flags(Plugin* plugin, size_t param_custom_idx, int flags) -> void {
	plugin->param_custom.get<ParamFlags>(param_custom_idx).value.value |= flags;
}

inline
auto name(Plugin* plugin, size_t param_custom_idx, std::string_view name) -> void {
	plugin->param_custom.get<ParamSettings>(param_custom_idx).value.name = {name.data()};
}

inline
auto short_name(Plugin* plugin, size_t param_custom_idx, std::string_view name) -> void {
	plugin->param_custom.get<ParamSettings>(param_custom_idx).value.short_name = {name.data()};
}

namespace option {

inline
auto default_value(Plugin* plugin, size_t type_idx, DefaultValue<int64_t> value) -> void {
	plugin->param_custom_option.get<DefaultValue<int64_t>>(type_idx) = value;
}

//inline
//auto searcher(Plugin* plugin, size_t type_idx, IntSearcher searcher) -> void {
//	plugin->param_custom_option.get<IntSearcher>(type_idx) = std::move(searcher);
//}

inline
auto strings(Plugin* plugin, size_t type_idx, StringVec strings) -> void {
	plugin->param_custom_option.get<StringVec>(type_idx) = std::move(strings);
}

} // option

namespace slider_real {

inline
auto clamp_range(Plugin* plugin, size_t type_idx, ClampRange value) -> void {
	plugin->param_custom_slider_real.get<ClampRange>(type_idx) = value;
}

inline
auto offset_env(Plugin* plugin, size_t type_idx, OffsetEnvIndex value) -> void {
	plugin->param_custom_slider_real.get<OffsetEnvIndex>(type_idx) = value;
}

inline
auto override_env(Plugin* plugin, size_t type_idx, OverrideEnvIndex value) -> void {
	plugin->param_custom_slider_real.get<OverrideEnvIndex>(type_idx) = value;
}

inline
auto slider(Plugin* plugin, size_t type_idx, blink_RealSliderIndex value) -> void {
	plugin->param_custom_slider_real.get<blink_RealSliderIndex>(type_idx) = value;
}

} // slider_real

} // param;
} // write

[[nodiscard]] inline
auto get_tweaker(const Plugin& plugin, blink_IntSliderIndex sld_idx) -> const tweak::Spec<int64_t>& {
	return plugin.slider_int.get<tweak::Spec<int64_t>>(sld_idx.value);
}

[[nodiscard]] inline
auto from_string(const Plugin& plugin, blink_IntSliderIndex sld_idx, std::string_view str, int64_t* out) -> blink_Bool {
	if (const auto value = tweak::from_string(get_tweaker(plugin, sld_idx), str.data())) {
		*out = value.value();
		return {true};
	}
	return {false};
}

[[nodiscard]] inline
auto to_string(const Plugin& plugin, blink_IntSliderIndex sld_idx, int64_t value) -> blink_TempString {
	static auto str = tweak::to_string(get_tweaker(plugin, sld_idx), value);
	return {str.c_str()};
}

[[nodiscard]] inline 
auto constrain(const Plugin& plugin, blink_IntSliderIndex sld_idx, int64_t value) -> int64_t {
	return tweak::constrain(get_tweaker(plugin, sld_idx), value);
}

[[nodiscard]] inline 
auto decrement(const Plugin& plugin, blink_IntSliderIndex sld_idx, int64_t value, blink_Bool precise) -> int64_t {
	return tweak::decrement(get_tweaker(plugin, sld_idx), value, precise.value > 0);
}

[[nodiscard]] inline 
auto drag(const Plugin& plugin, blink_IntSliderIndex sld_idx, int64_t start_value, int64_t amount, blink_Bool precise) -> int64_t {
	return tweak::drag(get_tweaker(plugin, sld_idx), start_value, amount, precise.value > 0);
}

[[nodiscard]] inline 
auto get_default_value(const Plugin& plugin, blink_IntSliderIndex sld_idx) -> int64_t {
	return plugin.slider_int.get<DefaultValue<int64_t>>(sld_idx.value).value;
}

[[nodiscard]] inline 
auto increment(const Plugin& plugin, blink_IntSliderIndex sld_idx, int64_t value, blink_Bool precise) -> int64_t {
	return tweak::increment(get_tweaker(plugin, sld_idx), value, precise.value > 0);
}

//[[nodiscard]] inline
//auto make_default_chord_searcher() -> ChordSearcher {
//	ChordSearcher out;
//	out.binary  = std_params::search::chord_binary;
//	out.forward = std_params::search::chord_forward;
//	return out;
//}
//
//[[nodiscard]] inline
//auto make_default_int_searcher(DefaultValue<int64_t> default_value) -> IntSearcher {
//	IntSearcher out;
//	out.default_value = default_value;
//	out.binary        = std_params::search::step_binary;
//	out.forward       = std_params::search::step_forward;
//	return out;
//}
//
//[[nodiscard]] inline
//auto make_default_real_searcher(DefaultValue<float> default_value) -> RealSearcher {
//	RealSearcher out;
//	out.default_value = default_value;
//	out.binary        = std_params::search::float_points_binary;
//	out.forward       = std_params::search::float_points_forward;
//	return out;
//}

namespace add {

namespace env {

template <int MIN = 0, int MAX = 100> [[nodiscard]]
auto percentage(Plugin* plugin) -> blink_EnvelopeIndex {
	const auto idx = plugin->env.push_back();
	write::env::default_value(plugin, idx, {0.0f});
	write::env::searcher(plugin, idx, make_default_real_searcher({0.0f});
	write::env::set_max_slider(plugin, idx, add::slider::
		//DefaultValue<float>,
		//EnvSnapSettings,
		//MaxSliderIndex,
		//MinSliderIndex,
		//RealSearcher,
		//ValueSliderIndex,
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
auto percentage(Plugin* plugin) -> blink_RealSliderIndex {
	const auto idx = plugin->slider_real.push_back();
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
auto push_back(Plugin* plugin, blink_UUID uuid, bool custom) -> blink_ParamIndex {
	const auto param_idx = plugin->param.push_back();
	plugin->param.get<blink_UUID>(param_idx) = uuid;
	if (custom) {
		const auto custom_idx = plugin->param_custom.push_back();
		plugin->param.get<ParamCustomIndex>(param_idx).value = custom_idx;
	}
	return {param_idx};
}

namespace option {

inline
auto push_back(Plugin* plugin, blink_UUID uuid, bool custom) -> blink_ParamIndex {
	const auto param_idx = add::param::push_back(plugin, uuid, custom);
	if (custom) {
		const auto custom_idx       = read::custom_idx(*plugin, param_idx).value.value();
		const auto param_option_idx = plugin->param_custom_option.push_back();
		plugin->param_custom.get<ParamTypeIndex>(custom_idx).value = param_option_idx;
	}
	return param_idx;
}

inline
auto noise_mode(Plugin* plugin, bool custom) -> blink_ParamIndex {
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
auto reverse(Plugin* plugin, bool custom) -> blink_ParamIndex {
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
auto noise_width(Plugin* plugin, bool custom) -> blink_ParamIndex {
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
} // blink