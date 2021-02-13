#pragma once 

#include "blkhdgen.h"

#define BLKHDGEN_SAMPLER

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

//
// Sample Info
//
typedef struct
{
	blkhdgen_ID id;
	blkhdgen_ChannelCount num_channels;
	blkhdgen_FrameCount num_frames;
	blkhdgen_SR SR;
	blkhdgen_BitDepth bit_depth;
} blkhdgen_SampleInfo;

//
// Sampler Buffer
//
typedef struct
{
	blkhdgen_SR song_rate;
	blkhdgen_SR sample_rate;

	blkhdgen_SampleInfo* sample_info;
	blkhdgen_Position* positions;
	blkhdgen_WarpPoints* warp_points;
	blkhdgen_ParameterData* parameter_data;

	// TODO:

} blkhdgen_SamplerBuffer;

//typedef blkhdgen_Error (*blkhdgen_Sampler_SetGetSampleInfoCB)(void* proc_data, void* host, blkhdgen_GetSampleInfoCB cb);
//typedef blkhdgen_Error (*blkhdgen_Sampler_SetGetSampleDataCB)(void* proc_data, void* host, blkhdgen_GetSampleDataCB cb);
//typedef blkhdgen_Error (*blkhdgen_Sampler_SetGetWarpPointDataCB)(void* proc_data, void* host, blkhdgen_GetWarpPointDataCB cb);
//typedef blkhdgen_Error (*blkhdgen_Sampler_SetGetManipulatorDataCB)(void* proc_data, void* host, blkhdgen_GetManipulatorDataCB cb);
typedef blkhdgen_Error (*blkhdgen_Sampler_GetWaveformPositions)(void* proc_data, const blkhdgen_Position* pos, float* out, float* derivatives);

// output pointer is aligned on a 16-byte boundary
// output pointer is an array of size BLKHDGEN_VECTOR_SIZE * 2 for non-interleaved L and R channels 
typedef blkhdgen_Error(*blkhdgen_Sampler_Process)(void* proc_data, const blkhdgen_SamplerBuffer* buffer, float* out);

typedef struct
{
	//blkhdgen_GeneratorBase generator;

	void* proc_data;

	bool enable_warp_markers;

	// Host will call these once to set callbacks that the plugin uses to
	// retrieve data.
	//
	// It is the host's responsibility to ensure that the returned data remains
	// valid for the duration of the call.
	//
	// If the callback is called simultaneously from the GUI and audio threads
	// then the host may return two different pointers.
	//blkhdgen_Sampler_SetGetSampleInfoCB set_get_sample_info_cb;
	//blkhdgen_Sampler_SetGetSampleDataCB set_get_sample_data_cb;
	//blkhdgen_Sampler_SetGetWarpPointDataCB set_get_warp_point_data_cb;

	blkhdgen_Sampler_Process process;
} blkhdgen_Sampler;

#ifdef BLKHDGEN_EXPORT
extern "C"
{
	// Blockhead will call this twice per sampler block to create a pair of samplers
	// for the purposes of crossfading between them to avoid clicks.
	EXPORTED blkhdgen_Sampler blkhdgen_make_sampler();

	EXPORTED blkhdgen_Error blkhdgen_destroy_sampler(blkhdgen_Sampler sampler);

	EXPORTED blkhdgen_Bool blkhdgen_sampler_requires_preprocessing();

	// Called by the host once per sample if blkhdgen_sampler_requires_preprocessing() returns true
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
	// preprocessing has finished so the plugin needs to synchronize this
	// situation. It is ok to simply return silence until preprocessing has
	// completed.
	//
	// If the plugin allocates any memory for the active sample it should
	// be freed when the host calls blkhdgen_sampler_sample_deleted().
	EXPORTED blkhdgen_Error blkhdgen_sampler_preprocess_sample(void* host, blkhdgen_PreprocessCallbacks callbacks, const blkhdgen_SampleInfo* sample_info);

	// Called when the sample is deleted by Blockhead. The plugin should free
	// any data associated to the sample.
	//
	// Note that Blockhead may keep samples in memory after the user requests
	// their deletion so this function may not be called immediately.
	//
	// It is the host's responsibility to ensure that this function is not
	// called until the last call to process() has completed for this sample.
	EXPORTED blkhdgen_Error blkhdgen_sampler_sample_deleted(blkhdgen_ID sample_id);

	// Get the transformed waveform positions and derivatives for the given block
	// positions
	//
	// output pointers are aligned on 16-byte boundaries
	EXPORTED blkhdgen_Error blkhdgen_sampler_get_waveform_positions(const blkhdgen_ParameterData* parameter_data, int data_offset, const blkhdgen_Position* pos, float* out, float* derivatives);
}
#endif
