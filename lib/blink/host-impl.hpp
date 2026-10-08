#pragma once

#include <blink.h>
#include <blink_std.h>
#include <cassert>
#include <ent.hpp>
#include "common-impl.hpp"
#include "math.hpp"
#include "tweak.hpp"
#include "types.hpp"
#include <cs_lr_guarded.h>
#include <unordered_map>
#include <vector>

namespace lg = libguarded;

namespace blink {

struct IsAlive { bool value = false; };

using PluginTable = ent::simple_table<
	"blink:host:plugin-table",
	blink_PluginInfo,
	PluginType,
	PluginTypeIdx,
	PluginInterface,
	PluginParams,
	PluginFRs,
	std::vector<GroupInfo>
>;

using PluginSamplerTable = ent::simple_table<
	"blink:host:plugin-sampler-table",
	SamplerInfo
>;

using InstanceTable = ent::table<
	"blink:host:instance-table",
	1000,
	IsAlive,
	blink_PluginIdx,
	InstanceProcess,
	UnitVec
>;

using UnitTable = ent::table<
	"blink:host:unit-table",
	1000,
	IsAlive,
	blink_PluginIdx,
	UnitProcess
>;

using ParamTable = ent::simple_table<
	"blink:host:param-table",
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

using ParamEnvTable = ent::simple_table<
	"blink:host:param-env-table",
	ApplyOffsetFn,
	ClampRange,
	EnvIdx,
	OffsetEnvIdx,
	OverrideEnvIdx
>;

using ParamOptionTable = ent::simple_table<
	"blink:host:param-option-table",
	DefaultValue<int64_t>,
	StringVec
>;

using ParamSliderIntTable = ent::simple_table<
	"blink:host:param-slider-int-table",
	blink_SliderIntIdx
>;

using ParamSliderRealTable = ent::simple_table<
	"blink:host:param-slider-real-table",
	ApplyOffsetFn,
	blink_SliderRealIdx,
	ClampRange,
	OffsetEnvIdx,
	OverrideEnvIdx
>;

using EnvTable = ent::simple_table<
	"blink:host:env-table",
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

using SliderIntTable = ent::simple_table<
	"blink:host:slider-int-table",
	DefaultValue<int64_t>,
	TweakerInt
>;

using SliderRealTable = ent::simple_table<
	"blink:host:slider-real-table",
	DefaultValue<float>,
	TweakerReal
>;

using FrequencyResponseTable = ent::simple_table<
	"blink:host:frequency-response-table",
	blink_PluginIdx,
	FrBandCount,
	FrExtraCount,
	FrEnabledParams,
	FrFrequencyParams,
	FrMagnitudeParams,
	FrMbRHzParams,
	FrMbRVtParams,
	FrMbLHzCtrlParams,
	FrMbLVtCtrlParams,
	FrMbRHzCtrlParams,
	FrMbRVtCtrlParams,
	FrExtraParams
>;

struct SampleInfo {
	std::vector<blink_PluginIdx> registered_plugins;
	std::vector<blink_PluginIdx> completed_analysis;
};

using SampleInfoMap = std::unordered_map<blink_ID, SampleInfo>;

struct SampleAnalysis {
	lg::lr_guarded<SampleInfoMap> sample_info;
};

struct Host {
	PluginTable plugin;
	PluginSamplerTable plugin_sampler;
	InstanceTable instance;
	UnitTable unit;
	ParamTable param;
	ParamEnvTable param_env;
	ParamOptionTable param_option;
	ParamSliderIntTable param_slider_int;
	ParamSliderRealTable param_slider_real;
	EnvTable env;
	SliderIntTable slider_int;
	SliderRealTable slider_real;
	FrequencyResponseTable frequency_response;
	std::optional<blink_PluginIdx> default_sampler;
	SampleAnalysis sample_analysis;
	blink_HostFns fns;
};

// Just for convenience. You could define this in your application somewhere and have it
// auto declared anywhere that this header is included.
[[nodiscard]] auto host() -> Host&;

} // blink

