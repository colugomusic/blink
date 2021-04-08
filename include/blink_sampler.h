#pragma once 

#include "blink.h"

#define BLINK_SAMPLER

//
// Sample Preprocessing
//
typedef bool (*blink_Preprocess_ShouldAbort)(void* host);
typedef void (*blink_Preprocess_ReportProgress)(void* host, float progress);

typedef struct
{
	blink_Preprocess_ShouldAbort should_abort;
	blink_Preprocess_ReportProgress report_progress;
} blink_PreprocessCallbacks;

typedef blink_FrameCount(*blink_GetSampleDataCB)(void* host, blink_ChannelCount channel, blink_Index index, blink_FrameCount size, float* buffer);

//
// Sample Info
//
typedef struct
{
	blink_ID id;
	blink_ChannelCount num_channels;
	blink_FrameCount num_frames;
	blink_SR SR;
	blink_BitDepth bit_depth;
	blink_Bool analysis_ready;

	void* host;
	blink_GetSampleDataCB get_data;
} blink_SampleInfo;

//
// Sampler Buffer
//
typedef struct
{
	blink_SR song_rate;
	blink_SR sample_rate;
	int data_offset;
	blink_ChannelMode channel_mode;

	blink_SampleInfo* sample_info;
	blink_Position* positions;
	blink_WarpPoints* warp_points;
	blink_ParameterData* parameter_data;
} blink_SamplerBuffer;

typedef blink_Error (*blink_Sampler_GetWaveformPositions)(void* proc_data, const blink_Position* pos, float* out, float* derivatives);

// output pointer is aligned on a 16-byte boundary
// output pointer is an array of size BLINK_VECTOR_SIZE * 2 for non-interleaved L and R channels 
typedef blink_Error(*blink_Sampler_Process)(void* proc_data, const blink_SamplerBuffer* buffer, float* out);

typedef struct
{
	bool enable_warp_markers;

	void* proc_data;

	blink_Sampler_Process process;
} blink_Sampler;

#ifdef BLINK_EXPORT
extern "C"
{
	// Blockhead will call this multiple times per sampler block to create a set
	// synchronized samplers for the purposes of crossfading between them to avoid
	// clicks.
	// A crossfade between one or more samplers occurs whenever block data changes
	// or the song loops back to an earlier position.
	EXPORTED blink_Sampler blink_make_sampler();

	EXPORTED blink_Error blink_destroy_sampler(blink_Sampler sampler);

	EXPORTED blink_Bool blink_sampler_requires_preprocessing();

	// Called by the host once per sample only if
	// blink_sampler_requires_preprocessing() returns true
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

	// Get the transformed waveform positions and derivatives for the given
	// n block positions
	EXPORTED blink_Error blink_sampler_get_waveform_positions(const blink_SamplerBuffer* buffer, blink_FrameCount n, float* out, float* derivatives, float* amp);
}
#endif
