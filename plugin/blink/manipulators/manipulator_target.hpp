#pragma once

#include <optional>
#include <string>
#include "manipulator_target_spec.hpp"

namespace blink {

class ManipulatorTarget
{
public:

	ManipulatorTarget(const ManipulatorTargetSpec& spec)
		: uuid_(spec.uuid)
		, name_(spec.name)
	{
		if (spec.short_name) short_name_ = spec.short_name;
		if (spec.long_desc) long_desc_ = spec.long_desc;
	}

	void set_group_index(int group_index) { group_index_ = group_index; }

	blink_UUID get_uuid() const { return uuid_; }
	int get_group_index() const { return group_index_; }
	const char* get_name() const { return name_.c_str(); }
	const char* get_short_name() const { return short_name_ ? short_name_->c_str() : nullptr; }
	const char* get_long_desc() const { return long_desc_ ? long_desc_->c_str() : nullptr; }

	virtual blink_MT_Type get_type() const = 0;

private:

	blink_UUID uuid_;
	int group_index_ = -1;
	std::string name_;
	std::optional<std::string> short_name_;
	std::optional<std::string> long_desc_;
};

} // blink
