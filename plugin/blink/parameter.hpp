#pragma once

#include <string>
#include <blink.h>
#include "parameter_spec.hpp"

namespace blink {

class Parameter
{
public:

	Parameter(const ParameterSpec& spec)
		: uuid_(spec.uuid)
		, name_(spec.name)
	{}

	void set_group_index(int group_index) { group_index_ = group_index; }

	blink_UUID get_uuid() const { return uuid_; }
	int get_group_index() const { return group_index_; }
	const char* get_name() const { return name_.c_str(); }

	virtual blink_ParameterType get_type() const = 0;

private:

	blink_UUID uuid_;
	int group_index_ = -1;
	std::string name_;
};

}