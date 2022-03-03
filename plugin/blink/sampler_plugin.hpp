#pragma once

#include <set>
#include <blink/plugin.hpp>
#include <blink/sampler_instance.hpp>

namespace blink {

class SamplerPlugin : public Plugin
{
public:

	SamplerInstance* add_instance();
	blink_Error destroy_instance(SamplerInstance* instance);
	blink_Error destroy_instance(blink_SamplerInstance && instance);

private:

	virtual auto make_instance() -> std::unique_ptr<blink::SamplerInstance> = 0;

	std::vector<std::unique_ptr<SamplerInstance>> instances_;
};

inline SamplerInstance* SamplerPlugin::add_instance()
{
	auto instance { make_instance() };

	register_instance(instance.get());

	return instances_.emplace_back(std::move(instance)).get();
}

inline blink_Error SamplerPlugin::destroy_instance(SamplerInstance* instance)
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

inline blink_Error SamplerPlugin::destroy_instance(blink_SamplerInstance && instance)
{
	return destroy_instance(static_cast<SamplerInstance*>(instance.proc_data));
}

} // tract