#pragma once

#include <optional>
#include <string>
#include <blink.h>
#include "parameter_spec.hpp"

namespace blink {

class Parameter
{
public:

	const blink_UUID uuid;
	const blink_Index manipulation_delegate;

	Parameter(const ParameterSpec& spec)
		: uuid(spec.uuid)
		, name_(spec.name)
		, manipulation_delegate(spec.manipulation_delegate)
	{
		if (spec.short_name) short_name_ = spec.short_name;
		if (spec.long_desc) long_desc_ = spec.long_desc;
	}

	void set_group_name(std::string group_name) { group_name_ = group_name; }

	const char* get_group_name() const { return group_name_.c_str(); }
	const char* get_name() const { return name_.c_str(); }
	const char* get_short_name() const { return short_name_ ? short_name_->c_str() : nullptr; }
	const char* get_long_desc() const { return long_desc_ ? long_desc_->c_str() : nullptr; }

	virtual blink_ParameterType get_type() const = 0;

private:

	int group_index_ = -1;
	std::string group_name_;
	std::string name_;
	std::optional<std::string> short_name_;
	std::optional<std::string> long_desc_;
	std::optional<std::string> manipulation_delegate_;
};

}