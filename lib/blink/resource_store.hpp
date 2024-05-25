#pragma once

#include <cassert>
#include <map>
#include <string>
#include <vector>
#include <cmrc/cmrc.hpp>
#include <blink.h>

namespace blink {

struct ResourceStore {
	[[nodiscard]]
	auto get(std::string path) const -> blink_ResourceData {
		const auto pos = store_.find(path); 
		if (pos == store_.end()) {
			return { 0, 0 };
		}
		blink_ResourceData out;
		out.size = pos->second.size();
		out.data = pos->second.data();
		return out;
	}
	[[nodiscard]]
	auto has(std::string path) const -> bool {
		return store_.find(std::move(path)) != store_.end();
	}
	template <class File> [[nodiscard]]
	auto store(std::string path, const File& file) -> blink_ResourceData {
		assert(store_.find(path) == store_.end());
		std::vector<char> buffer(file.size());
		std::copy(file.begin(), file.end(), buffer.begin());
		store_[path] = std::move(buffer);
		return get(std::move(path));
	}
private:
	std::map<std::string, std::vector<char>> store_;
};

} // blink
