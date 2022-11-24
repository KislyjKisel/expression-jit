#pragma once
#include <string_view>

namespace evo::cli
{
	std::pair<std::string_view, std::string_view> split(std::string_view s) {
		size_t i = 0;
		while (i < s.size() && s[i] == ' ') ++i;
		if (i == s.size()) return { {}, {} };
		s = s.substr(i);
		i = s.find_first_of(' ');
		if (i == std::string_view::npos) {
			return { s, {} };
		}
		return { s.substr(0, i), s.substr(i) };
	}
}