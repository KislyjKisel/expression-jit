#pragma once
#include <concepts>

namespace evo
{
	template<std::signed_integral T>
	struct Point {
		T x; 
		T y;

		Point() noexcept = default;
		constexpr Point(T x, T y) noexcept : x(x), y(y) { }
	};
}