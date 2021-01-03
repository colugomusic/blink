#pragma once

#include <stdint.h>

#define BLKHDGEN_VECTOR_SIZE 64
#define BLKHDGEN_OK 0
#define BLKHDGEN_TRUE 1
#define BLKHDGEN_FALSE 0

typedef uint8_t blkhdgen_ChannelCount;
typedef uint32_t blkhdgen_FrameCount;
typedef uint32_t blkhdgen_ParamCount;
typedef uint32_t blkhdgen_Index;
typedef uint32_t blkhdgen_ID;
typedef const char* blkhdgen_UUID;
typedef uint32_t blkhdgen_SR;
typedef uint8_t blkhdgen_BitDepth;
typedef double blkhdgen_Position;
typedef uint32_t blkhdgen_IntPosition;
typedef int8_t blkhdgen_Bool;
typedef uint32_t blkhdgen_Scale;

// Generators define their own error codes. Blockhead will probably just ignore
// any errors but this might be useful for future proofing
typedef int blkhdgen_Error;

typedef struct
{
	blkhdgen_IntPosition x;
	float y;
} blkhdgen_EnvelopePointPosition;

typedef struct
{
	blkhdgen_EnvelopePointPosition position;
	float curve;
} blkhdgen_EnvelopePoint;

typedef struct
{
	blkhdgen_Index count;
	blkhdgen_EnvelopePoint* points;
} blkhdgen_EnvelopePoints;

typedef struct
{
	blkhdgen_IntPosition x, y;
} blkhdgen_WarpPoint;

typedef struct
{
	blkhdgen_Index count;
	blkhdgen_WarpPoint* points;
} blkhdgen_WarpPoints;

typedef struct
{
	float min;
	float max;
} blkhdgen_Range;

typedef struct
{
	int min;
	int max;
} blkhdgen_IntRange;

typedef struct
{
	blkhdgen_Range range;
	float default_value;
	float step_size;
} blkhdgen_RangeValue;

typedef struct
{
	blkhdgen_IntRange range;
	int default_value;
	int step_size;
} blkhdgen_IntRangeValue;

typedef void (*blkhdgen_EnvelopeRangeAttribute_Set)(void* proc_data, float value);
typedef float (*blkhdgen_EnvelopeRangeAttribute_Get)(void* proc_data);

typedef struct
{
	blkhdgen_RangeValue range;

	void* proc_data;

	blkhdgen_EnvelopeRangeAttribute_Set set;
	blkhdgen_EnvelopeRangeAttribute_Get get;
} blkhdgen_EnvelopeRangeAttribute;

// Min/max values for envelope parameters can be modified by the user so
// the min and max values are themselves ranges with their own min and max
// values. So here we are representing the following data hierarchy:
// - min value:
//   - minimum min value
//   - maximum min value
//   - default min value
//   - step size for min value
// - max value:
//   - minimum max value
//   - maximum max value
//   - default max value
//   - step size for max value
typedef struct
{
	blkhdgen_EnvelopeRangeAttribute min;
	blkhdgen_EnvelopeRangeAttribute max;
} blkhdgen_EnvelopeRange;

typedef struct
{
	// Step size is also a range with a value that
	// can be configured by the user
	blkhdgen_RangeValue step_size;
	float default_snap_amount;
} blkhdgen_EnvelopeSnapSettings;

typedef float (*blkhdgen_Curve)(void* proc_data, float value);
typedef float (*blkhdgen_InverseCurve)(void* proc_data, float value);
typedef const char* (*blkhdgen_DisplayValue)(void* proc_data, float value);
typedef const char* (*blkhdgen_IntDisplayValue)(void* proc_data, int value);

//
// Sample Preprocessing
//
typedef bool (*blkhdgen_Preprocess_ShouldAbort)(void* host);
typedef void (*blkhdgen_Preprocess_ReportProgress)(void* host, float progress);

typedef struct
{
	blkhdgen_Preprocess_ShouldAbort should_abort;
	blkhdgen_Preprocess_ReportProgress report_progress;
} blkhdgen_PreprocessCallbacks;

typedef struct
{
	blkhdgen_ChannelCount num_channels;
	blkhdgen_FrameCount num_frames;
	blkhdgen_SR SR;
	blkhdgen_BitDepth bit_depth;
	void* aux_buffer;
} blkhdgen_SampleInfo;

enum blkhdgen_ParameterType
{
	blkhdgen_ParameterType_Chord,
	blkhdgen_ParameterType_Envelope,
	blkhdgen_ParameterType_Option,
	blkhdgen_ParameterType_Slider,
	blkhdgen_ParameterType_IntSlider,
	blkhdgen_ParameterType_Toggle,
};

