#pragma once

// This is meant to be a C interface. I have only ever compiled it with C++ compilers though so
// it's probably not. If you want to help me fix the C-ness of it then pull requests are welcome.

#include <stddef.h>
#include <stdint.h>

#define BLINK_VECTOR_SIZE 64
#define BLINK_OK 0
#define BLINK_TRUE 1
#define BLINK_FALSE 0
#define BLINK_STRING_MAX 128

typedef int      blink_Error;
typedef double   blink_Position;
typedef uint32_t blink_Scale;
typedef float (*blink_ApplyOffsetFn)(float, float);

typedef struct {      int8_t value; } blink_Bool;
typedef struct {    uint64_t value; } blink_BufferID;
typedef struct {      size_t value; } blink_EnvIdx;
typedef struct {      size_t value; } blink_InstanceIdx;
typedef struct {      size_t value; } blink_SliderIntIdx;
typedef struct {      size_t value; } blink_ParamIdx;
typedef struct {      size_t value; } blink_PluginIdx;
typedef struct {      size_t value; } blink_SliderRealIdx;
typedef struct {      size_t value; } blink_UnitIdx;
typedef struct {     int32_t value; } blink_Flags;
typedef struct {     int32_t value; } blink_ID;
typedef struct {     uint8_t value; } blink_BitDepth;
typedef struct {     uint8_t value; } blink_ChannelCount;
typedef struct {    uint32_t value; } blink_ParamCount;
typedef struct {    uint32_t value; } blink_SR;
typedef struct {    uint64_t value; } blink_FrameCount;
typedef struct { const char* value; } blink_StaticString;
typedef struct { const char* value; } blink_TempString;
typedef struct { const char* value; } blink_UUID;

enum blink_ChannelMode {
	blink_ChannelMode_Left = 0,
	blink_ChannelMode_Right = 1,
	blink_ChannelMode_Stereo = 2,
	blink_ChannelMode_StereoSwap = 3,
};

