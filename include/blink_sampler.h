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

typedef blink_FrameCount(*blink_GetSampleDataCB)(void* host, blink_ChannelCount channel, blink_FrameCount index, blink_FrameCount size, float* buffer);

//
// Sample Info
//
typedef struct
{
	blink_ID id;
	blink_ChannelCount num_channels;
	blink_FrameCount num_frames;

	// Array of 2 positions for the start and end of the loop.
	// Or null if no loop
	blink_FrameCount* loop_points;

	blink_SR SR;
	blink_BitDepth bit_depth;

	void* host;
	blink_GetSampleDataCB get_data;
} blink_SampleInfo;

typedef struct
{
	blink_IntPosition x, y;
	float value; // currently unused
} blink_WarpPoint;

typedef struct
{
	blink_Index count;
	blink_WarpPoint* points;
} blink_WarpPoints;

//
// Sampler Buffer Data
//
typedef struct
{
	uint64_t buffer_id; // increments for each audio buffer. instances should reset themselves
	                    // if they go at least one audio buffer without being processed i.e.
	                    // if (buffer_id > (previous_buffer_id + 1)) )
	                    // units should also reset themselves if they go at least one audio buffer
	                    // without being processed.

	blink_SR song_rate;
	blink_SampleInfo* sample_info;
	blink_Bool analysis_ready;
	blink_Position* positions;
} blink_SamplerBuffer;

//
// Draw Info
// This is passed in to blink_sampler_draw() as an output argument
// Blockhead may set any of the pointers to null depending on what data it needs from the plugin.
//
typedef struct
{
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

// Unit state is essentially state that changes in response to user input (e.g. parameter
// state).
struct blink_SamplerUnitState
{
	uint64_t id; // Increments by at least one every time the unit state changes.
	             // The same unit state might be passed in for multiple audio buffers, i.e.
	             // this id might not change from one buffer to the next.
	             // Could be used to optimize things like envelope traversals, for example
	             // if the parameter state has not changed since the last audio buffer then
				 // a plugin could continue searching from the previously hit envelope
	             // point instead of searching from the beginning

	int64_t data_offset;
	blink_ChannelMode channel_mode;
	const blink_WarpPoints* warp_points;
	const blink_ParameterData* parameter_data;
};

// output pointer is aligned on a 16-byte boundary
// output pointer is an array of size BLINK_VECTOR_SIZE * 2 for non-interleaved L and R channels 
typedef blink_Error(*blink_Sampler_Process)(void* proc_data, const blink_SamplerBuffer* buffer, const blink_SamplerUnitState* unit_state, float* out);

typedef struct
{
	void* proc_data;

	blink_Sampler_Process process;
} blink_SamplerUnit;

typedef blink_SamplerUnit(*blink_SamplerInstance_AddUnit)(void* proc_data);

typedef struct
{
	void* proc_data;

	blink_Instance_StreamInit stream_init;

	// Blockhead will call add_unit() four times per effect block to create a set
	// synchronized samplers for the purposes of crossfading between them to
	// avoid clicks.
	//
	// A crossfade between one or more units occurs whenever block data changes
	// or the song loops back to an earlier position. These two sitations may
	// occur simulataneously therefore Blockhead requires four units in total.
	blink_SamplerInstance_AddUnit add_unit;
} blink_SamplerInstance;

#ifdef BLINK_EXPORT
extern "C"
{
	EXPORTED blink_SamplerInstance blink_make_sampler_instance();

	// Free all memory associated with this sampler instance.
	EXPORTED blink_Error blink_destroy_sampler_instance(blink_SamplerInstance instance);

	// Returns true if Blockhead should enable warp markers. From the plugin's
	// perspective this just means warp data will be passed in to process().
	// It is up to the plugin to interpret this data.
	EXPORTED blink_Bool blink_sampler_enable_warp_markers();

	// Returns true if the plugin needs to preprocess samples in some way.
	// Preprocessing happens once per sample.
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

	// The host takes care of actually rendering the waveform but relies on
	// the plugin to calculate the waveform position at each pixel.
	EXPORTED blink_Error blink_sampler_draw(const blink_SamplerBuffer* buffer, const blink_SamplerUnitState* unit_state, blink_FrameCount n, blink_SamplerDrawInfo* out);
}
#endif