enum blkhdgen_EnvelopeFlags
{
	blkhdgen_EnvelopeFlags_None = 0x0,
	blkhdgen_EnvelopeFlags_AlwaysShowButtonWhenGroupIsVisible = 0x1,
	blkhdgen_EnvelopeFlags_ShowIcon = 0x2,
	blkhdgen_EnvelopeFlags_ShowName = 0x4,
	blkhdgen_EnvelopeFlags_DefaultEnabled = 0x8,
	blkhdgen_EnvelopeFlags_DefaultVisible = 0x10,
};

enum blkhdgen_ToggleFlags
{
	blkhdgen_ToggleFlags_None = 0x0,
	blkhdgen_ToggleFlags_ShowButton = 0x1,
	blkhdgen_ToggleFlags_ShowIcon = 0x2,
	blkhdgen_ToggleFlags_ShowName = 0x3,
	blkhdgen_ToggleFlags_ShowInContextMenu = 0x4,
	blkhdgen_ToggleFlags_DefaultEnabled = 0x8,
};

typedef const blkhdgen_EnvelopePoints* (*blkhdgen_GetPointDataCB)(void* host);

//
// Envelope parameter
// Can be manipulated in Blockhead using the envelope editor
//
typedef blkhdgen_EnvelopeRange(*blkhdgen_Envelope_GetRange)(void* proc_data);
typedef blkhdgen_EnvelopeSnapSettings(*blkhdgen_Envelope_GetSnapSettings)(void* proc_data);
typedef blkhdgen_Error(*blkhdgen_Envelope_SetGetPointDataCB)(void* proc_data, void* host, blkhdgen_GetPointDataCB cb);

typedef struct
{
	enum blkhdgen_ParameterType parameter_type; // blkhdgen_ParameterType_Envelope

	void* proc_data;

	blkhdgen_Envelope_GetRange get_range;
	blkhdgen_Envelope_GetSnapSettings get_snap_settings;
	float default_value;
	int flags; // blkhdgen_EnvelopeFlags

	// Defines the value curve
	blkhdgen_Curve curve;
	blkhdgen_InverseCurve inverse_curve;

	// Convert a non-normalized value to a display string
	// e.g. "50" -> "50%"
	//   or "1200" -> "1.2 kHz"
	//
	// The returned buffer remains valid until the next call to display_value or
	// until the generator is destroyed.
	blkhdgen_DisplayValue display_value;

	// Host will call this once to set a callback that the plugin uses to
	// retrieve point data.
	//
	// It is the host's responsibility to ensure that the returned data remains
	// valid for the duration of the call.
	//
	// If the callback is called simultaneously from the GUI and audio threads
	// then the host may return two different pointers.
	//
	// The point data will be normalized
	blkhdgen_Envelope_SetGetPointDataCB set_get_point_data_cb;
} blkhdgen_Envelope;

//
// Chord parameter
// Can be manipulated in Blockhead using the chord/scale/harmonics editor thing
//
typedef struct
{
	blkhdgen_IntPosition position;
	blkhdgen_Scale scale;
} blkhdgen_ChordBlock;

typedef struct
{
	blkhdgen_Index count;
	blkhdgen_ChordBlock* blocks;
} blkhdgen_ChordData;

typedef blkhdgen_ChordData* (*blkhdgen_GetChordDataCB)(void* host);
typedef blkhdgen_Error(*blkhdgen_Chord_SetGetChordDataCB)(void* proc_data, void* host, blkhdgen_GetChordDataCB cb);

typedef struct
{
	enum blkhdgen_ParameterType parameter_type; // blkhdgen_ParameterType_Chord

	void* proc_data;

	blkhdgen_Chord_SetGetChordDataCB set_get_chord_data_cb;
} blkhdgen_Chord;

//
// Option parameter
// Will be displayed in Blockhead as a drop-down menu or radio buttons or something
//
typedef blkhdgen_Error(*blkhdgen_Option_Set)(void* proc_data, blkhdgen_Index value);
typedef const char* (*blkhdgen_Option_GetText)(void* proc_data, blkhdgen_Index value);

typedef struct
{
	enum blkhdgen_ParameterType parameter_type; // blkhdgen_ParameterType_Option
	blkhdgen_Range range;
	blkhdgen_Index default_value;

	void* proc_data;

	blkhdgen_Option_Set set;

	// Returns the display text for the option index
	blkhdgen_Option_GetText get_text;
} blkhdgen_Option;

//
// Slider parameter
// Will be displayed in Blockhead as a slider or spinbox control
//
typedef blkhdgen_Error(*blkhdgen_Slider_Set)(void* proc_data, float value);
typedef blkhdgen_Error(*blkhdgen_IntSlider_Set)(void* proc_data, int value);

