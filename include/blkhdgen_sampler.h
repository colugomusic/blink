#pragma once 

#include "blkhdgen.h"

#define BLKHDGEN_SAMPLER

typedef blkhdgen_Error (*blkhdgen_Sampler_SampleDeleted)(void* proc_data, blkhdgen_ID sample_id);
typedef blkhdgen_Error (*blkhdgen_Sampler_SetGetSampleInfoCB)(void* proc_data, void* host, blkhdgen_GetSampleInfoCB cb);
typedef blkhdgen_Error (*blkhdgen_Sampler_SetGetSampleDataCB)(void* proc_data, void* host, blkhdgen_GetSampleDataCB cb);
typedef blkhdgen_Error (*blkhdgen_Sampler_SetGetWarpPointDataCB)(void* proc_data, void* host, blkhdgen_GetWarpPointDataCB cb);
typedef blkhdgen_Error (*blkhdgen_Sampler_SetGetManipulatorDataCB)(void* proc_data, void* host, blkhdgen_GetManipulatorDataCB cb);
typedef blkhdgen_Error (*blkhdgen_Sampler_GetWaveformPositions)(void* proc_data, const blkhdgen_Position* pos, float* out, float* derivatives);
typedef blkhdgen_Error (*blkhdgen_Sampler_PreprocessSample)(void* proc_data, void* host, blkhdgen_PreprocessCallbacks callbacks);

typedef struct
{
	blkhdgen_GeneratorBase generator;

	void* proc_data;

	bool requires_preprocess;
	bool enable_warp_markers;

	// Host will call these once to set callbacks that the plugin uses to
	// retrieve data.
	//
	// It is the host's responsibility to ensure that the returned data remains
	// valid for the duration of the call.
	//
	// If the callback is called simultaneously from the GUI and audio threads
	// then the host may return two different pointers.
	blkhdgen_Sampler_SetGetSampleInfoCB set_get_sample_info_cb;
	blkhdgen_Sampler_SetGetSampleDataCB set_get_sample_data_cb;
	blkhdgen_Sampler_SetGetWarpPointDataCB set_get_warp_point_data_cb;

	blkhdgen_Generator_Process process;

	// Get the transformed waveform positions and derivatives for the given block
	// positions
	//
	// output pointers are aligned on 16-byte boundaries
	blkhdgen_Sampler_GetWaveformPositions get_waveform_positions;

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
	// The process() function may be called in the audio thread before
	// preprocessing has finished so the plugin needs to synchronize this
	// situation. It is ok to simply return silence until preprocessing has
	// completed.
	//
	// If the plugin allocates any memory for the active sample it should
	// be freed when the host calls sample_deleted().
	blkhdgen_Sampler_PreprocessSample preprocess_sample;

	// Called when the sample is deleted by Blockhead. The plugin should free
	// any data associated to the sample.
	//
	// Note that Blockhead may keep samples in memory after the user requests
	// their deletion so this function may not be called immediately.
	//
	// It is the host's responsibility to ensure that this function is not
	// called until the last call to process() has completed for this sample.
	blkhdgen_Sampler_SampleDeleted sample_deleted;
} blkhdgen_Sampler;

#ifdef BLKHDGEN_EXPORT
extern "C"
{
	EXPORTED blkhdgen_Sampler blkhdgen_make_sampler();
	EXPORTED blkhdgen_Error blkhdgen_destroy_sampler(blkhdgen_Sampler sampler);
}
#endif
