#pragma once

#include "blink.h"
#include "blink_sampler.h"
#include "block_positions.hpp"
#include <functional>
#include <optional>
#include <vector>

namespace blink {

enum class PluginType { effect, sampler, synth };

struct ApplyOffsetFn    { blink_ApplyOffsetFn fn = nullptr; };
struct StepifyFn        { blink_Tweak_StepifyReal fn = nullptr; };
struct BufferID         { blink_BufferID value = {0}; };
struct ClampRange       { blink_Range value = {0}; };
struct EnvFns           { blink_EnvFns value = {0}; };
struct EnvIdx           { blink_EnvIdx value = {0}; };
struct EnvSnapSettings  { blink_EnvSnapSettings value = {0, 1}; };
struct LocalInstanceIdx { blink_InstanceIdx value = {0}; };
struct LocalUnitIdx     { blink_UnitIdx value = {0}; };
struct MaxSliderIdx     { int value = -1; };
struct MinSliderIdx     { int value = -1; };
struct OffsetEnvIdx     { blink_EnvIdx value = {0}; };
struct OverrideEnvIdx   { blink_EnvIdx value = {0}; };
struct ParamFlags       { blink_Flags value = {0}; };
struct ParamIcon        { blink_StdIcon value = {blink_StdIcon_None}; };
struct ParamStrings     { blink_ParamStrings value = {0}; };
struct ParamTypeIdx     { size_t value = 0; };
struct SR               { blink_SR value = {0}; };
struct StringVec        { std::vector<std::string> value; };
struct SubParams        { std::vector<blink_ParamIdx> value; };
struct TweakerInt       { blink_TweakerInt value {0}; };
struct TweakerReal      { blink_TweakerReal value {0}; };
struct UnitVec          { std::vector<blink_UnitIdx> value; };
struct ValueSliderIdx   { int value = -1; };
template <typename T> struct DefaultValue { T value = T(0); };

struct InstanceProcess {
	BufferID buffer_id;
	int active_buffer_units = 0;
};

struct UnitProcess {
	blink_InstanceIdx instance_idx;
	BlockPositions block_positions;
	BufferID buffer_id;
};

struct PluginInterface {
	using get_error_string_fn = std::function<blink_TempString(blink_Error error)>;
	using get_plugin_info_fn = std::function<blink_PluginInfo()>;
	using get_resource_data_fn = std::function<blink_ResourceData(const char* path)>;
	using init_fn = std::function<blink_Error(blink_PluginIdx plugin_idx, blink_HostFns host)>;
	using instance_destroy_fn = std::function<blink_Error(blink_InstanceIdx instance_idx)>;
	using instance_make_fn = std::function<blink_InstanceIdx()>;
	using instance_reset_fn = std::function<blink_Error(blink_InstanceIdx instance_idx, blink_SR SR)>;
	using terminate_fn = std::function<blink_Error()>;
	using unit_add_fn = std::function<blink_UnitIdx(blink_InstanceIdx instance_idx)>;
	using unit_reset_fn = std::function<blink_Error(blink_InstanceIdx instance_idx, blink_SR SR)>;
	get_error_string_fn  blink_get_error_string;
	get_plugin_info_fn   blink_get_plugin_info;
	get_resource_data_fn blink_get_resource_data;
	init_fn              blink_init;
	instance_destroy_fn  blink_instance_destroy;
	instance_make_fn     blink_instance_make;
	instance_reset_fn    blink_instance_reset;
	terminate_fn         blink_terminate;
	unit_add_fn          blink_unit_add;
	unit_reset_fn        blink_unit_reset;
	struct Sampler {
		using get_sampler_info_fn = std::function<blink_SamplerInfo()>;
		using sampler_process_fn = std::function<blink_Error(blink_UnitIdx unit_idx, const blink_SamplerBuffer* buffer, const blink_SamplerUnitState* unit_state, float* out)>;
		using sampler_preprocess_sample_fn = std::function<blink_Error(void* host, blink_PreprocessCallbacks callbacks, const blink_SampleInfo* sample_info)>;
		using sampler_sample_deleted_fn = std::function<blink_Error(blink_ID sample_id)>;
		using sampler_draw_fn = std::function<blink_Error(const blink_SamplerBuffer* buffer, const blink_SamplerUnitState* unit_state, blink_FrameCount n, blink_SamplerDrawInfo* out)>;
		using get_sonic_fragment_at_block_position_fn = std::function<double(blink_Position block_position)>;
		using block_position_for_sonic_fragment_fn = std::function<blink_Position(double fragment)>;
		get_sampler_info_fn                     blink_get_sampler_info;
		sampler_process_fn                      blink_sampler_process;
		sampler_preprocess_sample_fn            blink_sampler_preprocess_sample;
		sampler_sample_deleted_fn               blink_sampler_sample_deleted;
		sampler_draw_fn                         blink_sampler_draw;
		get_sonic_fragment_at_block_position_fn blink_get_sonic_fragment_at_block_position;
		block_position_for_sonic_fragment_fn    blink_block_position_for_sonic_fragment;
	} sampler;
};

} // blink
