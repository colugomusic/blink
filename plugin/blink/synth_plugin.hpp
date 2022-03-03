#pragma once

#include <algorithm>
#include <memory>
#include <vector>
#include <blink/plugin.hpp>
#include <blink/synth_instance.hpp>

namespace blink {

class SynthPlugin : public Plugin
{
public:

	SynthInstance* add_instance();
	blink_Error destroy_instance(SynthInstance* instance);
	blink_Error destroy_instance(blink_SynthInstance && instance);

private:

	virtual auto make_instance() -> std::unique_ptr<blink::SynthInstance> = 0;

	std::vector<std::unique_ptr<SynthInstance>> instances_;
};

inline SynthInstance* SynthPlugin::add_instance()
{
	auto instance { make_instance() };

	register_instance(instance.get());

	return instances_.emplace_back(std::move(instance)).get();
}

inline blink_Error SynthPlugin::destroy_instance(SynthInstance* instance)
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

inline blink_Error SynthPlugin::destroy_instance(blink_SynthInstance && instance)
{
	return destroy_instance(static_cast<SynthInstance*>(instance.proc_data));
}

} // tract