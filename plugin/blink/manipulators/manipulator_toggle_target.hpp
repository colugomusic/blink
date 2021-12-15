#pragma once

#include "manipulator_target.hpp"
#include "manipulator_toggle_target_spec.hpp"

namespace blink {

class ManipulatorToggleTarget : public ManipulatorTarget
{
public:

	ManipulatorToggleTarget(ManipulatorToggleTargetSpec spec);

	blink_MT_Type get_type() const override { return blink_MT_Type_Toggle; }

	bool get_default_value() const { return default_value_; }

private:

	bool default_value_;
};

inline ManipulatorToggleTarget::ManipulatorToggleTarget(ManipulatorToggleTargetSpec spec)
	: ManipulatorTarget(spec)
	, default_value_(spec.default_value)
{
}

} // blink