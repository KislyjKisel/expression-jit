#pragma once
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"

namespace evo
{
	namespace detail
	{
		template<std::floating_point T, unsigned Dimension>
		struct Vector_impl { /*using type = void;*/ };

		template<std::floating_point T> struct Vector_impl<T, 2> { using type = Vector2<T>; };
		template<std::floating_point T> struct Vector_impl<T, 3> { using type = Vector3<T>; };
		template<std::floating_point T> struct Vector_impl<T, 4> { using type = Vector4<T>; };
	}

	template<std::floating_point T, unsigned Dimension> using Vector = typename detail::Vector_impl<T, Dimension>::type;

	namespace math
	{
		template<std::floating_point T> constexpr bool is_zero(const Vector2<T> v) noexcept { 
			return is_zero(v.x) && is_zero(v.y); 
		}

		template<std::floating_point T> constexpr bool is_zero(const Vector3<T> v) noexcept {
			return is_zero(v.x) && is_zero(v.y) && is_zero(v.z);
		}

		template<std::floating_point T> constexpr bool is_zero(const Vector4<T> v) noexcept {
			return is_zero(v.x) && is_zero(v.y) && is_zero(v.z) && is_zero(v.w);
		}
	}
}