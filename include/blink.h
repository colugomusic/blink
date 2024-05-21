#pragma once

// This is meant to be a C interface. I have only ever compiled it with C++ compilers though so
// it's probably not. If you want to help me fix the C-ness of it then pull requests are welcome.

#include <stddef.h>
#include <stdint.h>

#define BLINK_VECTOR_SIZE 64
#define BLINK_OK 0
#define BLINK_TRUE 1
#define BLINK_FALSE 0

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

typedef struct {      double value; } blink_Position;
typedef struct {      int8_t value; } blink_Bool;
typedef struct {     int32_t value; } blink_EnvelopeIndex;
typedef struct {     int32_t value; } blink_EnvelopeParameterIndex;
typedef struct {     int32_t value; } blink_Flags;
typedef struct {     int32_t value; } blink_ID;
typedef struct {     int32_t value; } blink_InstanceIndex;
typedef struct {     int32_t value; } blink_IntSliderIndex;
typedef struct {     int32_t value; } blink_IntSliderParameterIndex;
typedef struct {     int32_t value; } blink_OptionParameterIndex;
typedef struct {     int32_t value; } blink_ParameterIndex;
typedef struct {     int32_t value; } blink_RealSliderIndex;
typedef struct {     int32_t value; } blink_RealSliderParameterIndex;
typedef struct {     int32_t value; } blink_UnitIndex;
typedef struct {     uint8_t value; } blink_BitDepth;
typedef struct {     uint8_t value; } blink_ChannelCount;
typedef struct {    uint32_t value; } blink_ParamCount;
typedef struct {    uint32_t value; } blink_Scale;
typedef struct {    uint32_t value; } blink_SR;
typedef struct {    uint64_t value; } blink_FrameCount;
typedef struct { const char* value; } blink_StaticString;
typedef struct { const char* value; } blink_TemporaryString;
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
typedef int blink_Error;

enum blink_StdError {
	blink_StdError_None = BLINK_OK,
	blink_StdError_AlreadyInitialized = -1,
	blink_StdError_NotInitialized = -2,
	blink_StdError_NotImplemented = -3,
	blink_StdError_InvalidInstance = -4,
};

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
// Parameter types
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
enum blink_ParameterType {
	blink_ParameterType_Chord,
	blink_ParameterType_Envelope,
	blink_ParameterType_Option,
	blink_ParameterType_Slider,
	blink_ParameterType_IntSlider,
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Parameter data
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct {
	blink_ParameterType type;
	blink_ChordBlocks points;
} blink_ChordData;

typedef struct {
	blink_ParameterType type;
	blink_RealPoints points;
} blink_EnvelopeData;

typedef struct {
	blink_ParameterType type;
	blink_IntPoints points;
} blink_OptionData;

typedef struct {
	blink_ParameterType type;
	blink_RealPoints points;
} blink_SliderData;

typedef struct {
	blink_ParameterType type;
	blink_IntPoints points;
} blink_IntSliderData;

union blink_ParameterData {
	blink_ParameterType type;
	blink_ChordData chord;
	blink_EnvelopeData envelope;
	blink_OptionData option;
	blink_SliderData slider;
	blink_IntSliderData int_slider;
};
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Parameter data END
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

typedef struct {
	// Step size is also a range with a value that
	// can be configured by the user
	blink_RealSliderIndex step_size_slider;
	float default_snap_amount;
} blink_EnvelopeSnapSettings;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Parameter Flags
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

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Parameter Flags END
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Manipulator Settings
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct {
	// Either of these can be empty, but at least one must be defined
	// if blink_ParamFlags_CanManipulate is set
	blink_EnvelopeIndex offset_envelope;
	blink_EnvelopeIndex override_envelope; 
} blink_ManipulatorSettings;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Manipulator Settings END
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

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
	blink_ParameterIndex manipulation_delegate; 
} blink_ParameterSettings;

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

#ifdef BLINK_EXPORT

#ifdef _WIN32
# define EXPORTED  __declspec( dllexport )
#else
# define EXPORTED
#endif

