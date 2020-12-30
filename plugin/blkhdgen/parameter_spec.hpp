#pragma once

#include <blkhdgen/draft.h>

namespace blkhdgen {

struct ParameterSpec
{
	blkhdgen_UUID uuid;
	blkhdgen_ID group_id = 0;
	const char* name;
	int flags = 0;
};

}