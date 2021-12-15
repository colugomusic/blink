#pragma once

#include "manipulator_target.hpp"
#include "manipulator_option_target_spec.hpp"

namespace blink {

class ManipulatorOptionTarget : public ManipulatorTarget
{
public:

	ManipulatorOptionTarget(ManipulatorOptionTargetSpec spec);

	blink_MT_Type get_type() const override { return blink_MT_Type_Option; }

	const char* get_text(blink_Index index) const;
	blink_Index get_max_index() const { return blink_Index(options_.size() - 1); }
	blink_Index get_default_index() const { return default_index_; }

private:

	std::vector<std::string> options_;
	blink_Index default_index_;
};

inline ManipulatorOptionTarget::ManipulatorOptionTarget(ManipulatorOptionTargetSpec spec)
	: ManipulatorTarget(spec)
	, options_(spec.options)
	, default_index_(spec.default_index)
{
}

inline const char* ManipulatorOptionTarget::get_text(blink_Index index) const
{
	return options_[index].c_str();
}

} // blink