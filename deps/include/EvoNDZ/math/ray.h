#pragma once
#include "vector.h"

namespace evo::math
{
	template<std::floating_point T, unsigned Dimension>
	struct Ray {
		using vector_type = evo::Vector<T, Dimension>;

		vector_type origin;
		vector_type direction;

		Ray() noexcept = default;
		constexpr Ray(const vector_type& origin, const vector_type& direction) noexcept
			: origin(origin), direction(direction) { }

		constexpr static Ray Axis(vector_type direction) noexcept { return { vector_type::Zero(), direction }; }

		constexpr vector_type at(const T t) const noexcept {
			return origin + direction * t;
		}

		constexpr Ray operator -() const noexcept {
			return Ray(origin, -direction);
		}
	};

	template<std::floating_point T>
	using Ray2 = Ray<T, 2>;

	using Ray2f = Ray2<float>;
	using Ray2d = Ray2<double>;
	using Ray2l = Ray2<long double>;

	template<std::floating_point T>
	using Ray3 = Ray<T, 3>;

	using Ray3f = Ray3<float>;
	using Ray3d = Ray3<double>;
	using Ray3l = Ray3<long double>;
}