enum blink_StdIcon {
	blink_StdIcon_None = 0,
	blink_StdIcon_Amp = 1,
	blink_StdIcon_Pan = 2,
	blink_StdIcon_Pitch = 3,
	blink_StdIcon_SampleOffset = 4,
	blink_StdIcon_Speed = 5,
	blink_StdIcon_Loop = 6,
	blink_StdIcon_Reverse = 7,
	blink_StdIcon_PianoRoll = 8,
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Standard error codes
// Plugins can define their own error codes which must be > 0
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
enum blink_StdError {
	blink_StdError_None = BLINK_OK,
	blink_StdError_AlreadyInitialized = -1,
	blink_StdError_NotInitialized = -2,
	blink_StdError_NotImplemented = -3,
	blink_StdError_InvalidInstance = -4,
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Range types
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct {
	float min;
	float max;
} blink_Range;

typedef struct {
	int min;
	int max;
} blink_IntRange;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Modulation point types
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct {
	blink_Position x;
	float y;
} blink_RealPoint;

typedef struct {
	blink_Position x;
	int64_t y;
} blink_IntPoint;

typedef struct {
	blink_Position x;
	blink_Scale y;
} blink_ChordBlock;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Modulation point array types
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct {
	size_t count;
	blink_RealPoint* data;
	float min;
	float max;
} blink_RealPoints;

typedef struct {
	size_t count;
	blink_IntPoint* data;
	int64_t min;
	int64_t max;
} blink_IntPoints;

typedef struct {
	size_t count;
	blink_ChordBlock* data;
} blink_ChordBlocks;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Param data
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct {
	blink_ChordBlocks points;
} blink_ChordData;

typedef struct {
	blink_RealPoints points;
	float default_value;
} blink_EnvData;

typedef struct {
	blink_IntPoints points;
	int64_t default_value;
} blink_OptionData;

typedef struct {
	blink_RealPoints points;
	float default_value;
} blink_SliderRealData;

typedef struct {
	blink_IntPoints points;
	int64_t default_value;
} blink_SliderIntData;

union blink_ParamData {
	blink_ChordData chord;
	blink_EnvData envelope;
	blink_OptionData option;
	blink_SliderIntData slider_int;
	blink_SliderRealData slider_real;
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Param Flags
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
enum blink_ParamFlags {
	blink_ParamFlags_None                       = 1 << 0,
	blink_ParamFlags_AlwaysShowWhenGroupVisible = 1 << 1,  // Will not appear in modulator tree
	blink_ParamFlags_CanManipulate              = 1 << 2,  // Has no effect for int sliders
	blink_ParamFlags_DefaultActive              = 1 << 3,
	blink_ParamFlags_DefaultAlwaysVisible       = 1 << 4,
	blink_ParamFlags_DefaultDisabled            = 1 << 5,
	blink_ParamFlags_Hidden                     = 1 << 6,
	blink_ParamFlags_HostClamp                  = 1 << 7,  // Host will clamp values to clamp_range, after applying manipulator offsets
	blink_ParamFlags_IconOnly                   = 1 << 8,  // Only show icon in button
	blink_ParamFlags_IsToggle                   = 1 << 9,  // Option will be treated as an ON/OFF toggle
	blink_ParamFlags_MovesDisplay               = 1 << 10, // Editing should trigger a visual update
	blink_ParamFlags_ShowButton                 = 1 << 11,
	blink_ParamFlags_ShowInContextMenu          = 1 << 12,
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Manipulator Settings
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct {
	// Either of these can be empty, but at least one must be defined
	// if blink_ParamFlags_CanManipulate is set
	blink_EnvIdx offset_envelope;
	blink_EnvIdx override_envelope; 
} blink_ManipSettings;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Envelope Snap Settings
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct {
	// Step size is also a range with a value that
	// can be configured by the user
	blink_SliderRealIdx step_size_slider;
	float default_snap_amount;
} blink_EnvSnapSettings;

typedef struct {
	size_t size;
	const char* data;
} blink_ResourceData;

typedef struct {
	// Unique identifier for this plugin
	blink_UUID uuid; 
	// The name of the plugin
	blink_StaticString name; 
	// The plugin category as a list of strings delimited by '|'
	// e.g. 
	//	"Destruction"
	//	"Filters|Butterworth"
	//	"Space|Reverbs"
	// 	"Developer Name|Phasers" 
	// May be null for no category
	blink_StaticString category; 
	// The plugin version string
	// Usual format is something like v0.0.0
	blink_StaticString version; 
	// If this is true, the host will try to find an icon resource at res/icon.png
	// If there isn't one, a default icon will be used
	blink_Bool has_icon;
	// True if Blockhead should enable warp markers. From the plugin's
	// perspective this just means warp data will be passed in to process().
	// It is up to the plugin to interpret this data.
	blink_Bool enable_warp_markers; 
} blink_PluginInfo;

typedef blink_Bool (*blink_GetGridLine)(int index, float* out);
typedef blink_Bool (*blink_GetStepLine)(int index, float step_size, float* out);
typedef float      (*blink_SnapValue)(float value, float step_size, float snap_amount);
typedef float      (*blink_Tweak_ConstrainReal)(float value);
typedef float      (*blink_Tweak_DecrementReal)(float value, bool precise);
typedef float      (*blink_Tweak_DragReal)(float value, int pixels, bool precise);
typedef blink_Bool (*blink_Tweak_FromStringReal)(const char* string, float* out);
typedef float      (*blink_Tweak_IncrementReal)(float value, bool precise);
typedef float      (*blink_Tweak_StepifyReal)(float value);
typedef int64_t    (*blink_Tweak_ConstrainInt)(int64_t value);
typedef int64_t    (*blink_Tweak_DecrementInt)(int64_t value, bool precise);
typedef int64_t    (*blink_Tweak_DragInt)(int64_t value, int pixels, bool precise);
typedef blink_Bool (*blink_Tweak_FromStringInt)(const char* string, int64_t* out);
typedef int64_t    (*blink_Tweak_IncrementInt)(int64_t value, bool precise);
typedef int64_t    (*blink_Tweak_StepifyInt)(int64_t value);
typedef void       (*blink_Tweak_ToStringInt)(int64_t value, char buffer[BLINK_STRING_MAX]);
typedef void       (*blink_Tweak_ToStringReal)(float value, char buffer[BLINK_STRING_MAX]);

typedef struct {
	blink_Tweak_ToStringReal to_string;
	blink_Tweak_FromStringReal from_string;
	blink_Tweak_ConstrainReal constrain;
	blink_Tweak_DragReal drag;
	blink_Tweak_IncrementReal increment;
	blink_Tweak_DecrementReal decrement;
	blink_Tweak_StepifyReal stepify;
} blink_TweakerReal;

typedef struct {
	blink_Tweak_ToStringInt to_string;
	blink_Tweak_FromStringInt from_string;
	blink_Tweak_ConstrainInt constrain;
	blink_Tweak_DragInt drag;
	blink_Tweak_IncrementInt increment;
	blink_Tweak_DecrementInt decrement;
	blink_Tweak_StepifyInt stepify;
} blink_TweakerInt;

typedef struct {
	blink_GetGridLine get_grid_line;
	blink_GetStepLine get_step_line;
	blink_SnapValue snap_value;
	blink_Tweak_StepifyReal stepify;
	blink_Tweak_ToStringReal to_string;
} blink_EnvFns;

typedef blink_EnvIdx        (*blink_host_add_env)(void*);
typedef blink_ParamIdx      (*blink_host_add_param_env)(void*, blink_UUID uuid);
typedef blink_ParamIdx      (*blink_host_add_param_option)(void*, blink_UUID uuid);
typedef blink_ParamIdx      (*blink_host_add_param_slider_int)(void*, blink_UUID uuid);
typedef blink_ParamIdx      (*blink_host_add_param_slider_real)(void*, blink_UUID uuid);
typedef blink_SliderIntIdx  (*blink_host_add_slider_int)(void*);
typedef blink_SliderRealIdx (*blink_host_add_slider_real)(void*);
typedef float               (*blink_host_read_param_env_default_value)(void*, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx);
typedef int64_t             (*blink_host_read_param_option_default_value)(void*, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx);
typedef float               (*blink_host_read_param_slider_real_default_value)(void*, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx);
typedef int64_t             (*blink_host_read_param_slider_int_default_value)(void*, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx);
typedef void                (*blink_host_write_env_default_value)(void*, blink_EnvIdx env_idx, float value);
typedef void                (*blink_host_write_env_fns)(void*, blink_EnvIdx env_idx, blink_EnvFns fns);
typedef void                (*blink_host_write_env_max_slider)(void*, blink_EnvIdx env_idx, blink_SliderRealIdx sld_idx);
typedef void                (*blink_host_write_env_min_slider)(void*, blink_EnvIdx env_idx, blink_SliderRealIdx sld_idx);
typedef void                (*blink_host_write_env_snap_settings)(void*, blink_EnvIdx env_idx, blink_EnvSnapSettings settings);
typedef void                (*blink_host_write_env_value_slider)(void*, blink_EnvIdx env_idx, blink_SliderRealIdx sld_idx);
typedef void                (*blink_host_write_param_add_flags)(void*, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, int flags);
typedef void                (*blink_host_write_param_add_subparam)(void*, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_ParamIdx subparam_idx);
typedef void                (*blink_host_write_param_env_clamp_range)(void*, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_Range range);
typedef void                (*blink_host_write_param_env_env_idx)(void*, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_EnvIdx env_idx);
typedef void                (*blink_host_write_param_env_offset_env)(void*, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_EnvIdx env_idx);
typedef void                (*blink_host_write_param_env_override_env)(void*, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_EnvIdx env_idx);
typedef void                (*blink_host_write_param_group)(void*, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_StaticString group_name);
typedef void                (*blink_host_write_param_icon)(void*, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_StdIcon icon);
typedef void                (*blink_host_write_param_long_desc)(void*, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_StaticString value);
typedef void                (*blink_host_write_param_manip_delegate)(void*, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_ParamIdx delegate_idx);
typedef void                (*blink_host_write_param_name)(void*, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_StaticString value);
typedef void                (*blink_host_write_param_short_name)(void*, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_StaticString value);
typedef void                (*blink_host_write_param_slider_real_offset_env)(void*, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_EnvIdx env_idx);
typedef void                (*blink_host_write_param_slider_real_override_env)(void*, blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_EnvIdx env_idx);
typedef void                (*blink_host_write_slider_int_default_value)(void*, blink_SliderIntIdx sld_idx, int64_t value);
typedef void                (*blink_host_write_slider_int_tweaker)(void*, blink_SliderIntIdx sld_idx, blink_TweakerInt tweaker);
typedef void                (*blink_host_write_slider_real_default_value)(void*, blink_SliderRealIdx sld_idx, float value);
typedef void                (*blink_host_write_slider_real_tweaker)(void*, blink_SliderRealIdx sld_idx, blink_TweakerReal tweaker);

struct blink_HostFns {
	void* usr;
	blink_host_add_env                              add_env;
	blink_host_add_param_env                        add_param_env;
	blink_host_add_param_option                     add_param_option;
	blink_host_add_param_slider_int                 add_param_slider_int;
	blink_host_add_param_slider_real                add_param_slider_real;
	blink_host_add_slider_int                       add_slider_int;
	blink_host_add_slider_real                      add_slider_real;
	blink_host_read_param_env_default_value         read_param_env_default_value;
	blink_host_read_param_option_default_value      read_param_option_default_value;
	blink_host_read_param_slider_int_default_value  read_param_slider_int_default_value;
	blink_host_read_param_slider_real_default_value read_param_slider_real_default_value;
	blink_host_write_env_default_value              write_env_default_value;
	blink_host_write_env_fns                        write_env_fns;
	blink_host_write_env_max_slider                 write_env_max_slider;
	blink_host_write_env_min_slider                 write_env_min_slider;
	blink_host_write_env_snap_settings              write_env_snap_settings;
	blink_host_write_env_value_slider               write_env_value_slider;
	blink_host_write_param_add_flags                write_param_add_flags;
	blink_host_write_param_add_subparam             write_param_add_subparam;
	blink_host_write_param_env_clamp_range          write_param_env_clamp_range;
	blink_host_write_param_env_env_idx              write_param_env_env_idx;
	blink_host_write_param_env_offset_env           write_param_env_offset_env;
	blink_host_write_param_env_override_env         write_param_env_override_env;
	blink_host_write_param_group                    write_param_group;
	blink_host_write_param_icon                     write_param_icon;
	blink_host_write_param_long_desc                write_param_long_desc;
	blink_host_write_param_manip_delegate           write_param_manip_delegate;
	blink_host_write_param_name                     write_param_name;
	blink_host_write_param_short_name               write_param_short_name;
	blink_host_write_param_slider_real_offset_env   write_param_slider_real_offset_env;
	blink_host_write_param_slider_real_override_env write_param_slider_real_override_env;
	blink_host_write_slider_int_default_value       write_slider_int_default_value;
	blink_host_write_slider_int_tweaker             write_slider_int_tweaker;
	blink_host_write_slider_real_default_value      write_slider_real_default_value;
	blink_host_write_slider_real_tweaker            write_slider_real_tweaker;
};

typedef bool (*blink_Preprocess_ShouldAbort)(void* host);
typedef void (*blink_Preprocess_ReportProgress)(void* host, float progress);

typedef struct {
	blink_Preprocess_ShouldAbort should_abort;
	blink_Preprocess_ReportProgress report_progress;
} blink_PreprocessCallbacks;

typedef blink_FrameCount (*blink_GetSampleDataCB)(void* host, blink_ChannelCount channel, blink_FrameCount index, blink_FrameCount size, float* buffer);

typedef struct {
	blink_ID id;
	blink_ChannelCount num_channels;
	blink_FrameCount num_frames; 
	// Array of 2 positions for the start and end of the loop.
	// Or null if no loop
	blink_FrameCount* loop_points; 
	blink_SR SR;
	blink_BitDepth bit_depth; 
	void* host;
	blink_GetSampleDataCB get_data; // TODO: move this into HostFns?
} blink_SampleInfo;

typedef struct {
	int64_t x, y;
	float value; // currently unused
} blink_WarpPoint;

typedef struct {
	size_t count;
	blink_WarpPoint* points;
} blink_WarpPoints;

// Draw Info
// This is passed in to blink_sampler_draw() as an output argument
// Blockhead may set any of the pointers to null depending on what data it needs from the plugin.
typedef struct {
	// The sample position after being transformed by parameter settings but before time warping
	// has been performed
	double* sculpted_sample_positions; 
	// The sample position after time warping has been performed
	// Blockhead will always pass null if blink_sampler_enable_warp_markers() returns false
	double* warped_sample_positions; 
	// The block position after being transformed by parameter settings but before time warping
	// has been performed.
	// This will differ from sculpted_sample_positions when the sample rate of the sample is not
	// equal to the song rate
	double* sculpted_block_positions; 
	// The block position after time warping has been performed
	// Blockhead will always pass null if blink_sampler_enable_warp_markers() returns false
	// This will differ from warped_sample_positions when the sample rate of the sample is not
	// equal to the song rate
	double* warped_block_positions; 
	// The sample position after time warping and other transformations (loop, reverse etc)
	double* final_sample_positions; 
	// The rate of change in the sample position after time warping
	float* waveform_derivatives; 
	// The amplitude after being transformed by parameter settings
	float* amp;
} blink_SamplerDrawInfo;

typedef struct {
	// True if the plugin needs to preprocess samples in some way.
	// Preprocessing happens once per sample.
	blink_Bool requires_preprocessing; 
	// True if the waveform resulting from the sample transformation
	// could be substantially different from the waveform generated
	// by blink_sampler_draw(). If this is true then Blockhead will
	// draw a second waveform for the baked block data.
	blink_Bool baked_waveform_could_be_different;
} blink_SamplerInfo;

typedef struct {
	// Blockhead uses these values to inform the user of any latency introduced
	// by the block due to buffering or whatever. Latency compensation is
	// dealt with manually by the end user (for now)
	// 
	// A value of -1 means "unknown"
	// If all values are -1, it means the block does not introduce any latency
	// 
	// Not all of these need to be filled. Blockhead will use:
	//  - exact_delay if it is set, otherwise
	//  - the (min/max)_delay pair if they are both set, otherwise
	//  - min_delay, otherwise
	//  - max_delay, otherwise
	// 	- approximate_delay
	// 
	int exact_delay;       // Exact latency (in frames) introduced by the block
	int min_delay;         // Minimum latency (in frames)
	int max_delay;         // Maximum latency (in frames)
	int approximate_delay; // Approximate latency (in frames)
} blink_EffectInstanceInfo;

typedef struct {
	blink_BufferID buffer_id;
	blink_SR song_rate;
	blink_Position* positions;
} blink_UnitBuffer;

struct blink_UnitState {
	uint64_t id; // Increments by at least one every time the unit state changes.
	             // The same unit state might be passed in for multiple audio buffers, i.e.
	             // this id might not change from one buffer to the next.
	             // Could be used to optimize things like envelope traversals, for example
	             // if the parameter state has not changed since the last audio buffer then
	             // a plugin could continue searching from the previously hit envelope
	             // point instead of searching from the beginning
	float scale;
	int64_t data_offset;
	blink_Bool smooth_transitions;
	const blink_WarpPoints* warp_points;
	// May be NULL, in which case plugins should act as if all parameters are default.
	const blink_ParamData* param_data;
};

typedef struct {
	blink_UnitBuffer unit;
} blink_EffectBuffer;

struct blink_EffectUnitState {
	blink_UnitState unit;
};

typedef struct {
	blink_UnitBuffer unit;
	const blink_SampleInfo* sample_info;
	blink_Bool analysis_ready;
} blink_SamplerBuffer;

struct blink_SamplerUnitState {
	blink_UnitState unit;
	blink_ChannelMode channel_mode;
};

struct blink_SynthBuffer {
	blink_UnitBuffer unit;
};

struct blink_SynthUnitState {
	blink_UnitState unit;
};

#ifdef BLINK_EXPORT

#ifdef _WIN32
# define EXPORTED  __declspec( dllexport )
#else
# define EXPORTED
#endif

extern "C"
{
	EXPORTED blink_TempString         blink_get_error_string(blink_Error error);
	EXPORTED blink_PluginInfo         blink_get_plugin_info();
	EXPORTED blink_ResourceData       blink_get_resource_data(const char* path); // Optional
	EXPORTED blink_Error              blink_init(blink_PluginIdx plugin_idx, blink_HostFns host);
	EXPORTED blink_Error              blink_instance_destroy(blink_InstanceIdx instance_idx);
	EXPORTED blink_InstanceIdx        blink_instance_make();
	EXPORTED blink_Error              blink_instance_reset(blink_InstanceIdx instance_idx);
	EXPORTED blink_Error              blink_instance_stream_init(blink_InstanceIdx instance_idx, blink_SR SR);
	EXPORTED blink_Error              blink_terminate();
	EXPORTED blink_UnitIdx            blink_unit_add(blink_InstanceIdx instance_idx);
	EXPORTED blink_Error              blink_unit_reset(blink_UnitIdx unit_idx);
	EXPORTED blink_Error              blink_unit_stream_init(blink_UnitIdx unit_idx, blink_SR SR);
	EXPORTED blink_SamplerInfo        blink_get_sampler_info();

	// EFFECT PLUGIN INTERFACE ----------------------------------------
	EXPORTED blink_Error              blink_effect_process(blink_UnitIdx unit_idx, const blink_EffectBuffer* buffer, const blink_EffectUnitState* unit_state, const float* in, float* out);
	EXPORTED blink_EffectInstanceInfo blink_effect_get_info(blink_InstanceIdx instance_idx);

	// SYNTH PLUGIN INTERFACE -----------------------------------------
	EXPORTED blink_Error              blink_synth_process(blink_UnitIdx unit_idx, const blink_SynthBuffer* buffer, const blink_SynthUnitState* unit_state, float* out);

	// SAMPLER PLUGIN INTERFACE ---------------------------------------

	// output pointer is aligned on a 16-byte boundary
	// output pointer is an array of size BLINK_VECTOR_SIZE * 2 for non-interleaved L and R channels 
	EXPORTED blink_Error blink_sampler_process(blink_UnitIdx unit_idx, const blink_SamplerBuffer* buffer, const blink_SamplerUnitState* unit_state, float* out);

	// Called by the host once per sample only if
	// blink_SamplerInfo::requires_preprocessing is true
	//
	// This function will be called in a separate thread from everything else.
	//
	// The plugin should periodically call the ShouldAbort callback and stop
	// preprocessing if it returns true.
	//
	// The plugin should periodically report the completion percentage by
	// calling the ReportProgress callback with a value between 0 and 1.
	//
	// The process() function may be called in the audio thread before
	// preprocessing has finished. Blockhead will pass in BLINK_FALSE for
	// blink_SampleInfo.analysis_ready until this function returns
	// BLINK_OK.
	//
	// If the plugin allocates any memory for the sample it should
	// be freed when the host calls blink_sampler_sample_deleted().
	EXPORTED blink_Error blink_sampler_preprocess_sample(void* host, blink_PreprocessCallbacks callbacks, const blink_SampleInfo* sample_info);

	// Called when a sample is deleted by Blockhead. The plugin should free
	// any data associated with the sample.
	//
	// Note that Blockhead may keep samples in memory after the user requests
	// their deletion so this function may not be called immediately.
	//
	// It is the host's responsibility to ensure that this function is not
	// called while process() is still executing.
	EXPORTED blink_Error blink_sampler_sample_deleted(blink_ID sample_id);

	// The host takes care of actually rendering the waveform but relies on
	// the plugin to calculate the waveform position at each pixel.
	EXPORTED blink_Error blink_sampler_draw(const blink_SamplerBuffer* buffer, const blink_SamplerUnitState* unit_state, blink_FrameCount n, blink_SamplerDrawInfo* out);
}

#endif
