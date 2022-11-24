#pragma once
#include <concepts>
#include "line.h"

namespace evo::math
{
	template<std::floating_point T>
	struct Line2Equation {
		T a, b, c;

		constexpr Line2Equation(T a, T b, T c) noexcept : a(a), b(b), c(c) { }
		constexpr Line2Equation(const Line<T, 2>& line) noexcept {
			a = line.b.y - line.a.y;
			b = line.a.x - line.b.x;
			c = line.b.x * line.a.y - line.a.x * line.b.y;
		}

		constexpr T eval(const Vector2<T> p) const noexcept { return a * p.x + b * p.y + c; }
	};
}