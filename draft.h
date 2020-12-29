#pragma once

#include <stdint.h>

#define BLKHDGEN_VECTOR_SIZE 64
#define BLKHDGEN_OK 0

typedef uint8_t blkhdgen_ChannelCount;
typedef uint32_t blkhdgen_FrameCount;
typedef uint32_t blkhdgen_ParamCount;
typedef uint32_t blkhdgen_Index;
typedef uint32_t blkhdgen_ID;
typedef uint32_t blkhdgen_SR;
typedef uint8_t blkhdgen_BitDepth;
typedef double blkhdgen_Position;
typedef uint32_t blkhdgen_IntPosition;
typedef int8_t blkhdgen_Bool;

// Generators define their own error codes. Blockhead will probably just ignore
// any errors but this might be useful for future proofing
typedef int blkhdgen_Error;

// IDs of standard Blockhead parameters which allow the user to switch back and
// forth between generators without losing modulation data (for example the "Amp",
// "Pan" and "Pitch" envelopes are shared between Classic and Fudge generators.)
//
// Generators are not required to use these
enum blkhdgen_StdParam
{
    blkhdgen_StdParam_Amp = -1,
    blkhdgen_StdParam_Pan = -2,
    blkhdgen_StdParam_Pitch = -3,
    blkhdgen_StdParam_Speed = -4,
    blkhdgen_StdParam_Formant = -5,
};

typedef struct
{
    float min;
    float max;
    float step_size;
} blkhdgen_Range;

typedef void (*blkhdgen_EnvelopeAttribute_Set)(void* proc_data, float value);
typedef float (*blkhdgen_EnvelopeAttribute_Get)(void* proc_data);

typedef struct 
{
    blkhdgen_Range range;

    void* proc_data;

    blkhdgen_EnvelopeAttribute_Set set;
    blkhdgen_EnvelopeAttribute_Get get;
} blkhdgen_EnvelopeAttribute;

// Min/max values for envelope parameters can be modified by the user so
// the min and max values are themselves ranges with their own min and max
// values. So here we are representing the following data hierarchy:
// - min value:
//   - minimum min value
//   - maximum min value
//   - step size for min value
// - max value:
//   - minimum max value
//   - maximum max value
//   - step size for max value
typedef struct
{
    blkhdgen_EnvelopeAttribute min;
    blkhdgen_EnvelopeAttribute max;
} blkhdgen_EnvelopeRange;

typedef float (*blkhdgen_NormalizePoint)(void* proc_data, float value);
typedef float (*blkhdgen_InverseNormalizePoint)(void* proc_data, float value);
typedef const char* (*blkhdgen_DisplayValue)(void* proc_data, float value);

typedef struct 
{
    blkhdgen_ChannelCount num_channels;
    blkhdgen_FrameCount num_frames;
    blkhdgen_SR SR;
    blkhdgen_BitDepth bit_depth;
} blkhdgen_SampleInfo;

enum blkhdgen_ParameterType
{
    blkhdgen_ParameterType_Chord,
    blkhdgen_ParameterType_Envelope,
    blkhdgen_ParameterType_Option,
    blkhdgen_ParameterType_Slider,
    blkhdgen_ParameterType_Toggle,
};

enum blkhdgen_ParameterFlags
{
    blkhdgen_ParameterFlags_None = 0x0,
    blkhdgen_ParameterFlags_AlwaysShowButtonWhenGroupIsVisible = 0x1,
    blkhdgen_ParameterFlags_ShowIcon = 0x2,
    blkhdgen_ParameterFlags_ShowName = 0x4,
    blkhdgen_ParameterFlags_DefaultOn = 0x8,
};

