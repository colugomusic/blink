#pragma once

#include "blink.h"
#include "block_positions.hpp"
#include <functional>
#include <optional>
#include <vector>

[[nodiscard]] inline auto operator<(const blink_PluginIdx& a, const blink_PluginIdx& b) { return a.value < b.value; }

namespace blink {

enum class StdChord {
	SCALE,
};

enum class StdEnv {
	AMP,
	DELAY_TIME,
	DRY,
	FEEDBACK,
	FILTER_FREQUENCY,
	FILTER_RESONANCE,
	FORMANT,
	MIX,
	NOISE_AMOUNT,
	NOISE_COLOR,
	NOISE_WIDTH,
	PAN,
	PITCH,
	SPEED,
	WET,
};

enum class StdOption {
	LOOP,
	NOISE_MODE,
	REVERSE_MODE,
	REVERSE_TOGGLE,
};

enum class StdSliderInt {
	SAMPLE_OFFSET,
};

enum class StdSliderReal {
	AMP,
	DELAY_TIME,
	DRY,
	FEEDBACK,
	FILTER_FREQUENCY,
	FILTER_RESONANCE,
	FORMANT,
	MIX,
	NOISE_AMOUNT,
	NOISE_COLOR,
	NOISE_WIDTH,
	PAN,
	PITCH,
	SPEED,
	WET,
};

enum class ParamType { chord, env, option, slider_int, slider_real };
enum class PluginType { effect, sampler, synth };

struct GroupInfo {
	blink_StaticString name;
	std::vector<blink_ParamIdx> params;
};

struct ApplyOffsetFn      { blink_ApplyOffsetFn fn = nullptr; };
struct StepifyFn          { blink_Tweak_StepifyReal fn = nullptr; };
struct BufferID           { blink_BufferID value = {0}; };
struct ClampRange         { std::optional<blink_Range> value; };
struct DefaultSnapAmount  { float value = 0.0f; };
struct EnvFlags           { int value = 0; };
struct EnvFns             { blink_EnvFns value = {0}; };
struct EnvIdx             { blink_EnvIdx value = {0}; };
struct LocalInstanceIdx   { blink_InstanceIdx value = {0}; };
struct LocalUnitIdx       { blink_UnitIdx value = {0}; };
struct MaxSliderIdx       { std::optional<blink_SliderRealIdx> value; };
struct MinSliderIdx       { std::optional<blink_SliderRealIdx> value; };
struct OffsetEnvIdx       { EnvIdx value; };
struct ParamEnvIdx        { size_t value = 0; };
struct ParamGlobalIdx     { size_t value = 0; };
struct ParamLocalIdx      { size_t value = 0; };
struct ParamOptionIdx     { size_t value = 0; };
struct ParamSliderIntIdx  { size_t value = 0; };
struct ParamSliderRealIdx { size_t value = 0; };
struct PluginParams       { std::vector<ParamGlobalIdx> global_indices; };
struct ManipDelegate      { std::optional<ParamGlobalIdx> value; };
struct OverrideEnvIdx     { EnvIdx value; };
struct ParamFlags         { blink_Flags value = {0}; };
struct ParamIcon          { blink_StdIcon value = {blink_StdIcon_None}; };
struct ParamTypeIdx       { size_t value = 0; };
struct PluginIdx          { blink_PluginIdx value = {0}; };
struct PluginTypeIdx      { size_t value = 0; };
struct SamplerInfo        { blink_SamplerInfo value = {0}; };
struct SR                 { blink_SR value = {0}; };
struct StepSizeSliderIdx  { std::optional<blink_SliderRealIdx> value; };
struct StringVec          { std::vector<std::string> value; };
struct SubParams          { std::vector<ParamGlobalIdx> value; };
struct TweakerInt         { blink_TweakerInt value {0}; };
struct TweakerReal        { blink_TweakerReal value {0}; };
struct UnitVec            { std::vector<blink_UnitIdx> value; };
struct ValueSliderIdx     { blink_SliderRealIdx value; };
template <typename T> struct DefaultMax { T value = T(0); };
template <typename T> struct DefaultMin { T value = T(0); };
template <typename T> struct DefaultValue { T value = T(0); };

struct ParamStrings {
	blink_StaticString group      = {0};
	blink_StaticString name       = {0};
	blink_StaticString short_name = {0};
	blink_StaticString long_desc  = {0};
};
struct InstanceProcess {
	LocalInstanceIdx local_idx;
	BufferID buffer_id;
	int active_buffer_units = 0;
};

struct UnitProcess {
	PluginIdx plugin_idx;
	LocalUnitIdx local_idx;
	blink_InstanceIdx instance_idx;
	BufferID buffer_id;
};

struct PluginInterface {
	using get_error_string_fn = std::function<blink_TempString(blink_Error error)>;
	using get_plugin_info_fn = std::function<blink_PluginInfo()>;
	using get_resource_data_fn = std::function<blink_ResourceData(const char* path)>;
	using init_fn = std::function<blink_Error(blink_PluginIdx plugin_idx, blink_HostFns host)>;
	using instance_destroy_fn = std::function<blink_Error(blink_InstanceIdx instance_idx)>;
	using instance_make_fn = std::function<blink_InstanceIdx()>;
	using instance_reset_fn = std::function<blink_Error(blink_InstanceIdx instance_idx)>;
	using instance_stream_init_fn = std::function<blink_Error(blink_InstanceIdx instance_idx, blink_SR SR)>;
	using terminate_fn = std::function<blink_Error()>;
	using unit_add_fn = std::function<blink_UnitIdx(blink_InstanceIdx instance_idx)>;
	using unit_reset_fn = std::function<blink_Error(blink_UnitIdx unit_idx)>;
	using unit_stream_init_fn = std::function<blink_Error(blink_UnitIdx unit_idx, blink_SR SR)>;
	get_error_string_fn     get_error_string;
	get_plugin_info_fn      get_plugin_info;
	get_resource_data_fn    get_resource_data;
	init_fn                 init;
	instance_destroy_fn     instance_destroy;
	instance_make_fn        instance_make;
	instance_reset_fn       instance_reset;
	instance_stream_init_fn instance_stream_init;
	terminate_fn            terminate;
	unit_add_fn             unit_add;
	unit_reset_fn           unit_reset;
	unit_stream_init_fn     unit_stream_init;
	struct Effect {
		using process_fn = std::function<blink_Error(blink_UnitIdx unit_idx, const blink_EffectBuffer* buffer, const blink_EffectUnitState* unit_state, const float* in, float* out)>;
		using get_info_fn = std::function<blink_EffectInstanceInfo(blink_InstanceIdx instance_idx)>;
		get_info_fn get_info;
		process_fn process;
	} effect;
	struct Sampler {
		using process_fn = std::function<blink_Error(blink_UnitIdx unit_idx, const blink_SamplerBuffer* buffer, const blink_SamplerUnitState* unit_state, float* out)>;
		using preprocess_sample_fn = std::function<blink_Error(void* host, blink_PreprocessCallbacks callbacks, const blink_SampleInfo* sample_info)>;
		using sample_deleted_fn = std::function<blink_Error(blink_ID sample_id)>;
		using draw_fn = std::function<blink_Error(const blink_SamplerBuffer* buffer, const blink_SamplerUnitState* unit_state, blink_FrameCount n, blink_SamplerDrawInfo* out)>;
		using get_sonic_fragment_at_block_position_fn = std::function<double(blink_Position block_position)>;
		using block_position_for_sonic_fragment_fn = std::function<blink_Position(double fragment)>;
		block_position_for_sonic_fragment_fn    block_position_for_sonic_fragment;
		draw_fn                                 draw;
		get_sonic_fragment_at_block_position_fn get_sonic_fragment_at_block_position;
		preprocess_sample_fn                    preprocess_sample;
		process_fn                              process;
		sample_deleted_fn                       sample_deleted;
	} sampler;
	struct Synth {
		using process_fn = std::function<blink_Error(blink_UnitIdx unit_idx, const blink_SynthBuffer* buffer, const blink_SynthUnitState* unit_state, float* out)>;
		process_fn process;
	} synth;
};

[[nodiscard]] inline auto operator==(const ParamGlobalIdx& a, const ParamGlobalIdx& b) -> bool { return a.value == b.value; }
[[nodiscard]] inline auto operator==(const ParamEnvIdx& a, const ParamEnvIdx& b) -> bool { return a.value == b.value; }
[[nodiscard]] inline auto operator==(const ParamOptionIdx& a, const ParamOptionIdx& b) -> bool { return a.value == b.value; }
[[nodiscard]] inline auto operator==(const ParamSliderIntIdx& a, const ParamSliderIntIdx& b) -> bool { return a.value == b.value; }
[[nodiscard]] inline auto operator==(const ParamSliderRealIdx& a, const ParamSliderRealIdx& b) -> bool { return a.value == b.value; }

} // blink

