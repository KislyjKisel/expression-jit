#pragma once
#include "vector.h"

namespace evo::math
{
	template<std::floating_point T, unsigned Dimension>
	struct Segment {
		using vector_type = evo::Vector<T, Dimension>;
		using value_type = T;

		vector_type a;
		vector_type b;

		Segment() noexcept = default;
		constexpr Segment(const vector_type a, const vector_type b) : a(a), b(b) { }

		const vector_type centroid() const noexcept {
			return ( a + b ) * T(0.5);
		}
	};

	template<typename T>
	using Segment2 = Segment<T, 2>;

	using Segment2f = Segment2<float>;
	using Segment2d = Segment2<double>;
	using Segment2l = Segment2<long double>;

	template<typename T>
	using Segment3 = Segment<T, 3>;

	using Segment3f = Segment3<float>;
	using Segment3d = Segment3<double>;
	using Segment3l = Segment3<long double>;
}