#pragma once

#include <set>
#include <blink/plugin.hpp>
#include <blink/effect_instance.hpp>

namespace blink {

class EffectPlugin : public Plugin
{
public:

	~EffectPlugin();

	EffectInstance* add_instance();
	void destroy_instance(EffectInstance* instance);

private:

	virtual EffectInstance* make_instance() = 0;

	std::set<EffectInstance*> instances_;
};

inline EffectPlugin::~EffectPlugin()
{
	for (auto instance : instances_)
	{
		delete instance;
	}
}

inline EffectInstance* EffectPlugin::add_instance()
{
	const auto instance = make_instance();

	instances_.insert(instance);
	register_instance(instance);

	return instance;
}

inline void EffectPlugin::destroy_instance(EffectInstance* instance)
{
	instances_.erase(instance);
	unregister_instance(instance);
	delete instance;
}

} // tract