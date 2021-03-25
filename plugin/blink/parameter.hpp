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
	int get_group_index() const;
	const char* get_name() const;

	virtual blink_ParameterType get_type() const = 0;

private:

	blink_UUID uuid_;
	int group_index_;
	std::string name_;
};

inline Parameter::Parameter(const ParameterSpec& spec)
	: uuid_(spec.uuid)
	, group_index_(spec.group_index)
	, name_(spec.name)
{
}

inline blink_UUID Parameter::get_uuid() const
{
	return uuid_;
}

inline int Parameter::get_group_index() const
{
	return group_index_;
}

inline const char* Parameter::get_name() const
{
	return name_.c_str();
}

}