typedef struct
{
	enum blkhdgen_ParameterType parameter_type; // blkhdgen_ParameterType_Slider
	blkhdgen_RangeValue range;

	void* proc_data;

	blkhdgen_Curve curve;
	blkhdgen_InverseCurve inverse_curve;
	blkhdgen_DisplayValue display_value;
	blkhdgen_Slider_Set set;
} blkhdgen_Slider;

typedef struct
{
	enum blkhdgen_ParameterType parameter_type; // blkhdgen_ParameterType_IntSlider
	blkhdgen_IntRangeValue range;

	void* proc_data;

	blkhdgen_IntDisplayValue display_value;
	blkhdgen_IntSlider_Set set;
} blkhdgen_IntSlider;

//
// Toggle parameter
// On/off value
//
typedef blkhdgen_Error(*blkhdgen_Toggle_Set)(void* proc_data, blkhdgen_Bool on);

typedef struct
{
	enum blkhdgen_ParameterType parameter_type; // blkhdgen_ParameterType_Toggle
	blkhdgen_Bool default_value;
	int flags; // blkhdgen_ToggleFlags

	void* proc_data;

	blkhdgen_Toggle_Set set;
} blkhdgen_Toggle;

//
// Generic Parameter
//
union blkhdgen_ParameterObject
{
	blkhdgen_Chord chord;
	blkhdgen_Envelope envelope;
	blkhdgen_Option option;
	blkhdgen_Slider slider;
	blkhdgen_IntSlider int_slider;
	blkhdgen_Toggle toggle;
};

typedef struct
{
	// Generators can share parameter UUIDs to allow the user to switch back and forth
	// between different generators without losing modulation data (for example the
	// "Amp", "Pan" and "Pitch" envelopes are shared between Classic and Fudge
	// generators.)
	blkhdgen_UUID uuid;

	// Zero if the parameter does not belong to a group
	blkhdgen_ID group_id;

	const char* name;

	union blkhdgen_ParameterObject parameter;
} blkhdgen_Parameter;

typedef struct
{
	blkhdgen_ID id;
	const char* name;
} blkhdgen_Group;

// Manipulators
//
// This will be a future Blockhead feature, plugins don't need
// to worry about it yet.
typedef struct
{
	blkhdgen_UUID uuid;
	blkhdgen_ID local_id;
	const char* name;
} blkhdgen_ManipulatorTarget;

typedef struct
{
	blkhdgen_ID local_id;
	float value;
	float glide_in;
	float glide_out;
} blkhdgen_ManipulatorPoint;

typedef struct
{
	blkhdgen_Index count;
	blkhdgen_ManipulatorPoint* points;
} blkhdgen_ManipulatorData;

//
// Callbacks
//
typedef const blkhdgen_SampleInfo* (*blkhdgen_GetSampleInfoCB)(void* host);
typedef blkhdgen_FrameCount(*blkhdgen_GetSampleDataCB)(void* host, blkhdgen_ChannelCount channel, blkhdgen_Index index, blkhdgen_FrameCount size, float* buffer);
typedef blkhdgen_WarpPoints* (*blkhdgen_GetWarpPointDataCB)(void* host);
typedef blkhdgen_ManipulatorData* (*blkhdgen_GetManipulatorDataCB)(void* host);

//
// Generator
//
typedef blkhdgen_Group (*blkhdgen_Generator_GetGroup)(void* proc_data, blkhdgen_Index index);
typedef blkhdgen_Group (*blkhdgen_Generator_GetGroupByID)(void* proc_data, blkhdgen_ID id);
typedef blkhdgen_Parameter (*blkhdgen_Generator_GetParameter)(void* proc_data, blkhdgen_Index index);
typedef blkhdgen_Parameter (*blkhdgen_Generator_GetParameterByID)(void* proc_data, blkhdgen_UUID uuid);
typedef const char* (*blkhdgen_Generator_GetErrorString)(void* proc_data, blkhdgen_Error error);
typedef blkhdgen_Error (*blkhdgen_Generator_SetGetSampleInfoCB)(void* proc_data, void* host, blkhdgen_GetSampleInfoCB cb);
typedef blkhdgen_Error (*blkhdgen_Generator_SetGetSampleDataCB)(void* proc_data, void* host, blkhdgen_GetSampleDataCB cb);
typedef blkhdgen_Error (*blkhdgen_Generator_SetGetWarpPointDataCB)(void* proc_data, void* host, blkhdgen_GetWarpPointDataCB cb);
typedef blkhdgen_Error (*blkhdgen_Generator_SetGetManipulatorDataCB)(void* proc_data, void* host, blkhdgen_GetManipulatorDataCB cb);
typedef blkhdgen_Error (*blkhdgen_Generator_SetDataOffset)(void* proc_data, int offset);
typedef blkhdgen_Error (*blkhdgen_Generator_Process)(void* proc_data, blkhdgen_SR song_rate, blkhdgen_SR sample_rate, const blkhdgen_Position* pos, float** out);
typedef blkhdgen_Error (*blkhdgen_Generator_GetWaveformPositions)(void* proc_data, const blkhdgen_Position* pos, float* out, float* derivatives);
typedef size_t (*blkhdgen_Generator_GetAuxBufferSize)(void* proc_data);
typedef blkhdgen_Error (*blkhdgen_Generator_PreprocessSample)(void* proc_data, void* host, blkhdgen_PreprocessCallbacks callbacks);

