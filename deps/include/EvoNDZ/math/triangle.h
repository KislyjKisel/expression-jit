#pragma once
#include "vector.h"

namespace evo::math
{
	template<std::floating_point T, unsigned Dimension>
	struct Triangle {
		using vector_type = evo::Vector<T, Dimension>;

		vector_type a, b, c;

		Triangle() noexcept = default;
		constexpr Triangle(const vector_type& a, const vector_type& b, const vector_type& c) noexcept : a(a), b(b), c(c) { }

		constexpr vector_type centroid() const noexcept {
			return T(1.0l / 3.0l) * ( a + b + c );
		}
	};
}