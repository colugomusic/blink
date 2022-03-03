#pragma once

#include <algorithm>
#include <memory>
#include <vector>
#include <blink/plugin.hpp>
#include <blink/effect_instance.hpp>

namespace blink {

class EffectPlugin : public Plugin
{
public:

	EffectInstance* add_instance();
	blink_Error destroy_instance(EffectInstance* instance);
	blink_Error destroy_instance(blink_EffectInstance && instance);

private:

	virtual auto make_instance() -> std::unique_ptr<EffectInstance> = 0;

	std::vector<std::unique_ptr<EffectInstance>> instances_;
};

inline EffectInstance* EffectPlugin::add_instance()
{
	auto instance { make_instance() };

	register_instance(instance.get());

	return instances_.emplace_back(std::move(instance)).get();
}

inline blink_Error EffectPlugin::destroy_instance(EffectInstance* instance)
{
	const auto pos { std::find_if(instances_.begin(), instances_.end(), [instance](auto && ptr)
	{
		return ptr.get() == instance;
	})};

	if (pos == instances_.end()) return blink_StdError_InvalidInstance;

	unregister_instance(instance);
	instances_.erase(pos);

	return BLINK_OK;
}

inline blink_Error EffectPlugin::destroy_instance(blink_EffectInstance && instance)
{
	return destroy_instance(static_cast<EffectInstance*>(instance.proc_data));
}

} // tract