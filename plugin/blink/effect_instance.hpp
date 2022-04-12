#pragma once

#include <memory>
#include <vector>
#include "effect_unit.hpp"
#include "instance.hpp"

namespace blink {

class EffectInstance : public Instance
{
public:

	EffectInstance(Plugin* plugin);

	virtual ~EffectInstance();

	virtual auto get_info() const -> blink_EffectInstanceInfo;
	
	auto add_unit() -> EffectUnit*;

private:

	virtual auto make_unit() -> std::shared_ptr<EffectUnit> = 0;

	std::vector<std::shared_ptr<EffectUnit>> units_;
};

inline EffectInstance::EffectInstance(Plugin* plugin)
	: Instance(plugin)
{
}

inline EffectInstance::~EffectInstance()
{
	for (auto unit : units_)
	{
		unregister_unit(unit.get());
	}
}

inline auto EffectInstance::get_info() const -> blink_EffectInstanceInfo
{
	blink_EffectInstanceInfo out;

	// -1 means "unknown"
	out.approximate_delay = -1;
	out.exact_delay = -1;
	out.min_delay = -1;
	out.max_delay = -1;

	return out;
}

inline auto EffectInstance::add_unit() -> EffectUnit*
{
	const auto unit = make_unit();

	if (is_initialized())
	{
		unit->stream_init();
	}

	units_.push_back(unit);

	register_unit(unit.get());

	return unit.get();
}

} // blink