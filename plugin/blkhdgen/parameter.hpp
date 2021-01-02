#pragma once

#include <string>
#include <blkhdgen.h>
#include "parameter_spec.hpp"

namespace blkhdgen {

class Parameter
{
public:

	Parameter(const ParameterSpec& spec);

	blkhdgen_UUID get_uuid() const;
	blkhdgen_ID get_group_id() const;
	const char* get_name() const;

	virtual blkhdgen_ParameterType get_type() const = 0;

private:

	blkhdgen_UUID uuid_;
	blkhdgen_ID group_id_;
	std::string name_;
};

Parameter::Parameter(const ParameterSpec& spec)
	: uuid_(spec.uuid)
	, group_id_(spec.group_id)
	, name_(spec.name)
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

}