#pragma once

#include <blkhdgen/draft.h>

namespace blkhdgen {

class Parameter
{
public:
	blkhdgen_UUID get_uuid() const;
	blkhdgen_ID get_group_id() const;
	const char* get_name() const;
	int get_flags() const;

	virtual blkhdgen_ParameterType get_type() const = 0;
};

}