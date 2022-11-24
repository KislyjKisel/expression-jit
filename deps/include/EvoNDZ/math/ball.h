#pragma once
#include "segment.h"
#include "vector.h"

namespace evo::math
{
	template<std::floating_point T, unsigned Dimension>
	struct Ball {
		using vector_type = evo::Vector<T, Dimension>;
		using value_type = T;

		vector_type center;
		value_type radius;

		Ball() noexcept = default;
		constexpr Ball(const vector_type center, const value_type radius) noexcept : center(center), radius(radius) { }

		constexpr vector_type centroid() const noexcept {
			return center;
		}

		constexpr bool contains(const vector_type& p) const noexcept {
			return ( p - center ).sqrlen() <= sqr(radius);
		}
		constexpr bool contains(const Ball& c) const noexcept {
			return ( center - c.center ).length() + c.radius <= radius;
		}
		constexpr bool contains(const Segment<T, Dimension>& segm) const noexcept {
			return contains(segm.a, segm.b);
		}

		constexpr Ball enclose(const vector_type& p) const noexcept {
			return Ball(center, std::max(radius, (p - center).length()));
		}
		constexpr Ball enclose(const Segment<T, Dimension>& segm) const noexcept {
			return enclose(segm.a).enclose(segm.b);
		}
	};

	template<std::floating_point T>
	using Ball2 = Ball<T, 2>;

	using Ball2f = Ball2<float>;
	using Ball2d = Ball2<double>;
	using Ball2l = Ball2<long double>;

	template<std::floating_point T>
	using Ball3 = Ball<T, 3>;

	using Ball3f = Ball3<float>;
	using Ball3d = Ball3<double>;
	using Ball3l = Ball3<long double>;
}