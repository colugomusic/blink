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

	virtual blink_EffectInstanceInfo get_info() const;
	
	EffectUnit* add_unit();

private:

	virtual std::shared_ptr<EffectUnit> make_unit() = 0;

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

inline blink_EffectInstanceInfo EffectInstance::get_info() const
{
	blink_EffectInstanceInfo out;

	// -1 means "unknown"
	out.approximate_delay = -1;
	out.exact_delay = -1;
	out.min_delay = -1;
	out.max_delay = -1;

	return out;
}

inline EffectUnit* EffectInstance::add_unit()
{
	const auto unit = make_unit();

	unit->stream_init();

	units_.push_back(unit);

	register_unit(unit.get());

	return unit.get();
}

} // blink