#pragma once

#include <set>
#include "unit.hpp"

namespace blink {

class Plugin;

class Instance
{
public:

	Instance(Plugin* plugin);

	virtual ~Instance() {}

	void begin_process(std::uint64_t buffer_id);
	void stream_init(blink_SR SR);

	Plugin* get_plugin() const { return plugin_; }
	blink_SR SR() const { return SR_; }

protected:

	void register_unit(Unit* unit);
	void unregister_unit(Unit* unit);

private:

	virtual void reset() {}
	virtual void stream_init() {}

	Plugin* plugin_;
	blink_SR SR_;
	std::set<Unit*> units_;
	std::uint64_t buffer_id_ = 0;
	int active_buffer_units_ = 0;
};

inline blink_SR get_SR(Instance* instance)
{
	return instance->SR();
}

inline Instance::Instance(Plugin* plugin)
	: plugin_(plugin)
{
}

inline void Instance::stream_init(blink_SR SR)
{
	SR_ = SR;

	stream_init();

	for (auto unit : units_)
	{
		unit->stream_init();
	}
}

inline void Instance::begin_process(std::uint64_t buffer_id)
{
	//
	// Instance::reset() is called at the start of the buffer if we have gone
	// at least one buffer with no units being processed
	//
	if (buffer_id > buffer_id_)
	{
		if (buffer_id > buffer_id_ + 1 || active_buffer_units_ == 0)
		{
			reset();
		}

		buffer_id_ = buffer_id;
		active_buffer_units_ = 0;
	}

	active_buffer_units_++;
}

inline void Instance::register_unit(Unit* unit)
{
	units_.insert(unit);
}

inline void Instance::unregister_unit(Unit* unit)
{
	units_.erase(unit);
}

} // blink