typedef struct
{
	blkhdgen_UUID uuid;
	const char* name;
	int num_groups;
	int num_parameters;
	blkhdgen_ChannelCount num_channels;
	int num_manipulator_targets;
	bool requires_preprocess;

	void* proc_data;

	blkhdgen_Generator_GetGroup get_group;
	blkhdgen_Generator_GetGroupByID get_group_by_id;
	blkhdgen_Generator_GetParameter get_parameter;
	blkhdgen_Generator_GetParameterByID get_parameter_by_id;
	blkhdgen_Generator_SetDataOffset set_data_offset;

	// Returned buffer remains valid until the next call to get_error_string or
	// until the generator is destroyed
	blkhdgen_Generator_GetErrorString get_error_string;

	// Host will call these once to set callbacks that the plugin uses to
	// retrieve data.
	//
	// It is the host's responsibility to ensure that the returned data remains
	// valid for the duration of the call.
	//
	// If the callback is called simultaneously from the GUI and audio threads
	// then the host may return two different pointers.
	blkhdgen_Generator_SetGetSampleInfoCB set_get_sample_info_cb;
	blkhdgen_Generator_SetGetSampleDataCB set_get_sample_data_cb;
	blkhdgen_Generator_SetGetWarpPointDataCB set_get_warp_point_data_cb;
	blkhdgen_Generator_SetGetManipulatorDataCB set_get_manipulator_data_cb;

	// pos is a buffer of length BLKHDGEN_VECTOR_SIZE containing block positions.
	//
	// Positions will usually be increasing linearly but may jump back in the case of
	// loop events.
	//
	// Be aware that Blockhead supports looping over extremely small regions (less
	// than BLKHDGEN_VECTOR_SIZE)
	//
	// The is the only function that is called in the audio thread
	//
	// output pointer is aligned on a 16-byte boundary
	blkhdgen_Generator_Process process;

	// Get the transformed waveform positions and derivatives for the given block
	// positions
	//
	// output pointers are aligned on 16-byte boundaries
	blkhdgen_Generator_GetWaveformPositions get_waveform_positions;

	// The host can allocate a buffer of memory that the plugin can use to read
	// and write data related to the current sample. The lifetime of the buffer
	// will be tied to the sample so it won't be freed unless the sample is 
	// deleted.
	//
	// This function should return the size in bytes of the required buffer.
	//
	// The plugin can use the GetSampleInfo/GetSampleData callbacks to access the
	// current sample.
	//
	// The host will always wait for this function to complete before process() is
	// called for the current sample.
	//
	// If no buffer is required then the plugin should return zero.
	blkhdgen_Generator_GetAuxBufferSize get_required_aux_buffer_size;

	// Called by the host once per sample if requires_preprocess==true
	//
	// This function will be called in a separate thread from everything else.
	//
	// The plugin should periodically call the ShouldAbort callback and stop
	// preprocessing if it returns true.
	//
	// The plugin should periodically report the completion percentage by
	// calling the ReportProgress callback with a value between 0 and 1.
	//
	// If get_required_aux_buffer_size() returns > 0 then the required buffer
	// will be available via the GetSampleInfo callback.
	//
	// The process() function may be called in the audio thread before
	// preprocessing has finished so the plugin needs to synchronize this
	// situation. It is ok to simply return silence until preprocessing has
	// completed.
	blkhdgen_Generator_PreprocessSample preprocess_sample;
} blkhdgen_Generator;

#ifdef BLKHDGEN_EXPORT

#ifdef _WIN32
# define EXPORTED  __declspec( dllexport )
#else
# define EXPORTED
#endif

EXPORTED blkhdgen_Generator make_generator();
EXPORTED blkhdgen_Error destroy_generator(blkhdgen_Generator generator);

#endif