namespace blink::read {

auto apply_offset_fn(const Host& host, ParamEnvIdx param_env_idx) -> ApplyOffsetFn;
auto apply_offset_fn(const Host& host, ParamSliderRealIdx param_sld_idx) -> ApplyOffsetFn;
auto clamp_range(const Host& host, ParamEnvIdx param_env_idx) -> std::optional<blink_Range>;
auto clamp_range(const Host& host, ParamSliderRealIdx param_sld_idx) -> std::optional<blink_Range>;
auto default_max(const Host& host, blink_EnvIdx env_idx) -> float;
auto default_min(const Host& host, blink_EnvIdx env_idx) -> float;
auto default_snap_amount(const Host& host, blink_EnvIdx env_idx) -> float;
auto default_value(const Host& host, blink_EnvIdx env_idx) -> float;
auto default_value(const Host& host, blink_SliderIntIdx sld_idx) -> int64_t;
auto default_value(const Host& host, blink_SliderRealIdx sld_idx) -> float;
auto default_value(const Host& host, ParamOptionIdx option_idx) -> int64_t;
auto env(const Host& host, ParamEnvIdx param_env_idx) -> blink_EnvIdx;
auto env_offset(const Host& host, ParamEnvIdx param_env_idx) -> blink_EnvIdx;
auto env_offset(const Host& host, ParamSliderRealIdx param_sld_idx) -> blink_EnvIdx;
auto env_override(const Host& host, ParamEnvIdx param_env_idx) -> blink_EnvIdx;
auto env_override(const Host& host, ParamSliderRealIdx param_sld_idx) -> blink_EnvIdx;
auto find_param(const Host& host, blink_PluginIdx plugin, std::string_view uuid_str) -> std::optional<blink_ParamIdx>;
auto find_plugin(const Host& host, std::string_view uuid_str) -> std::optional<blink_PluginIdx>;
auto flags(const Host& host, blink_EnvIdx env_idx) -> int;
auto flags(const Host& host, blink_ParamIdx param_idx) -> int;
auto fns(const Host& host, blink_EnvIdx env_idx) -> blink_EnvFns;
auto global_to_local(const Host& host, ParamGlobalIdx global_idx) -> ParamLocalIdx;
auto group_name(const Host& host, blink_ParamIdx param_idx) -> std::string_view;
auto icon(const Host& host, blink_ParamIdx param_idx) -> blink_StdIcon;
auto iface(const Host& host, blink_PluginIdx plugin_idx) -> const PluginInterface&;
auto info(const Host& host, blink_PluginIdx plugin_idx) -> const blink_PluginInfo&;
auto local_to_global(const Host& host, blink_PluginIdx plugin_idx, blink_ParamIdx local_idx) -> ParamGlobalIdx;
auto long_desc(const Host& host, blink_ParamIdx param_idx) -> std::string_view;
auto manip_delegate(const Host& host, blink_ParamIdx param_idx) -> std::optional<ParamGlobalIdx>;
auto max_slider(const Host& host, blink_EnvIdx env_idx) -> std::optional<blink_SliderRealIdx>;
auto min_slider(const Host& host, blink_EnvIdx env_idx) -> std::optional<blink_SliderRealIdx>;
auto name(const Host& host, blink_ParamIdx param_idx) -> std::string_view;
auto name(const Host& host, blink_PluginIdx plugin_idx) -> std::string_view;
auto plugin(const Host& host, blink_ParamIdx param_idx) -> blink_PluginIdx;
auto plugin_count(const Host& host) -> size_t;
auto process(Host* host, blink_InstanceIdx instance_idx) -> InstanceProcess&;
auto type(const Host& host, blink_PluginIdx plugin_idx) -> PluginType;
auto sampler_baked_waveform_could_be_different(const Host& host, blink_PluginIdx plugin_idx) -> bool;
auto sampler_requires_sample_analysis(const Host& host, blink_PluginIdx plugin_idx) -> bool;
auto short_name(const Host& host, blink_ParamIdx param_idx) -> std::string_view;
auto slider(const Host& host, ParamSliderIntIdx param_env_idx) -> blink_SliderIntIdx;
auto slider(const Host& host, ParamSliderRealIdx param_env_idx) -> blink_SliderRealIdx;
auto step_size_slider(const Host& host, blink_EnvIdx env_idx) -> std::optional<blink_SliderRealIdx>;
auto strings(const Host& host, ParamOptionIdx option_idx) -> const std::vector<std::string>&;
auto subparams(const Host& host, ParamGlobalIdx param_idx) -> const std::vector<ParamGlobalIdx>&;
auto tweaker(const Host& host, blink_SliderIntIdx sld_idx) -> const blink_TweakerInt&;
auto tweaker(const Host& host, blink_SliderRealIdx sld_idx) -> const blink_TweakerReal&;
auto type(const Host& host, ParamGlobalIdx param_idx) -> ParamType;
auto type_idx(const Host& host, ParamGlobalIdx param_idx) -> size_t;
auto uuid(const Host& host, ParamGlobalIdx param_idx) -> blink_UUID;
auto value_slider(const Host& host, blink_EnvIdx env_idx) -> blink_SliderRealIdx;
auto version(const Host& host, blink_PluginIdx plugin_idx) -> std::string_view;

} // blink::read