//
// Envelope parameter
// Can be manipulated in Blockhead using the envelope editor
//
typedef blkhdgen_EnvelopeRange* (*blkhdgen_Envelope_GetRange)(void* proc_data);
typedef blkhdgen_Index (*blkhdgen_Envelope_AddPoint)(void* proc_data, blkhdgen_IntPosition position, float value);
typedef blkhdgen_Error (*blkhdgen_Envelope_RemovePoint)(void* proc_data, blkhdgen_Index index);
typedef blkhdgen_Error (*blkhdgen_Envelope_MovePoint)(void* proc_data, blkhdgen_Index index, blkhdgen_IntPosition new_position, float new_value);
typedef blkhdgen_Error (*blkhdgen_Envelope_Clear)(void* proc_data);
typedef blkhdgen_Error (*blkhdgen_Envelope_SetPointCurve)(void* proc_data, blkhdgen_Index index, float curve);

typedef struct
{
    enum blkhdgen_ParameterType parameter_type; // blkhdgen_ParameterType_Envelope

    void* proc_data;

    blkhdgen_Envelope_GetRange get_range;
    
    // Transform an envelope value [min..max] to a normalized value [0..1]
    blkhdgen_NormalizePoint normalize;
    
    // Transform a normalized value [0..1] to an envelope value [min..max]
    blkhdgen_InverseNormalizePoint inverse_normalize;

    // Convert a non-normalized value to a display string
    // e.g. "50" -> "50%"
    //   or "1200" -> "1.2 kHz"
    //
    // The returned buffer remains valid until the next call to display_value or
    // until the generator is destroyed.
    blkhdgen_DisplayValue display_value;

    // Manipulate modulation points
    blkhdgen_Envelope_AddPoint add_point;
    blkhdgen_Envelope_RemovePoint remove_point;
    blkhdgen_Envelope_MovePoint move_point;
    blkhdgen_Envelope_Clear clear;

    // Set the curve [-1..1] for the specified point. This effects the connecting
    // modulation to the right of this point, if it has a neighboring point to the
    // right.
    //
    // This is currently not supported by Blockhead so it can be left unimplemented.
    blkhdgen_Envelope_SetPointCurve set_point_curve;
} blkhdgen_Envelope;

//
// Chord parameter
// Can be manipulated in Blockhead using the chord/scale/harmonics editor thing
//
typedef blkhdgen_Error (*blkhdgen_Chord_Set)(void* proc_data, blkhdgen_Index note, blkhdgen_Index beg, blkhdgen_Index end, blkhdgen_Bool on);

typedef struct
{
    enum blkhdgen_ParameterType parameter_type; // blkhdgen_ParameterType_Chord

    void* proc_data;

    blkhdgen_Chord_Set set;
} blkhdgen_Chord;

//
// Toggle parameter
// On/off value
//
typedef blkhdgen_Error (*blkhdgen_Toggle_Set)(void* proc_data, blkhdgen_Bool on);

typedef struct
{
    enum blkhdgen_ParameterType parameter_type; // blkhdgen_ParameterType_Toggle

    void* proc_data;

    blkhdgen_Toggle_Set set;
} blkhdgen_Toggle;

//
// Option parameter
// Will be displayed in Blockhead as a drop-down menu or radio buttons or something
//
typedef blkhdgen_Error (*blkhdgen_Option_Set)(void* proc_data, blkhdgen_Index value);
typedef const char* (*blkhdgen_Option_GetText)(void* proc_data, blkhdgen_Index value);

typedef struct
{
    enum blkhdgen_ParameterType parameter_type; // blkhdgen_ParameterType_Option
    blkhdgen_Range range;

    void* proc_data;

    blkhdgen_Option_Set set;

    // Returns the display text for the option index
    blkhdgen_Option_GetText get_text;
} blkhdgen_Option;

//
// Slider parameter
// Will be displayed in Blockhead as a slider or spinbox control
//
typedef blkhdgen_Error (*blkhdgen_Slider_Set)(void* proc_data, float value);

