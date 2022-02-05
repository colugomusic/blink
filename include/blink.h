#pragma once

#include <stddef.h>
#include <stdint.h>

#define BLINK_VECTOR_SIZE 64
#define BLINK_OK 0
#define BLINK_TRUE 1
#define BLINK_FALSE 0

#define BLINK_STD_UUID_AMP "273e7c30-404b-4db6-ba97-20f33d49fe51"
#define BLINK_STD_UUID_SCALE "860166f7-e839-448b-bd3b-a5bccbfe3ac1"
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
#define BLINK_STD_UUID_REVERSE "e7cacaf8-4afc-4e81-83de-50620fed4b13"
#define BLINK_STD_UUID_SAMPLE_OFFSET "88373752-7656-4d0e-8da2-a18c05af0106"
#define BLINK_STD_UUID_SPEED "04293c38-3a64-42b2-80f0-43a4f8190ba7"

#define BLINK_STD_CATEGORY_DESTRUCTION "Destruction"
#define BLINK_STD_CATEGORY_DYNAMICS "Dynamics"
#define BLINK_STD_CATEGORY_FILTERS "Filters"
#define BLINK_STD_CATEGORY_SPACE "Space"
#define BLINK_STD_CATEGORY_TONAL "Tonal"

typedef uint8_t blink_ChannelCount;
typedef uint64_t blink_FrameCount;
typedef uint32_t blink_ParamCount;
typedef uint32_t blink_Index;
typedef int32_t blink_ID;
typedef const char* blink_UUID;
typedef uint32_t blink_SR;
typedef uint8_t blink_BitDepth;
typedef double blink_Position;
typedef int32_t blink_IntPosition;
typedef int8_t blink_Bool;
typedef uint32_t blink_Scale;

enum blink_ChannelMode
{
	blink_ChannelMode_Left = 0,
	blink_ChannelMode_Right = 1,
	blink_ChannelMode_Stereo = 2,
	blink_ChannelMode_StereoSwap = 3,
};