namespace blink::write {

auto add_flags(Host* host, blink_EnvIdx env_idx, int flags) -> void;
auto add_flags(Host* host, ParamGlobalIdx param_idx, int flags) -> void;
auto add_string(Host* host, ParamOptionIdx option_idx, blink_TempString string) -> void;
auto add_subparam(Host* host, ParamGlobalIdx param_idx, ParamGlobalIdx subparam_idx) -> void;
auto apply_offset_fn(Host* host, ParamEnvIdx param_env_idx, blink_ApplyOffsetFn fn) -> void;
auto clamp_range(Host* host, ParamEnvIdx param_env_idx, ClampRange value) -> void;
auto clamp_range(Host* host, ParamSliderRealIdx param_sld_idx, ClampRange value) -> void;
auto default_max(Host* host, blink_EnvIdx env_idx, DefaultMax<float> value) -> void;
auto default_min(Host* host, blink_EnvIdx env_idx, DefaultMin<float> value) -> void;
auto default_snap_amount(Host* host, blink_EnvIdx env_idx, DefaultSnapAmount value) -> void;
auto default_value(Host* host, blink_EnvIdx env_idx, DefaultValue<float> value) -> void;
auto default_value(Host* host, ParamOptionIdx option_idx, DefaultValue<int64_t> value) -> void;
auto default_value(Host* host, blink_SliderIntIdx sld_idx, DefaultValue<int64_t> value) -> void;
auto default_value(Host* host, blink_SliderRealIdx sld_idx, DefaultValue<float> value) -> void;
auto env(Host* host, ParamEnvIdx param_env_idx, blink_EnvIdx value) -> void;
auto fns(Host* host, blink_EnvIdx env_idx, EnvFns fns) -> void;
auto group(Host* host, ParamGlobalIdx param_idx, blink_StaticString group) -> void;
auto icon(Host* host, ParamGlobalIdx param_idx, blink_StdIcon icon) -> void;
auto info(Host* host, blink_PluginIdx plugin_idx, blink_PluginInfo info) -> void;
auto long_desc(Host* host, ParamGlobalIdx param_idx, blink_StaticString desc) -> void;
auto manip_delegate(Host* host, ParamGlobalIdx param_idx, ParamGlobalIdx delegate) -> void;
auto max_slider(Host* host, blink_EnvIdx env_idx, MaxSliderIdx value) -> void;
auto min_slider(Host* host, blink_EnvIdx env_idx, MinSliderIdx value) -> void;
auto offset_env(Host* host, ParamEnvIdx env_idx, OffsetEnvIdx value) -> void;
auto offset_env(Host* host, ParamSliderRealIdx sld_idx, OffsetEnvIdx value) -> void;
auto override_env(Host* host, ParamEnvIdx env_idx, OverrideEnvIdx value) -> void;
auto override_env(Host* host, ParamSliderRealIdx sld_idx, OverrideEnvIdx value) -> void;
auto plugin_interface(Host* host, blink_PluginIdx plugin_idx, PluginInterface iface) -> void;
auto name(Host* host, ParamGlobalIdx param_idx, blink_StaticString name) -> void;
auto sampler_info(Host* host, blink_PluginIdx plugin_idx, blink_SamplerInfo info) -> void;
auto short_name(Host* host, ParamGlobalIdx param_idx, blink_StaticString name) -> void;
auto slider(Host* host, ParamSliderIntIdx sld_idx, blink_SliderIntIdx value) -> void;
auto slider(Host* host, ParamSliderRealIdx sld_idx, blink_SliderRealIdx value) -> void;
auto step_size_slider(Host* host, blink_EnvIdx env_idx, StepSizeSliderIdx value) -> void;
auto strings(Host* host, ParamOptionIdx option_idx, StringVec strings) -> void;
auto tweaker(Host* host, blink_SliderIntIdx sld_idx, TweakerInt value) -> void;
auto tweaker(Host* host, blink_SliderRealIdx sld_idx, TweakerReal value) -> void;
auto type_idx(Host* host, ParamGlobalIdx param_idx, ParamEnvIdx type_idx) -> void;
auto type_idx(Host* host, ParamGlobalIdx param_idx, ParamOptionIdx type_idx) -> void;
auto type_idx(Host* host, ParamGlobalIdx param_idx, ParamSliderIntIdx type_idx) -> void;
auto type_idx(Host* host, ParamGlobalIdx param_idx, ParamSliderRealIdx type_idx) -> void;
auto uuid(Host* host, ParamGlobalIdx param_idx, blink_UUID uuid) -> void;
auto value_slider(Host* host, blink_EnvIdx env_idx, ValueSliderIdx value) -> void;

} // blink::write

