#pragma once

#include <string>
#include <blink.h>
#include "parameter_spec.hpp"

namespace blink {

class Parameter
{
public:

	Parameter(const ParameterSpec& spec);

	blink_UUID get_uuid() const;
	blink_ID get_group_id() const;
	const char* get_name() const;

	virtual blink_ParameterType get_type() const = 0;

private:

	blink_UUID uuid_;
	blink_ID group_id_;
	std::string name_;
};

inline Parameter::Parameter(const ParameterSpec& spec)
	: uuid_(spec.uuid)
	, group_id_(spec.group_id)
	, name_(spec.name)
{
}

inline blink_UUID Parameter::get_uuid() const
{
	return uuid_;
}

inline blink_ID Parameter::get_group_id() const
{
	return group_id_;
}

inline const char* Parameter::get_name() const
{
	return name_.c_str();
}

}