#pragma once
#include "segment.h"
#include "ray.h"
#include "vector.h"

namespace evo::math
{
	template<std::floating_point T, unsigned Dimension>
	struct Line {
		using vector_type = evo::Vector<T, Dimension>;
		using value_type = T;

		vector_type a;
		vector_type b;

		Line() noexcept = default;
		constexpr Line(const vector_type a, const vector_type b) : a(a), b(b) { }
		constexpr explicit Line(const Segment<T, Dimension>& segment) : a(segment.a), b(segment.b) { }
		constexpr explicit Line(const Ray<T, Dimension>& ray) : a(ray.origin), b(ray.origin + ray.direction) { }

		constexpr static Line Axis(vector_type direction) noexcept { return { vector_type::Zero(), direction }; }
	};

	template<std::floating_point T>
	using Line2 = Line<T, 2>;

	using Line2f = Line2<float>;
	using Line2d = Line2<double>;
	using Line2l = Line2<long double>;

	template<std::floating_point T>
	using Line3 = Line<T, 3>;

	using Line3f = Line3<float>;
	using Line3d = Line3<double>;
	using Line3l = Line3<long double>;
}