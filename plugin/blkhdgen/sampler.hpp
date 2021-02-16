#pragma once

#include <map>
#include <blkhdgen_sampler.h>
#include "envelope_spec.hpp"
#include "group.hpp"
#include "parameter.hpp"
#include "sample_data.hpp"
#include "slider_spec.hpp"
#include "generator_base.hpp"

namespace blkhdgen {

class Sampler : public GeneratorBase
{

public:

	virtual ~Sampler() {}

	virtual blkhdgen_Error process(const blkhdgen_SamplerBuffer* buffer, float* out) = 0;
	virtual blkhdgen_Error preprocess_sample(void* host, blkhdgen_PreprocessCallbacks callbacks) const = 0;

private:

	std::function<blkhdgen_WarpPoints*()> get_warp_point_data_;
};

}