extern "C"
{
	EXPORTED blink_Error           blink_destroy_instance(blink_InstanceIndex instance_idx);
	EXPORTED blink_Error           blink_init();
	EXPORTED blink_Error           blink_stream_init(blink_InstanceIndex instance_idx, blink_SR SR);
	EXPORTED blink_Error           blink_terminate();
	EXPORTED blink_InstanceIndex   blink_make_instance();
	EXPORTED blink_PluginInfo      blink_get_plugin_info();
	EXPORTED blink_UnitIndex       blink_add_unit(blink_InstanceIndex instance_idx);
	EXPORTED size_t                blink_get_num_parameters();
	EXPORTED blink_TemporaryString blink_get_error_string(blink_Error error);
	EXPORTED blink_ResourceData    blink_get_resource_data(const char* path); // Optional

	// Parameter
	EXPORTED blink_Flags             blink_get_parameter_flags(blink_ParameterIndex param_idx);
	EXPORTED blink_ParameterIndex    blink_get_parameter_by_uuid(blink_UUID uuid);
	EXPORTED blink_ParameterIndex    blink_param_get_subparameter(blink_ParameterIndex param_idx, size_t index);
	EXPORTED blink_ParameterSettings blink_param_get_settings(blink_ParameterIndex param_idx);
	EXPORTED blink_ParameterType     blink_param_get_type(blink_ParameterIndex param_idx);
	EXPORTED blink_StdIcon           blink_get_icon(blink_ParameterIndex param_idx);
	EXPORTED blink_UUID              blink_get_parameter_uuid(blink_ParameterIndex param_idx);
	EXPORTED size_t                  blink_param_get_num_subparameters(blink_ParameterIndex param_idx);

	// Envelope Parameter
	EXPORTED float                     blink_param_env_apply_offset(blink_EnvelopeParameterIndex env_idx float value, float offset);
	EXPORTED blink_EnvelopeIndex       blink_param_env_get_env(blink_EnvelopeParameterIndex env_idx);
	EXPORTED blink_ManipulatorSettings blink_param_env_get_manipulator_settings(blink_EnvelopeParameterIndex env_idx);
	EXPORTED blink_Range               blink_param_env_get_clamp_range(blink_EnvelopeParameterIndex env_idx);

	// Int Slider Parameter
	EXPORTED blink_IntSliderIndex     blink_param_int_slider_get_slider(blink_IntSliderParameterIndex sld_idx);

	// Real Slider Parameter
	EXPORTED float                     blink_param_slider_apply_offset(blink_RealSliderParameterIndex env_idx float value, float offset);
	EXPORTED blink_ManipulatorSettings blink_param_slider_get_manipulator_settings(blink_RealSliderParameterIndex sld_idx);
	EXPORTED blink_Range               blink_param_slider_get_clamp_range(blink_RealSliderParameterIndex sld_idx);
	EXPORTED blink_RealSliderIndex     blink_param_slider_get_slider(blink_RealSliderParameterIndex sld_idx);

	// Option Parameter
	EXPORTED blink_TemporaryString blink_param_option_get_text(blink_OptionParameterIndex opt_idx, int64_t value);
	EXPORTED int64_t               blink_param_option_get_default_value(blink_OptionParameterIndex opt_idx);
	EXPORTED int64_t               blink_param_option_get_max_value(blink_OptionParameterIndex opt_idx);

	// Envelope
	EXPORTED blink_Bool                 blink_env_from_string(blink_EnvelopeIndex env_indx, const char* str, float* value);
	EXPORTED blink_Bool                 blink_env_get_grid_line(blink_EnvelopeIndex env_idx, int index, float* out);
	EXPORTED blink_Bool                 blink_env_get_step_line(blink_EnvelopeIndex env_idx, int index, float step_size, float* out);
	EXPORTED blink_Bool                 blink_env_should_show_grid_labels(blink_EnvelopeIndex env_idx);
	EXPORTED blink_EnvelopeSnapSettings blink_env_get_snap_settings(blink_EnvelopeIndex env_indx);
	EXPORTED blink_RealSliderIndex      blink_env_get_max_slider(blink_EnvelopeIndex env_idx);
	EXPORTED blink_RealSliderIndex      blink_env_get_min_slider(blink_EnvelopeIndex env_idx);
	EXPORTED blink_RealSliderIndex      blink_env_get_value_slider(blink_EnvelopeIndex env_idx);
	EXPORTED blink_TemporaryString      blink_env_to_string(blink_EnvelopeIndex env_indx, float value);
	EXPORTED float                      blink_env_get_default_value(blink_EnvelopeIndex env_idx);
	EXPORTED float                      blink_env_search(blink_EnvelopeIndex env_idx, const blink_EnvelopeData* data, float block_position);
	EXPORTED float                      blink_env_snap_value(blink_EnvelopeIndex env_indx, float value, float step_size, float snap_amount);
	EXPORTED float                      blink_env_stepify(blink_EnvelopeIndex env_idx, float value);

	// Real Slider
	EXPORTED blink_Bool            blink_slider_from_string(blink_RealSliderIndex sld_idx, const char* str, float* value);
	EXPORTED blink_TemporaryString blink_slider_to_string(blink_RealSliderIndex sld_idx, float value);
	EXPORTED float                 blink_slider_constrain(blink_RealSliderIndex sld_idx, float value);
	EXPORTED float                 blink_slider_decrement(blink_RealSliderIndex sld_idx, float value, blink_Bool precise);
	EXPORTED float                 blink_slider_drag(blink_RealSliderIndex sld_idx, float start_value, int amount, blink_Bool precise);
	EXPORTED float                 blink_slider_get_default_value(blink_RealSliderIndex sld_idx);
	EXPORTED float                 blink_slider_increment(blink_RealSliderIndex sld_idx, float value, blink_Bool precise);
	EXPORTED float                 blink_slider_snap_value(blink_RealSliderIndex sld_idx, float value, float step_size, float snap_amount);
	EXPORTED float                 blink_slider_stepify(blink_RealSliderIndex sld_idx, float value);

	// Int Slider
	EXPORTED blink_Bool            blink_int_slider_from_string(blink_IntSliderIndex sld_idx, const char* str, int64_t* value);
	EXPORTED blink_TemporaryString blink_int_slider_to_string(blink_IntSliderIndex sld_idx, int64_t value);
	EXPORTED int64_t               blink_int_slider_constrain(blink_IntSliderIndex sld_idx, int64_t value);
	EXPORTED int64_t               blink_int_slider_decrement(blink_IntSliderIndex sld_idx, int64_t value, blink_Bool precise);
	EXPORTED int64_t               blink_int_slider_drag(blink_IntSliderIndex sld_idx, int64_t start_value, int64_t amount, blink_Bool precise);
	EXPORTED int64_t               blink_int_slider_increment(blink_IntSliderIndex sld_idx, int64_t value, blink_Bool precise);
	EXPORTED int64_t               blink_slider_get_default_value(blink_RealSliderIndex sld_idx);
}

#endif
