#pragma once

#include <algorithm>
#include <cassert>
#include <map>
#include <string>
#include <vector>
#include <cmrc/cmrc.hpp>
#include <blink.h>

namespace blink {

class ResourceStore
{
public:

	blink_ResourceData get(std::string path) const
	{
		const auto pos = store_.find(path);

		if (pos == store_.end()) return { 0, 0 };

		blink_ResourceData out;

		out.size = pos->second.size();
		out.data = pos->second.data();

		return out;
	}

	bool has(std::string path) const
	{
		return store_.find(path) != store_.end();
	}

	template <class File>
	blink_ResourceData store(std::string path, const File& file)
	{
		assert(store_.find(path) == store_.end());

		std::vector<char> buffer(file.size());

		std::copy(file.begin(), file.end(), buffer.begin());

		store_[path] = std::move(buffer);

		return get(path);
	}

private:

	std::map<std::string, std::vector<char>> store_;
};

} // blink
