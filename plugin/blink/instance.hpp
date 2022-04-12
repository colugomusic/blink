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

	auto begin_process(std::uint64_t buffer_id) -> void;
	auto stream_init(blink_SR SR) -> void;

	auto get_plugin() const { return plugin_; }
	auto SR() const { return SR_; }

protected:

	auto register_unit(Unit* unit) -> void;
	auto unregister_unit(Unit* unit) -> void;

	auto is_initialized() const { return SR_ > 0; }

private:

	virtual auto reset() -> void {}
	virtual auto stream_init() -> void {}

	Plugin* plugin_;
	blink_SR SR_ {};
	std::set<Unit*> units_;
	std::uint64_t buffer_id_ {};
	int active_buffer_units_ {};
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