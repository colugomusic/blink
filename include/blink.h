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
typedef struct {     int64_t value; } blink_BufferID;
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
} blink_EnvData;

typedef struct {
	blink_IntPoints points;
} blink_OptionData;

typedef struct {
	blink_RealPoints points;
} blink_SliderRealData;

typedef struct {
	blink_IntPoints points;
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
	blink_ParamFlags_SonicPivot                 = 1 << 13,
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
	// Null if the parameter does not belong to a group
	blink_StaticString group_name; 
	// Full parameter name e.g. "Noise Amount"
	blink_StaticString name; 
	// Short parameter name to use in the context of a group, e.g. "Amount". Can be null
	blink_StaticString short_name; 
	// Long description of the parameter. Can be null
	blink_StaticString long_desc; 
} blink_ParamStrings;

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

typedef blink_EnvIdx        (*blink_host_add_env)();
typedef blink_ParamIdx      (*blink_host_add_param_env)(blink_UUID uuid);
typedef blink_ParamIdx      (*blink_host_add_param_option)(blink_UUID uuid);
typedef blink_ParamIdx      (*blink_host_add_param_slider_int)(blink_UUID uuid);
typedef blink_ParamIdx      (*blink_host_add_param_slider_real)(blink_UUID uuid);
typedef blink_SliderIntIdx  (*blink_host_add_slider_int)();
typedef blink_SliderRealIdx (*blink_host_add_slider_real)();
typedef float               (*blink_host_read_env_default_value)(blink_EnvIdx env_idx);
typedef blink_EnvIdx        (*blink_host_read_param_env_env_idx)(blink_ParamIdx param_idx);
typedef int64_t             (*blink_host_read_param_option_default_value)(blink_ParamIdx param_idx);
typedef blink_SliderIntIdx  (*blink_host_read_param_slider_int_slider_idx)(blink_ParamIdx param_idx);
typedef blink_SliderRealIdx (*blink_host_read_param_slider_real_slider_idx)(blink_ParamIdx param_idx);
typedef float               (*blink_host_read_slider_real_default_value)(blink_SliderRealIdx sld_idx);
typedef int64_t             (*blink_host_read_slider_int_default_value)(blink_SliderIntIdx sld_idx);
typedef blink_Error         (*blink_host_write_env_default_value)(blink_EnvIdx env_idx, float value);
typedef blink_Error         (*blink_host_write_env_fns)(blink_EnvIdx env_idx, blink_EnvFns fns);
typedef blink_Error         (*blink_host_write_env_max_slider)(blink_EnvIdx env_idx, blink_SliderRealIdx sld_idx);
typedef blink_Error         (*blink_host_write_env_min_slider)(blink_EnvIdx env_idx, blink_SliderRealIdx sld_idx);
typedef blink_Error         (*blink_host_write_env_snap_settings)(blink_EnvIdx env_idx, blink_EnvSnapSettings settings);
typedef blink_Error         (*blink_host_write_env_value_slider)(blink_EnvIdx env_idx, blink_SliderRealIdx sld_idx);
typedef blink_Error         (*blink_host_write_param_add_flags)(blink_ParamIdx param_idx, blink_ParamFlags flags);
typedef blink_Error         (*blink_host_write_param_add_subparam)(blink_ParamIdx param_idx, blink_ParamIdx subparam_idx);
typedef blink_Error         (*blink_host_write_param_delegate)(blink_ParamIdx param_idx, blink_ParamIdx delegate_idx);
typedef blink_Error         (*blink_host_write_param_group)(blink_ParamIdx param_idx, blink_StaticString group_name);
typedef blink_Error         (*blink_host_write_param_icon)(blink_ParamIdx param_idx, blink_StdIcon icon);
typedef blink_Error         (*blink_host_write_param_strings)(blink_ParamIdx param_idx, blink_ParamStrings strings);
typedef blink_Error         (*blink_host_write_slider_int_default_value)(blink_SliderIntIdx sld_idx, int64_t value);
typedef blink_Error         (*blink_host_write_slider_int_tweaker)(blink_SliderIntIdx sld_idx, blink_TweakerInt tweaker);
typedef blink_Error         (*blink_host_write_slider_real_default_value)(blink_SliderRealIdx sld_idx, float value);
typedef blink_Error         (*blink_host_write_slider_real_tweaker)(blink_SliderRealIdx sld_idx, blink_TweakerReal tweaker);

struct blink_HostFns {
	blink_host_add_env                           add_env;
	blink_host_add_param_env                     add_param_env;
	blink_host_add_param_option                  add_param_option;
	blink_host_add_param_slider_int              add_param_slider_int;
	blink_host_add_param_slider_real             add_param_slider_real;
	blink_host_add_slider_int                    add_slider_int;
	blink_host_add_slider_real                   add_slider_real;
	blink_host_read_env_default_value            read_env_default_value;
	blink_host_read_param_env_env_idx            read_param_env_env_idx;
	blink_host_read_param_option_default_value   read_param_option_default_value;
	blink_host_read_param_slider_int_slider_idx  read_param_slider_int_slider_idx;
	blink_host_read_param_slider_real_slider_idx read_param_slider_real_slider_idx;
	blink_host_read_slider_int_default_value     read_slider_int_default_value;
	blink_host_read_slider_real_default_value    read_slider_real_default_value;
	blink_host_write_env_default_value           write_env_default_value;
	blink_host_write_env_fns                     write_env_fns;
	blink_host_write_env_max_slider              write_env_max_slider;
	blink_host_write_env_min_slider              write_env_min_slider;
	blink_host_write_env_snap_settings           write_env_snap_settings;
	blink_host_write_env_value_slider            write_env_value_slider;
	blink_host_write_param_add_flags             write_param_add_flags;
	blink_host_write_param_add_subparam          write_param_add_subparam;
	blink_host_write_param_delegate	             write_param_delegate;
	blink_host_write_param_group                 write_param_group;
	blink_host_write_param_icon                  write_param_icon;
	blink_host_write_param_strings               write_param_strings;
	blink_host_write_slider_int_tweaker          write_slider_int_tweaker;
	blink_host_write_slider_int_default_value    write_slider_int_default_value;
	blink_host_write_slider_real_tweaker         write_slider_real_tweaker;
	blink_host_write_slider_real_default_value   write_slider_real_default_value;
};

#ifdef BLINK_EXPORT

#ifdef _WIN32
# define EXPORTED  __declspec( dllexport )
#else
# define EXPORTED
#endif

extern "C"
{
	EXPORTED blink_TempString   blink_get_error_string(blink_Error error);
	EXPORTED blink_PluginInfo   blink_get_plugin_info();
	EXPORTED blink_ResourceData blink_get_resource_data(const char* path); // Optional
	EXPORTED blink_Error        blink_init(blink_PluginIdx plugin_idx, blink_HostFns host);
	EXPORTED blink_Error        blink_instance_destroy(blink_InstanceIdx instance_idx);
	EXPORTED blink_InstanceIdx  blink_instance_make();
	EXPORTED blink_Error        blink_instance_reset(blink_InstanceIdx instance_idx, blink_SR SR);
	EXPORTED blink_Error        blink_terminate();
	EXPORTED blink_UnitIdx      blink_unit_add(blink_InstanceIdx instance_idx);
	EXPORTED blink_Error        blink_unit_reset(blink_InstanceIdx instance_idx, blink_SR SR);
}

#endif