namespace blink::add {

auto frequency_response(Host* host, blink_PluginIdx plugin_idx, const blink_FrequencyResponseInfo* info) -> blink_FrequencyResponseIdx;
auto plugin(Host* host, PluginType type) -> blink_PluginIdx;

} // blink::add

namespace blink::add::slider {

auto empty_int(Host* host) -> blink_SliderIntIdx;
auto empty_real(Host* host) -> blink_SliderRealIdx;
auto amp(Host* host) -> blink_SliderRealIdx;
auto delay_time(Host* host) -> blink_SliderRealIdx;
auto filter_frequency(Host* host) -> blink_SliderRealIdx;
auto pan(Host* host) -> blink_SliderRealIdx;
auto pitch(Host* host) -> blink_SliderRealIdx;
auto sample_offset(Host* host) -> blink_SliderIntIdx;
auto speed(Host* host) -> blink_SliderRealIdx;

} // blink::add::slider

namespace blink::add::env {

auto empty(Host* host) -> blink_EnvIdx;
auto amp(Host* host) -> blink_EnvIdx;
auto delay_time(Host* host) -> blink_EnvIdx;
auto feedback(Host* host) -> blink_EnvIdx;
auto filter_frequency(Host* host) -> blink_EnvIdx;
auto formant(Host* host) -> blink_EnvIdx;
auto pan(Host* host) -> blink_EnvIdx;
auto pitch(Host* host) -> blink_EnvIdx;
auto speed(Host* host) -> blink_EnvIdx;

} // blink::add::env

