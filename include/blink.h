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

#define BLINK_STD_UUID_AMP "273e7c30-404b-4db6-ba97-20f33d49fe51"
#define BLINK_STD_UUID_DELAY_TIME "064315cd-affd-43a7-b7e8-4e586890375e"
#define BLINK_STD_UUID_DRY "15893220-b29d-4bd5-ba21-a9d1480a1432"
#define BLINK_STD_UUID_SCALE "860166f7-e839-448b-bd3b-a5bccbfe3ac1"
#define BLINK_STD_UUID_FEEDBACK "5c1ca6de-7ffd-417c-9541-2272ff1305b0"
#define BLINK_STD_UUID_FILTER_FREQUENCY "91181212-7072-41a9-9d11-3a265301a9a3"
#define BLINK_STD_UUID_FILTER_RESONANCE "4436fc1c-ae51-4580-b1fa-24b9c41425e3"
#define BLINK_STD_UUID_FORMANT "7b72dbef-e36d-4dce-958b-b0fa498ae41e"
#define BLINK_STD_UUID_LOOP "dfa36d24-3c41-4a13-9b57-dc0116ef19f7"
#define BLINK_STD_UUID_MIX "6441d97c-37c9-4670-9049-d22fac68b023"
#define BLINK_STD_UUID_NOISE_AMOUNT "29d5ecb5-cb5d-4f19-afd3-835dd805682a"
#define BLINK_STD_UUID_NOISE_COLOR "30100123-7343-4386-9ed2-f913b9e1e571"
#define BLINK_STD_UUID_NOISE_MODE "e426cc55-306d-4561-99bc-003bb7707a93"
#define BLINK_STD_UUID_NOISE_WIDTH "84e18fd3-03f1-49c2-a713-12e7e24dc03f"
#define BLINK_STD_UUID_PAN "9c312a2c-a1b4-4a8d-ab68-07ea157c4574"
#define BLINK_STD_UUID_PITCH "ca2529db-e7bd-4019-9a07-22aee24526d1"
#define BLINK_STD_UUID_REVERSE_MODE "af00fb24-07ed-4b10-bc79-5b405334cc17"
#define BLINK_STD_UUID_REVERSE_TOGGLE "e7cacaf8-4afc-4e81-83de-50620fed4b13"
#define BLINK_STD_UUID_SAMPLE_OFFSET "88373752-7656-4d0e-8da2-a18c05af0106"
#define BLINK_STD_UUID_SPEED "04293c38-3a64-42b2-80f0-43a4f8190ba7"
#define BLINK_STD_UUID_WET "953c5871-fe00-4b51-a93f-b5142fd9de81"

#define BLINK_STD_CATEGORY_DELAY "Delay"
#define BLINK_STD_CATEGORY_DESTRUCTION "Destruction"
#define BLINK_STD_CATEGORY_DYNAMICS "Dynamics"
#define BLINK_STD_CATEGORY_FILTERS "Filters"
#define BLINK_STD_CATEGORY_SPACE "Space"
#define BLINK_STD_CATEGORY_TONAL "Tonal"
#define BLINK_STD_CATEGORY_UTILITY "Utility"

typedef int      blink_Error;
typedef double   blink_Position;
typedef uint32_t blink_Scale;

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
	// Normally -1.
	// If this is >= 0, when the user tries to create a manipulator for
	// this parameter, the host will create a manipulator for the parameter
	// at this specified index instead
	int manipulation_delegate; 
} blink_ParamSettings;

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

typedef size_t  (*blink_Tweak_ToStringReal)(float value, char buffer[BLINK_STRING_MAX]);
typedef float   (*blink_Tweak_FromStringReal)(const char* string);
typedef float   (*blink_Tweak_ConstrainReal)(float value);
typedef float   (*blink_Tweak_DragReal)(float value, int pixels, bool precise);
typedef float   (*blink_Tweak_IncrementReal)(float value, bool precise);
typedef float   (*blink_Tweak_DecrementReal)(float value, bool precise);
typedef float   (*blink_Tweak_StepifyReal)(float value);
typedef size_t  (*blink_Tweak_ToStringInt)(int64_t value, char buffer[BLINK_STRING_MAX]);
typedef int64_t (*blink_Tweak_FromStringInt)(const char* string);
typedef int64_t (*blink_Tweak_ConstrainInt)(int64_t value);
typedef int64_t (*blink_Tweak_DragInt)(int64_t value, int pixels, bool precise);
typedef int64_t (*blink_Tweak_IncrementInt)(int64_t value, bool precise);
typedef int64_t (*blink_Tweak_DecrementInt)(int64_t value, bool precise);
typedef int64_t (*blink_Tweak_StepifyInt)(int64_t value);

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

