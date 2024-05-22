#pragma once

#include "block_positions.hpp"
#include "standard_parameters/search.hpp"
#include <blink_effect.h>
#include <blink_sampler.h>
#include <blink_synth.h>
#include <tweak/tweak.hpp>
#include <vector>

namespace blink {

using ApplyOffsetFn = std::function<float(float, float)>;
struct BufferID         { blink_BufferID value = {0}; };
struct ClampRange       { blink_Range value = {0}; };
struct EnvIndex         { blink_EnvelopeIndex value = {0}; };
struct EnvSnapSettings  { blink_EnvSnapSettings value = {0, 1}; };
struct MaxSliderIndex   { size_t value = 0; };
struct MinSliderIndex   { size_t value = 0; };
struct OffsetEnvIndex   { blink_EnvelopeIndex value = {0}; };
struct OverrideEnvIndex { blink_EnvelopeIndex value = {0}; };
struct ParamCustomIndex { std::optional<size_t> value; };
struct ParamFlags       { blink_Flags value = {0}; };
struct ParamIcon        { blink_StdIcon value = {blink_StdIcon_None}; };
struct ParamSettings    { blink_ParamSettings value = {{0}, {0}, {0}, {0}, -1}; };
struct ParamTypeIndex   { size_t value = 0; };
struct SR               { blink_SR value = {0}; };
struct StringVec        { std::vector<std::string> value; };
struct SubParams        { std::vector<blink_ParamIndex> value; };
struct UnitVec          { std::vector<blink_UnitIndex> value; };
struct ValueSliderIndex { size_t value = 0; };
template <typename T> struct DefaultValue { T value = T(0); };

template <typename BinarySearchFn, typename ForwardSearchFn>
struct Searcher {
	BinarySearchFn binary;
	ForwardSearchFn forward;
};

template <typename SearchFn>
auto make_searcher(SearchFn binary, SearchFn forward) -> Searcher<SearchFn, SearchFn> {
	return {binary, forward};
}

template <typename BinarySearchFn, typename ForwardSearchFn>
auto make_searcher(BinarySearchFn binary, ForwardSearchFn forward) -> Searcher<BinarySearchFn, ForwardSearchFn> {
	return {binary, forward};
}

struct ChordSearcher {
	using SearchFn = auto (*)(const blink_ChordData* data, blink_Position block_pos, size_t search_beg_index, size_t* left) -> blink_Scale;
	SearchFn binary;
	SearchFn forward;
};

struct RealSearcher {
	using SearchFn = auto (*)(const blink_RealPoints* points, float default_value, blink_Position block_pos, size_t search_beg_index, size_t* left) -> float;
	SearchFn binary;
	SearchFn forward;
	DefaultValue<float> default_value;
};

struct IntSearcher {
	using SearchFn = auto (*)(const blink_IntPoints* points, int64_t default_value, blink_Position block_pos, size_t search_beg_index, size_t* left) -> int64_t;
	SearchFn binary;
	SearchFn forward;
	DefaultValue<int64_t> default_value;
};

template <typename... Ts>
struct EntityStore {
public:
	auto push_back() -> size_t {
		const auto index = size();
		(std::get<std::vector<T>>(data_).emplace_back(), ...);
		return index;
	}
	auto size() const -> size_t { return std::get<0>(data_).size(); }
	template <typename T> [[nodiscard]] auto get() -> std::vector<T>& { return std::get<std::vector<T>>(data_); }
	template <typename T> [[nodiscard]] auto get() const -> const std::vector<T>& { return std::get<std::vector<T>>(data_); }
	template <typename T> [[nodiscard]] auto get(size_t index) -> T& { return std::get<std::vector<T>>(data_)[index]; }
	template <typename T> [[nodiscard]] auto get(size_t index) const -> const T& { return std::get<std::vector<T>>(data_)[index]; }
private:
	using Tuple = std::tuple<std::vector<Ts>...>;
	Tuple data_;
};

struct InstanceProcess {
	BufferID buffer_id;
	int active_buffer_units = 0;
};

namespace ent {
	using Instance = EntityStore<
		SR,
		UnitVec,
		InstanceProcess
	>;
	using Unit = EntityStore<
		blink_InstanceIndex,
		BlockPositions,
		BufferID
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
	using ParamCustomChord = EntityStore<
		ChordSearcher
	>;
	using ParamCustomEnv = EntityStore<
		ApplyOffsetFn,
		ClampRange,
		EnvIndex,
		OffsetEnvIndex,
		OverrideEnvIndex
	>;
	using ParamCustomOption = EntityStore<
		DefaultValue<int64_t>,
		IntSearcher,
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
		RealSearcher,
		ValueSliderIndex
	>;
	using SliderInt = EntityStore<
		DefaultValue<int64_t>,
		tweak::Spec<int64_t>,
		IntSearcher
	>;
	using SliderReal = EntityStore<
		DefaultValue<float>,
		tweak::Spec<float>,
		RealSearcher
	>;
} // ent

struct Plugin {
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

inline
auto searcher(Plugin* plugin, blink_RealSliderIndex sld_idx, RealSearcher value) -> void {
	plugin->slider_real.get<RealSearcher>(sld_idx.value) = value;
}

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

inline
auto searcher(Plugin* plugin, size_t type_idx, IntSearcher searcher) -> void {
	plugin->param_custom_option.get<IntSearcher>(type_idx) = std::move(searcher);
}

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

template <typename T, typename U, typename BinarySearchFn>
auto search(const U& data, blink_Position block_position, const BinarySearchFn& binary_search_fn) -> T {
	size_t left; 
	return binary_search_fn(data, block_position, 0, &left);
} 

template <typename T, typename U, typename Searcher>
auto search_vec(const U& data, const BlockPositions& block_positions, int n, Searcher searcher, T* out) -> void {
	size_t left = 0;
	bool reset = false;
	auto prev_pos = block_positions.prev_pos; 
	for (int i = 0; i < n; i++) {
		const auto pos = block_positions.positions[i]; 
		if (pos < prev_pos) {
			// This occurs when Blockhead loops back to an earlier song position.
			// We perform a binary search to get back on track
			reset = true;
		} 
		if (reset) {
			reset = false; 
			out[i] = searcher.binary(data, block_positions.positions[i], 0, &left);
		}
		else {
			out[i] = searcher.forward(data, block_positions.positions[i], left, &left);
		} 
		prev_pos = pos;
	}
} 

template <typename T, typename U, typename Searcher>
auto search_vec(const U& data, const BlockPositions& block_positions, Searcher searcher, T* out) -> void {
	search_vec(data, block_positions, block_positions.count, searcher, out);
} 

auto search_vec(ChordSearcher searcher, const blink_ChordData& data, const BlockPositions& block_positions) -> ml::DSPVectorInt {
	ml::DSPVectorInt out;
	std::array<blink_Scale, kFloatsPerDSPVector> buffer; 
	search_vec(data, block_positions, make_searcher(searcher.binary, searcher.forward), buffer.data()); 
	for (int i = 0; i < kFloatsPerDSPVector; i++) {
		out[i] = buffer[i];
	} 
	return out;
} 

auto search_vec(RealSearcher searcher, const blink_RealPoints& data, const BlockPositions& block_positions) -> ml::DSPVector {
	ml::DSPVector out; 
	auto binary_search = [searcher](const blink_RealPoints& data, blink_Position position, size_t search_beg_index, size_t* left) -> float {
		return searcher.binary(&data, searcher.default_value.value, position, search_beg_index, left);
	};
	auto forward_search = [searcher](const blink_RealPoints& data, blink_Position position, size_t search_beg_index, size_t* left) -> float {
		return searcher.forward(&data, searcher.default_value.value, position, search_beg_index, left);
	};
	search_vec(data, block_positions, make_searcher(binary_search, forward_search), out.getBuffer());
	return out;
} 

auto search_vec(IntSearcher searcher, const blink_IntPoints& data, const BlockPositions& block_positions) -> ml::DSPVectorInt {
	ml::DSPVectorInt out; 
	auto binary_search = [searcher](const blink_IntPoints& data, blink_Position position, size_t search_beg_index, size_t* left) -> float {
		return searcher.binary(&data, searcher.default_value.value, position, search_beg_index, left);
	};
	auto forward_search = [searcher](const blink_IntPoints& data, blink_Position position, size_t search_beg_index, size_t* left) -> float {
		return searcher.forward(&data, searcher.default_value.value, position, search_beg_index, left);
	};
	std::array<int64_t, kFloatsPerDSPVector> buffer; 
	search_vec(data, block_positions, make_searcher(binary_search, forward_search), buffer.data());
	for (int i = 0; i < kFloatsPerDSPVector; i++) {
		out[i] = static_cast<int>(buffer[i]);
	} 
	return out;
} 

template <typename InstanceResetFn>
auto begin_process(blink::Plugin* plugin, blink_InstanceIndex instance_idx, uint64_t buffer_id, InstanceResetFn&& instance_reset_fn) -> void {
	// instance is reset at the start of the buffer if we have gone
	// at least one buffer with no units being processed
	auto& instance_process = plugin->instance.process[instance_idx.value];
	if (buffer_id > instance_process.buffer_id) {
		if (buffer_id > instance_process.buffer_id + 1 || instance_process.active_buffer_units == 0) {
			instance_reset_fn();
		}
		instance_process.buffer_id           = buffer_id;
		instance_process.active_buffer_units = 0;
	}
	instance_process.active_buffer_units++;
}

template <typename UnitProcessFn, typename UnitResetFn, typename InstanceResetFn> [[nodiscard]]
auto sampler_process(blink::Plugin* plugin, blink_UnitIndex unit_idx, const blink_SamplerBuffer& buffer, const blink_SamplerUnitState* unit_state, float* out, UnitProcessFn&& process_fn, UnitResetFn&& unit_reset_fn, InstanceResetFn&& instance_reset_fn) -> blink_Error {
	const auto instance_idx = entities->unit.instance[unit_idx.value];
	blink::begin_process(plugin, instance_idx, buffer.buffer_id, instance_reset_fn);
	if (buffer.buffer_id > entities->unit.buffer_id[unit_idx.value] + 1) {
		// unit is reset at the start of the buffer if we have gone
		// at least one buffer without processing this unit
		unit_reset_fn();
	}
	entities->unit.buffer_id[unit_idx.value] = buffer.buffer_id;
	entities->unit.block_positions[unit_idx.value](buffer.positions, kFloatsPerDSPVector);
	return process_fn(buffer, unit_state, out);
}

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

[[nodiscard]] inline
auto get_num_params(const Plugin& plugin) -> size_t {
	return plugin.param.size();
}

[[nodiscard]] inline
auto get_param_by_uuid(const Plugin& plugin, blink_UUID uuid, blink_ParamIndex* out) -> blink_Bool {
	const auto& uuids = plugin.param.get<blink_UUID>();
	const auto pos = std::find(uuids.begin(), uuids.end(), uuid);
	if (pos == uuids.end()) {
		return {false};
	}
	const auto index = std::distance(uuids.begin(), pos);
	*out = {static_cast<size_t>(index)};
	return {true};
}

[[nodiscard]] inline
auto get_param_uuid(const Plugin& plugin, blink_ParamIndex index) -> blink_UUID {
	return plugin.param.get<blink_UUID>(index.value);
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

[[nodiscard]] inline
auto make_default_chord_searcher() -> ChordSearcher {
	ChordSearcher out;
	out.binary  = std_params::search::chord_binary;
	out.forward = std_params::search::chord_forward;
	return out;
}

[[nodiscard]] inline
auto make_default_int_searcher(DefaultValue<int64_t> default_value) -> IntSearcher {
	IntSearcher out;
	out.default_value = default_value;
	out.binary        = std_params::search::step_binary;
	out.forward       = std_params::search::step_forward;
	return out;
}

[[nodiscard]] inline
auto make_default_real_searcher(DefaultValue<float> default_value) -> RealSearcher {
	RealSearcher out;
	out.default_value = default_value;
	out.binary        = std_params::search::float_points_binary;
	out.forward       = std_params::search::float_points_forward;
	return out;
}

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