enum blink_StdIcon
{
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

// Plugins define their own error codes. Blockhead will probably just ignore
// any errors but this might be useful for future proofing
typedef int blink_Error;

// Some shared error codes. Plugin-specific error codes should be > 0
enum blink_StdError
{
	blink_StdError_None = 0,
	blink_StdError_AlreadyInitialized = -1,
	blink_StdError_NotInitialized = -2,
	blink_StdError_NotImplemented = -3,
	blink_StdError_ManipulatorTargetDoesNotExist = -4,
};

typedef struct
{
	blink_IntPosition x;
	blink_Bool value;
} blink_BoolPoint;

typedef struct
{
	blink_IntPosition x;
	float y;
} blink_FloatPoint;

typedef struct
{
	blink_IntPosition x;
	int32_t y;
} blink_IntPoint;

typedef struct
{
	blink_Index count;
	blink_BoolPoint* points;
} blink_BoolPoints;

typedef struct
{
	blink_Index count;
	blink_FloatPoint* points;
	float min;
	float max;
} blink_FloatPoints;

typedef struct
{
	blink_Index count;
	blink_IntPoint* points;
	int32_t min;
	int32_t max;
} blink_IntPoints;

typedef struct
{
	float min;
	float max;
} blink_Range;

typedef struct
{
	int min;
	int max;
} blink_IntRange;

enum blink_ParameterType
{
	blink_ParameterType_Chord,
	blink_ParameterType_Envelope,
	blink_ParameterType_Option,
	blink_ParameterType_Slider,
	blink_ParameterType_IntSlider,
	blink_ParameterType_Toggle,
};

typedef struct
{
	blink_IntPosition position;
	blink_Scale scale;
} blink_ChordBlock;

typedef struct
{
	blink_Index count;
	blink_ChordBlock* blocks;
} blink_ChordBlocks;

typedef struct
{
	blink_ParameterType type;
	blink_ChordBlocks blocks;
} blink_ChordData;

typedef struct
{
	blink_ParameterType type;
	blink_FloatPoints points;
} blink_EnvelopeData;

typedef struct
{
	blink_ParameterType type;
	blink_FloatPoints data;
} blink_OptionData;

typedef struct
{
	blink_ParameterType type;
	blink_FloatPoints data;
} blink_SliderData;

typedef struct
{
	blink_ParameterType type;
	int32_t value;
} blink_IntSliderData;

typedef struct
{
	blink_ParameterType type;
	blink_BoolPoints data;
} blink_ToggleData;

union blink_ParameterData
{
	blink_ParameterType type;
	blink_ChordData chord;
	blink_EnvelopeData envelope;
	blink_OptionData option;
	blink_SliderData slider;
	blink_IntSliderData int_slider;
	blink_ToggleData toggle;
};

typedef float (*blink_Stepify)(void* proc_data, float value);
typedef float (*blink_SnapValue)(void* proc_data, float value, float step_size, float snap_amount);
typedef float (*blink_Constrain)(void* proc_data, float value);
typedef float (*blink_Drag)(void* proc_data, float start_value, int amount, blink_Bool precise);
typedef float (*blink_Increment)(void* proc_data, float value, blink_Bool precise);
typedef float (*blink_Decrement)(void* proc_data, float value, blink_Bool precise);
typedef const char* (*blink_DisplayValue)(void* proc_data, float value);
typedef blink_Bool (*blink_FromString)(void* proc_data, const char* str, float* value);
typedef int (*blink_IntConstrain)(void* proc_data, int value);
typedef int (*blink_IntDrag)(void* proc_data, int start_value, int amount, blink_Bool precise);
typedef int (*blink_IntIncrement)(void* proc_data, int value, blink_Bool precise);
typedef int (*blink_IntDecrement)(void* proc_data, int value, blink_Bool precise);
typedef const char* (*blink_IntDisplayValue)(void* proc_data, int value);
typedef blink_Bool (*blink_IntFromString)(void* proc_data, const char* str, int* value);

typedef struct
{
	float default_value;

	void* proc_data;

	blink_DisplayValue display_value;
	blink_FromString from_string;
	blink_Constrain constrain;
	blink_Drag drag;
	blink_Increment increment;
	blink_Decrement decrement;
	blink_Stepify stepify;
} blink_Slider;

typedef struct
{
	int default_value;

	void* proc_data;

	blink_IntDisplayValue display_value;
	blink_IntFromString from_string;
	blink_IntConstrain constrain;
	blink_IntDrag drag;
	blink_IntIncrement increment;
	blink_IntDecrement decrement;
} blink_IntSlider;

typedef struct
{
	// Step size is also a range with a value that
	// can be configured by the user
	blink_Slider step_size;
	float default_snap_amount;
} blink_EnvelopeSnapSettings;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Parameter Flags
//
// If a parameter is flagged with 'CanManipulate' it just means that a manipulator
// can be created for the associated UUID through the UI for that parameter.
//
// If a parameter is flagged with 'IsManipulatorTarget' then if a manipulator is
// acting on the associated UUID then this parameter will be the one whose data
// is actually transformed by the host.
//
// Different parameters may share the same UUID within the same plugin (for example
// a Pitch envelope and a Pitch slider which work together), but only one parameter
// per-UUID may be flagged as 'IsManipulatorTarget'.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
enum blink_ChordFlags
{
	blink_ChordFlags_None                               = 1 << 0,
	blink_ChordFlags_AlwaysShowButtonWhenGroupIsVisible = 1 << 1, // Will not appear in modulator tree
	blink_ChordFlags_DefaultActive                      = 1 << 2,
	blink_ChordFlags_DefaultDisabled                    = 1 << 3,
	blink_ChordFlags_IconOnly                           = 1 << 4, // Only show icon in button
	blink_ChordFlags_MovesDisplay                       = 1 << 5, // Editing should trigger a visual update
	blink_ChordFlags_CanManipulate                      = 1 << 6,
	blink_ChordFlags_IsManipulatorTarget                = 1 << 7,
};

enum blink_EnvelopeFlags
{
	blink_EnvelopeFlags_None                               = 1 << 0,
	blink_EnvelopeFlags_AlwaysShowButtonWhenGroupIsVisible = 1 << 1, // Will not appear in modulator tree
	blink_EnvelopeFlags_DefaultActive                      = 1 << 2,
	blink_EnvelopeFlags_DefaultDisabled                    = 1 << 3,
	blink_EnvelopeFlags_DefaultAlwaysVisible               = 1 << 4,
	blink_EnvelopeFlags_IconOnly                           = 1 << 5, // Only show icon in button
	blink_EnvelopeFlags_MovesDisplay                       = 1 << 6, // Editing should trigger a visual update
	blink_EnvelopeFlags_CanManipulate                      = 1 << 7,
	blink_EnvelopeFlags_IsManipulatorTarget                = 1 << 8,
	blink_EnvelopeFlags_HostClamp                          = 1 << 9, // Host will clamp values to clamp_range,
	                                                                 // after applying manipulator offsets
};

enum blink_OptionFlags
{
	blink_OptionFlags_None                = 1 << 0,
	blink_OptionFlags_CanManipulate       = 1 << 1,
	blink_OptionFlags_IsManipulatorTarget = 1 << 2,
	blink_OptionFlags_MovesDisplay        = 1 << 3, // Editing should trigger a visual update
	blink_OptionFlags_Morphable           = 1 << 4, // Can the host can send us fractional values?
};

enum blink_SliderFlags
{
	blink_SliderFlags_None                = 1 << 0,
	blink_SliderFlags_MovesDisplay        = 1 << 1, // Editing should trigger a visual update
	blink_SliderFlags_NonGlobal           = 1 << 2, // Do not create a global control for this slider (can be used
	                                                // to create sliders which are only visible when an envelope is
	                                                // selected)
	blink_SliderFlags_CanManipulate       = 1 << 3, // Has no effect for int sliders
	blink_SliderFlags_IsManipulatorTarget = 1 << 4,
	blink_SliderFlags_HostClamp           = 1 << 5, // Host will clamp values to clamp_range,
	                                                // after applying manipulator offsets
};

enum blink_ToggleFlags
{
	blink_ToggleFlags_None                = 1 << 0,
	blink_ToggleFlags_ShowButton          = 1 << 1,
	blink_ToggleFlags_ShowInContextMenu   = 1 << 2,
	blink_ToggleFlags_MovesDisplay        = 1 << 3, // Editing should trigger a visual update
	blink_ToggleFlags_IconOnly            = 1 << 4, // Only show icon in toggle
	blink_ToggleFlags_CanManipulate       = 1 << 5,
	blink_ToggleFlags_IsManipulatorTarget = 1 << 6,
};

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Parameter Flags END
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//
// Option parameter
//
// Will be displayed in Blockhead as a drop-down menu or radio buttons or something
//
// Values are actually stored as floats, not ints, to enable morphing via
// manipulators when blink_OptionFlags_Morphable is set
//
typedef const char* (*blink_Option_GetText)(void* proc_data, blink_Index index);

typedef struct
{
	enum blink_ParameterType parameter_type; // blink_ParameterType_Option
	blink_Index max_index;
	blink_Index default_index;

	void* proc_data;
	int flags; // blink_OptionFlags

	// Returns the display text for the option index
	blink_Option_GetText get_text;

} blink_Option;

//
// Envelope parameter
// Can be edited in Blockhead using the envelope editor
//
typedef blink_Bool (*blink_GetGridLine)(void* proc_data, int index, float* out);
typedef blink_Bool (*blink_GetStepLine)(void* proc_data, int index, float step_size, float* out);
typedef float (*blink_EnvelopeSearch)(void* proc_data, const blink_EnvelopeData* data, float block_position);
typedef blink_Index (*blink_GetOption)(void* proc_data, blink_Index index);
typedef blink_Index (*blink_GetSlider)(void* proc_data, blink_Index index);

typedef struct
{
	void* proc_data;

	float default_value;
	bool show_grid_labels;

	blink_Slider value_slider;
	blink_Slider min_slider;
	blink_Slider max_slider;
	blink_EnvelopeSnapSettings snap_settings;

	blink_DisplayValue display_value;
	blink_FromString from_string;
	blink_EnvelopeSearch search;
	blink_GetGridLine get_gridline;
	blink_GetStepLine get_stepline;
	blink_Stepify stepify;
	blink_SnapValue snap_value;
} blink_Envelope;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Manipulators
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef float (*blink_ApplyOffset)(void* proc_data, float value, float offset);

typedef struct
{
	void* proc_data;

	// Either of these can be null, but at least one must be defined
	blink_Envelope* offset_envelope;
	blink_Envelope* override_envelope;

	// Can be null, even if offset_envelope is set, in which case
	// the host will default to applying offset values by simple
	// addition
	blink_ApplyOffset apply_offset;

} blink_EnvelopeManipulatorTarget;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Manipulators END
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

typedef struct
{
	enum blink_ParameterType parameter_type; // blink_ParameterType_Envelope

	void* proc_data;
	int flags; // blink_EnvelopeFlags
	blink_StdIcon icon;

	// Only used if blink_EnvelopeFlags_HostClamp is set
	blink_Range clamp_range;

	blink_Index sliders_count;
	blink_Index options_count;

	blink_GetOption get_option;
	blink_GetSlider get_slider;

	blink_Envelope envelope;
} blink_EnvelopeParameter;

//
// Chord parameter
// Can be edited in Blockhead using the chord/scale/harmonics editor thing
//
typedef struct
{
	enum blink_ParameterType parameter_type; // blink_ParameterType_Chord
	int flags; // blink_ChordFlags
	blink_StdIcon icon;
} blink_Chord;

//
// Slider parameter
// Will be displayed in Blockhead as a slider or spinbox control
//

typedef struct
{
	enum blink_ParameterType parameter_type; // blink_ParameterType_Slider
	int flags; // blink_SliderFlags
	blink_StdIcon icon;
	blink_Slider slider;

	// Only used if blink_SliderFlags_HostClamp is set
	blink_Range clamp_range;
} blink_SliderParameter;

typedef struct
{
	enum blink_ParameterType parameter_type; // blink_ParameterType_IntSlider
	blink_StdIcon icon;
	int flags; // blink_SliderFlags
	blink_IntSlider slider;
} blink_IntSliderParameter;

//
// Toggle parameter
// On/off value
//
typedef struct
{
	enum blink_ParameterType parameter_type; // blink_ParameterType_Toggle
	blink_Bool default_value;
	blink_StdIcon icon;
	int flags; // blink_ToggleFlags
} blink_Toggle;

//
// Generic Parameter
//
union blink_ParameterObject
{
	enum blink_ParameterType type;
	blink_Chord chord;
	blink_EnvelopeParameter envelope;
	blink_Option option;
	blink_SliderParameter slider;
	blink_IntSliderParameter int_slider;
	blink_Toggle toggle;
};

typedef struct
{
	// The UUID is unique to a particular parameter concept e.g. "Amp", "Pitch", etc.

	// Can be shared between different parameter types e.g. the plugin could declare
	// both an Amp envelope and and Amp slider with the same UUID.
	
	// Declaring multiple parameters with the same UUID, of the same type, within
	// the same plugin is invalid.

	// UUIDs can be shared between different plugins
	blink_UUID uuid;

	// < 0 if the parameter does not belong to a group
	blink_ID group_index;

	// Full parameter name e.g. "Noise Amount"
	const char* name;

	// Short parameter name to use in the context of a group, e.g. "Amount". Can be null
	const char* short_name;

	// Long description of the parameter. Can be null
	const char* long_desc;

	union blink_ParameterObject parameter;
} blink_Parameter;

typedef struct
{
	const char* name;
} blink_Group;

typedef struct
{
	size_t size;
	const char* data;
} blink_ResourceData;

typedef struct
{
	// Unique identifier for this plugin
	blink_UUID uuid;

	// The name of the plugin
	const char* name;

	// The plugin category as a list of strings delimited by '|'
	//
	// e.g. 
	//	"Destruction"
	//	"Filters|Butterworth"
	//	"Space|Reverbs"
	// 	"Developer Name|Phasers"

	// May be null for no category
	const char* category;

	// The plugin version string
	// Usual format is something like v0.0.0
	const char* version;

	// If this is true, the host will try to find an icon resource at res/icon.png
	// If there isn't one, a default icon will be used
	blink_Bool has_icon;
} blink_PluginInfo;

typedef blink_Error(*blink_Instance_StreamInit)(void* proc_data, blink_SR SR);

#ifdef BLINK_EXPORT

#ifdef _WIN32
# define EXPORTED  __declspec( dllexport )
#else
# define EXPORTED
#endif

extern "C"
{
	EXPORTED blink_PluginInfo blink_get_plugin_info();
	EXPORTED blink_Error blink_init();
	EXPORTED blink_Error blink_terminate();
	EXPORTED int blink_get_num_groups();
	EXPORTED int blink_get_num_parameters();
	EXPORTED blink_Group blink_get_group(blink_Index index);
	EXPORTED blink_Parameter blink_get_parameter(blink_Index index);
	EXPORTED blink_Parameter blink_get_parameter_by_uuid(blink_UUID uuid);

	// Optional, but required if manipulators are enabled
	EXPORTED blink_Error blink_get_envelope_manipulator_target(blink_UUID uuid, blink_EnvelopeManipulatorTarget* out);

	// Optional
	EXPORTED blink_ResourceData blink_get_resource_data(const char* path);

	// Returned buffer remains valid until the next call to get_error_string or
	// until the plugin is destroyed
	EXPORTED const char* blink_get_error_string(blink_Error error);
}

#endif