typedef blink_ParamIdx      (*blink_host_add_param_env)(blink_PluginIdx plugin_idx, blink_UUID uuid);
typedef blink_ParamIdx      (*blink_host_add_param_option)(blink_PluginIdx plugin_idx, blink_UUID uuid);
typedef blink_ParamIdx      (*blink_host_add_param_slider_int)(blink_PluginIdx plugin_idx, blink_UUID uuid);
typedef blink_ParamIdx      (*blink_host_add_param_slider_real)(blink_PluginIdx plugin_idx, blink_UUID uuid);
typedef float               (*blink_host_read_env_default_value)(blink_PluginIdx plugin_idx, blink_EnvIdx env_idx);
typedef blink_EnvIdx        (*blink_host_read_param_env_env_idx)(blink_PluginIdx plugin_idx, blink_ParamIdx param_idx);
typedef int64_t             (*blink_host_read_param_option_default_value)(blink_PluginIdx plugin_idx, blink_ParamIdx param_idx);
typedef blink_SliderIntIdx  (*blink_host_read_param_slider_int_slider_idx)(blink_PluginIdx plugin_idx, blink_ParamIdx param_idx);
typedef blink_SliderRealIdx (*blink_host_read_param_slider_real_slider_idx)(blink_PluginIdx plugin_idx, blink_ParamIdx param_idx);
typedef float               (*blink_host_read_slider_real_default_value)(blink_PluginIdx plugin_idx, blink_SliderRealIdx sld_idx);
typedef int64_t             (*blink_host_read_slider_int_default_value)(blink_PluginIdx plugin_idx, blink_SliderIntIdx sld_idx);
typedef blink_Error         (*blink_host_write_param_add_flags)(blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_ParamFlags flags);
typedef blink_Error         (*blink_host_write_param_add_subparam)(blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_ParamIdx subparam_idx);
typedef blink_Error         (*blink_host_write_param_delegate)(blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_ParamIdx delegate_idx);
typedef blink_Error         (*blink_host_write_param_group)(blink_PluginIdx plugin_idx, blink_ParamIdx param_idx, blink_StaticString group_name);
typedef blink_Error         (*blink_host_write_slider_int_tweaker)(blink_SliderIntIdx sld_idx, blink_TweakerInt tweaker);
typedef blink_Error         (*blink_host_write_slider_real_tweaker)(blink_SliderRealIdx sld_idx, blink_TweakerReal tweaker);

struct blink_HostFns {
	blink_host_add_param_env                     add_param_env;
	blink_host_add_param_option                  add_param_option;
	blink_host_add_param_slider_int              add_param_slider_int;
	blink_host_add_param_slider_real             add_param_slider_real;
	blink_host_read_env_default_value            read_env_default_value;
	blink_host_read_param_env_env_idx            read_param_env_env_idx;
	blink_host_read_param_option_default_value   read_param_option_default_value;
	blink_host_read_param_slider_int_slider_idx  read_param_slider_int_slider_idx;
	blink_host_read_param_slider_real_slider_idx read_param_slider_real_slider_idx;
	blink_host_read_slider_int_default_value     read_slider_int_default_value;
	blink_host_read_slider_real_default_value    read_slider_real_default_value;
	blink_host_write_param_add_flags             write_param_add_flags;
	blink_host_write_param_add_subparam          write_param_add_subparam;
	blink_host_write_param_delegate	             write_param_delegate;
	blink_host_write_param_group                 write_param_group;
	blink_host_write_slider_int_tweaker          write_slider_int_tweaker;
	blink_host_write_slider_real_tweaker         write_slider_real_tweaker;
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
