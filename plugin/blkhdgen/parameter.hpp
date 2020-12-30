#pragma once

#include <string>
#include <blkhdgen/draft.h>

namespace blkhdgen {

class Parameter
{
public:

	Parameter(blkhdgen_UUID uuid, blkhdgen_ID group_id, std::string name, int flags);

	blkhdgen_UUID get_uuid() const;
	blkhdgen_ID get_group_id() const;
	const char* get_name() const;
	int get_flags() const;

	virtual blkhdgen_ParameterType get_type() const = 0;

private:

	blkhdgen_UUID uuid_;
	blkhdgen_ID group_id_;
	std::string name_;
	int flags_;
};

Parameter::Parameter(blkhdgen_UUID uuid, blkhdgen_ID group_id, std::string name, int flags)
	: uuid_(uuid)
	, group_id_(group_id)
	, name_(name)
	, flags_(flags)
{
}

blkhdgen_UUID Parameter::get_uuid() const
{
	return uuid_;
}

blkhdgen_ID Parameter::get_group_id() const
{
	return group_id_;
}

const char* Parameter::get_name() const
{
	return name_.c_str();
}

int Parameter::get_flags() const
{
	return flags_;
}

}