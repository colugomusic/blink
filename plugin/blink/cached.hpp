#pragma once

#include <functional>

template <class T>
class Cached
{
public:

	Cached(std::function<T()> update_func)
		: update_func_(update_func)
	{}

	void set_dirty()
	{
		dirty_ = true;
	}

	bool is_dirty() const { return dirty_; }

	const T& get() const
	{
		if (dirty_)
		{
			object_ = update_func_();
			dirty_ = false;
		}

		return object_;
	}

	const T& operator*() const
	{
		return get();
	}

	const T* operator->() const
	{
		return &(get());
	}

private:

	mutable bool dirty_ = true;
	mutable T object_;

	std::function<T()> update_func_;
};