namespace blink::add::param {

struct NewParam {
	blink_ParamIdx local_idx;
	ParamGlobalIdx global_idx;
};

auto default_active(ParamType type) -> bool;
auto empty(Host* host, blink_PluginIdx plugin_idx, ParamType type) -> NewParam;

} // blink::add::param

namespace blink::add::param::chord {

auto scale(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto std(Host* host, blink_PluginIdx plugin_idx, StdChord std_chord) -> blink_ParamIdx;
auto custom(Host* host, blink_PluginIdx plugin_idx, blink_UUID uuid) -> blink_ParamIdx;

} // blink::add::param::chord

namespace blink::add::param::env {

auto empty(Host* host) -> ParamEnvIdx;
auto mix(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto amp(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto delay_time(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto dry(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto feedback(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto filter_frequency(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto filter_resonance(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto formant(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto noise_amount(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto noise_color(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto noise_width(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto pan(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto pitch(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto speed(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto wet(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto std(Host* host, blink_PluginIdx plugin_idx, StdEnv std_env) -> blink_ParamIdx;
auto custom(Host* host, blink_PluginIdx plugin_idx, blink_UUID uuid) -> blink_ParamIdx;

} // blink::add::param::param::env

namespace blink::add::param::option {

auto empty(Host* host) -> ParamOptionIdx;
auto loop(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto noise_mode(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto reverse_mode(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto reverse_toggle(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto std(Host* host, blink_PluginIdx plugin_idx, StdOption std_option) -> blink_ParamIdx;
auto custom(Host* host, blink_PluginIdx plugin_idx, blink_UUID uuid) -> blink_ParamIdx;

} // blink::add::param::option

namespace blink::add::param::slider_int {

auto empty(Host* host) -> ParamSliderIntIdx;
auto sample_offset(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto std(Host* host, blink_PluginIdx plugin_idx, StdSliderInt std_sld) -> blink_ParamIdx;
auto custom(Host* host, blink_PluginIdx plugin_idx, blink_UUID uuid) -> blink_ParamIdx;

} // blink::add::param::slider_int

namespace blink::add::param::slider_real {

auto empty(Host* host) -> ParamSliderRealIdx;
auto mix(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto amp(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto delay_time(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto dry(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto feedback(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto filter_frequency(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto filter_resonance(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto formant(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto noise_amount(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto noise_color(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto noise_width(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto pan(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto pitch(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto speed(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto wet(Host* host, blink_PluginIdx plugin_idx) -> blink_ParamIdx;
auto std(Host* host, blink_PluginIdx plugin_idx, StdSliderReal std_sld) -> blink_ParamIdx;
auto custom(Host* host, blink_PluginIdx plugin_idx, blink_UUID uuid) -> blink_ParamIdx;

} // blink::add::param::slider_real

namespace blink {

auto begin_unit_process(Host* host, const PluginInterface& plugin, blink_InstanceIdx instance_idx, blink_VectorID vector_id) -> void;
auto effect_process(Host* host, blink_UnitIdx unit_idx, const blink_VaryingData& varying, const blink_UniformData& uniform, const float* in, float* out) -> blink_Error;
auto frequency_response(const Host& host, blink_PluginIdx plugin_idx, const blink_UniformData& uniform, blink_FrequencyResponseIdx fr_idx, blink_FrameCount n, const float* in_x_01, float* out_y_01) -> blink_Error;
auto sampler_draw(const Host& host, blink_PluginIdx plugin_idx, const blink_SamplerVaryingData& varying, const blink_SamplerUniformData& uniform, blink_FrameCount n, blink_SamplerDrawInfo* out) -> blink_Error;
auto sampler_process(Host* host, blink_UnitIdx unit_idx, const blink_SamplerVaryingData& varying, const blink_SamplerUniformData& uniform, float* out) -> blink_Error;
auto synth_process(Host* host, blink_UnitIdx unit_idx, const blink_VaryingData& varying, const blink_UniformData& uniform, float* out) -> blink_Error;
auto terminate(const Host& host, blink_PluginIdx plugin_idx) -> blink_Error;
[[nodiscard]] auto get_plugins_of_type(const Host& host, PluginType type) -> std::vector<blink_PluginIdx>;
[[nodiscard]] auto get_effect_plugins(const Host& host) -> std::vector<blink_PluginIdx>;
[[nodiscard]] auto get_sampler_plugins(const Host& host) -> std::vector<blink_PluginIdx>;
[[nodiscard]] auto get_synth_plugins(const Host& host) -> std::vector<blink_PluginIdx>;
[[nodiscard]] auto get_std_chord(blink_UUID uuid) -> std::optional<StdChord>;
[[nodiscard]] auto get_std_env(blink_UUID uuid) -> std::optional<StdEnv>;
[[nodiscard]] auto get_std_option(blink_UUID uuid) -> std::optional<StdOption>;
[[nodiscard]] auto get_std_slider_int(blink_UUID uuid) -> std::optional<StdSliderInt>;
[[nodiscard]] auto get_std_slider_real(blink_UUID uuid) -> std::optional<StdSliderReal>;
[[nodiscard]] auto add_unit(Host* host, blink_PluginIdx plugin_idx, blink_InstanceIdx instance_idx, blink_SR SR) -> blink_UnitIdx;
[[nodiscard]] auto destroy_instance(Host* host, blink_PluginIdx plugin_idx, blink_InstanceIdx instance_idx) -> void;
[[nodiscard]] auto get_effect_info(const Host& host, blink_PluginIdx plugin_idx, blink_InstanceIdx instance_idx) -> blink_EffectInstanceInfo ;
[[nodiscard]] auto make_instance(Host* host, blink_PluginIdx plugin_idx, blink_SR SR) -> blink_InstanceIdx;
[[nodiscard]] auto sampler_sample_deleted(const Host& host, blink_PluginIdx plugin_idx, blink_ID sample_id) -> void;
[[nodiscard]] auto stream_init(const Host& host, blink_SR SR) -> void;
[[nodiscard]] auto host_ptr(void* user) -> Host*;

auto init(Host* host) -> void;
auto is_sample_analysis_ready(const Host& host, blink_ID sample_id, blink_PluginIdx plugin_idx) -> bool;
auto has_registered_plugin(const Host& host, blink_ID sample_id, blink_PluginIdx plugin_idx) -> bool;
auto get_registered_plugins(const Host& host, blink_ID sample_id) -> const std::vector<blink_PluginIdx>;

auto register_sample_plugin(Host* host, blink_ID sample_id, blink_PluginIdx plugin_idx) -> void;
auto sample_deleted(Host* host, blink_ID sample_id) -> void;

template <typename ProcessFn>
auto unit_process(Host* host, blink_UnitIdx unit_idx, const blink_VaryingData& varying, ProcessFn&& process_fn) -> blink_Error {
	auto& process      = host->unit.get<UnitProcess>(unit_idx.value);
	const auto& plugin = read::iface(*host, process.plugin_idx.value);
	begin_unit_process(host, plugin, process.instance_idx, varying.vector_id);
	if (varying.vector_id.value > process.vector_id.value.value + 1) {
		// unit is reset at the start of the buffer if we have gone
		// at least one buffer without processing this unit
		plugin.unit_reset(process.local_idx.value);
	}
	process.vector_id.value = varying.vector_id;
	return process_fn(plugin, process.local_idx);
}

} // blink

namespace blink::sample_analysis_thread {

auto sampler_analyze(Host* host, blink_PluginIdx plugin_idx, void* usr, blink_AnalysisCallbacks callbacks, const blink_SampleInfo& info) -> blink_AnalysisResult;

} // blink::sample_analysis_thread