namespace std { template <> struct hash<blink_EnvIdx> { auto operator()(const blink_EnvIdx& idx) const -> size_t { return std::hash<size_t>{}(idx.value); } }; }
namespace std { template <> struct hash<blink_ID> { auto operator()(const blink_ID& id) const -> size_t { return std::hash<int64_t>{}(id.value); } }; }
namespace std { template <> struct hash<blink_SliderIntIdx> { auto operator()(const blink_SliderIntIdx& idx) const -> size_t { return std::hash<size_t>{}(idx.value); } }; }
namespace std { template <> struct hash<blink_SliderRealIdx> { auto operator()(const blink_SliderRealIdx& idx) const -> size_t { return std::hash<size_t>{}(idx.value); } }; }
namespace std { template <> struct hash<blink_ParamIdx> { auto operator()(const blink_ParamIdx& idx) const -> size_t { return std::hash<size_t>{}(idx.value); } }; }
namespace std { template <> struct hash<blink::ParamGlobalIdx> { auto operator()(const blink::ParamGlobalIdx& idx) const -> size_t { return std::hash<size_t>{}(idx.value); } }; }
namespace std { template <> struct hash<blink::ParamEnvIdx> { auto operator()(const blink::ParamEnvIdx& idx) const -> size_t { return std::hash<size_t>{}(idx.value); } }; }
namespace std { template <> struct hash<blink::ParamOptionIdx> { auto operator()(const blink::ParamOptionIdx& idx) const -> size_t { return std::hash<size_t>{}(idx.value); } }; }
namespace std { template <> struct hash<blink::ParamSliderIntIdx> { auto operator()(const blink::ParamSliderIntIdx& idx) const -> size_t { return std::hash<size_t>{}(idx.value); } }; }
namespace std { template <> struct hash<blink::ParamSliderRealIdx> { auto operator()(const blink::ParamSliderRealIdx& idx) const -> size_t { return std::hash<size_t>{}(idx.value); } }; }
[[nodiscard]] inline auto operator==(const blink_EnvIdx& a, const blink_EnvIdx& b) -> bool { return a.value == b.value; }
[[nodiscard]] inline auto operator==(const blink_ID& a, const blink_ID& b) -> bool { return a.value == b.value; }
[[nodiscard]] inline auto operator==(const blink_SliderIntIdx& a, const blink_SliderIntIdx& b) -> bool { return a.value == b.value; }
[[nodiscard]] inline auto operator==(const blink_SliderRealIdx& a, const blink_SliderRealIdx& b) -> bool { return a.value == b.value; }
[[nodiscard]] inline auto operator==(const blink_ParamIdx& a, const blink_ParamIdx& b) -> bool { return a.value == b.value; }
[[nodiscard]] inline auto operator!=(const blink_ParamIdx& a, const blink_ParamIdx& b) -> bool { return a.value != b.value; }
[[nodiscard]] inline auto operator<(const blink_ID& a, const blink_ID& b) -> bool { return a.value < b.value; }
[[nodiscard]] inline auto operator<(const blink_ParamIdx& a, const blink_ParamIdx& b) -> bool { return a.value < b.value; }
inline auto operator++(blink_ChannelCount& a, int) -> blink_ChannelCount { a.value += 1; return a; }