typedef struct
{
    enum blkhdgen_ParameterType parameter_type; // blkhdgen_ParameterType_Slider
    blkhdgen_Range range;

    void* proc_data;
    
    blkhdgen_NormalizePoint normalize;
    blkhdgen_InverseNormalizePoint inverse_normalize;
    blkhdgen_DisplayValue display_value;
    blkhdgen_Slider_Set set;
} blkhdgen_Slider;

union blkhdgen_ParameterObject
{
    blkhdgen_Chord chord;
    blkhdgen_Envelope envelope;
    blkhdgen_Option option;
    blkhdgen_Slider slider;
    blkhdgen_Toggle toggle;
};

typedef struct
{
    blkhdgen_ID id;

    // Zero if the parameter does not belong to a group
    blkhdgen_ID group_id;

    const char* name;
    int flags; // blkhdgen_ParameterFlags

    union blkhdgen_ParameterObject parameter;
} blkhdgen_Parameter;

typedef struct
{
    blkhdgen_ID id;
    const char* name;
} blkhdgen_Group;

typedef const char* (*blkhdgen_Generator_GetErrorString)(void* proc_data, blkhdgen_Error error);
typedef blkhdgen_Error (*blkhdgen_Generator_SetGetSampleInfoCB)(void* proc_data, void* user, blkhdgen_GetSampleInfoCB cb);
typedef blkhdgen_Error (*blkhdgen_Generator_SetGetSampleDataCB)(void* proc_data, void* user, blkhdgen_GetSampleDataCB cb);
typedef blkhdgen_Error (*blkhdgen_Generator_Process)(void* proc_data, const blkhdgen_Position* pos, float** out);
typedef blkhdgen_Position (*blkhd_Generator_GetWaveformPosition)(void* proc_data, blkhdgen_Position block_position);
typedef float (*blkhd_Generator_GetModValue)(void* proc_data, blkhdgen_Position block_position);
typedef blkhdgen_Index (*blkhd_Generator_AddWarpPoint)(void* proc_data, blkhdgen_IntPosition block_position);
typedef blkhdgen_Error (*blkhd_Generator_RemoveWarpPoint)(void* proc_data, blkhdgen_Index index);
typedef blkhdgen_Error (*blkhd_Generator_MoveWarpPoint)(void* proc_data, blkhdgen_Index index, blkhdgen_IntPosition new_position);
typedef blkhdgen_Error (*blkhd_Generator_ClearWarpPoints)(void* proc_data);

typedef struct
{
    const char* name;
    int num_groups;
    int num_params;
    blkhdgen_ChannelCount num_channels;

    void* proc_data;

    // Returned buffer remains valid until the next call to get_error_string or
    // until the generator is destroyed
    blkhdgen_Generator_GetErrorString get_error_string;

    // Set the callback for retrieving sample information
    blkhdgen_Generator_SetGetSampleInfoCB set_get_sample_info_cb;

    // Set the callback for retrieving sample data
    blkhdgen_Generator_SetGetSampleDataCB set_get_sample_data_cb;

    // pos is a buffer of length BLKHDGEN_VECTOR_SIZE containing block positions.
    //
    // Positions will usually be increasing linearly but may jump back in the case of
    // loop events.
    //
    // Be aware that Blockhead supports looping over extremely small regions (less
    // than BLKHDGEN_VECTOR_SIZE)
    blkhdgen_Generator_Process process;

    // Get the transformed waveform position for the given block position
    blkhd_Generator_GetWaveformPosition get_waveform_position;

    // Get the normalized modulation value [0..1] for the given block position
    blkhd_Generator_GetModValue get_mod_value;

    // Manipulate warp points
    blkhd_Generator_AddWarpPoint add_warp_point;
    blkhd_Generator_RemoveWarpPoint remove_warp_point;
    blkhd_Generator_MoveWarpPoint move_warp_point;
    blkhd_Generator_ClearWarpPoints clear_warp_points;
} blkhdgen_Generator;