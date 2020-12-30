#pragma once

#include <blkhdgen/draft.h>

namespace blkhdgen {

class Group
{
public:

	blkhdgen_ID get_id() const;
	const char* get_name() const;
};

}