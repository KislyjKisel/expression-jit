#pragma once
#include <string_view>
#include <string>
#include "concepts.h"

namespace evo
{
	template<typename T>
	concept StringKey = evo::AnyOf<std::decay_t<T>, std::string, std::string_view>;

	struct StringKeyHasher {
		using is_transparent = std::true_type;

		template<StringKey K>
		constexpr size_t operator()(const K& s) const noexcept {
			return std::hash<std::string_view>{}(std::string_view{s});
		}
	};

	struct StringKeyEqual {
		using is_transparent = std::true_type;

		template<StringKey K1, StringKey K2>
		constexpr bool operator()(const K1& a, const K2& b) const noexcept {
			return a == b;
		}
	};
}