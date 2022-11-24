#pragma once
#include <algorithm>
#include <concepts>
#include <numbers>
#include <iosfwd>
#include <cmath>
#include "math.h"

namespace evo
{
	template<std::floating_point T>
	struct Vector3 {
		T x;
		T y;
		T z;

		Vector3() noexcept = default;
		constexpr explicit Vector3(T v) noexcept : x(v), y(v), z(v) { }
		constexpr Vector3(T x, T y, T z) noexcept : x(x), y(y), z(z) { }
		template<std::floating_point U>
		constexpr Vector3(const Vector3<U> v) : x(v.x), y(v.y), z(v.z) { }

		constexpr static Vector3 Zero() noexcept {
			return Vector3(T(0), T(0), T(0));
		}
		constexpr static Vector3 UnitX() noexcept {
			return Vector3(T(1.0l), T(0), T(0));
		}
		constexpr static Vector3 UnitY() noexcept {
			return Vector3(T(0), T(1.0l), T(0));
		}
		constexpr static Vector3 UnitZ() noexcept {
			return Vector3(T(0), T(0), T(0));
		}
		constexpr static Vector3 UnitNX() noexcept {
			return Vector3(T(-1.0l), T(0), T(0));
		}
		constexpr static Vector3 UnitNY() noexcept {
			return Vector3(T(0), T(-1.0l), T(0));
		}
		constexpr static Vector3 UnitNZ() noexcept {
			return Vector3(T(0), T(0), T(-1.0l));
		}

		constexpr void reset() noexcept {
			x = y = z = T(0);
		}
		constexpr void fill(T v) noexcept {
			x = y = z = v;
		}
		constexpr Vector3 inversed_x() const noexcept {
			return Vector3(-x, y, z);
		}
		constexpr Vector3 inversed_y() const noexcept {
			return Vector3(x, -y, z);
		}
		constexpr Vector3 inversed_z() const noexcept {
			return Vector3(x, y, -z);
		}
		constexpr static Vector3 X(const T x) noexcept {
			return Vector3(x, T(0), T(0));
		}
		constexpr static Vector3 Y(const T y) noexcept {
			return Vector3(T(0), y, T(0));
		}
		constexpr static Vector3 Z(const T z) noexcept {
			return Vector3(T(0), T(0), z);
		}
		constexpr static T Dot(const Vector3& l, const Vector3& r) noexcept {
			return l.x * r.x + l.y * r.y + l.z * r.z;
		}
		constexpr static Vector3 Cross(const Vector3& l, const Vector3& r) noexcept {
			return Vector3(l.y * r.z - l.z * r.y, l.z * r.x - l.x * r.z, l.x * r.y - l.y * r.x);
		}
		constexpr T length() const {
			return math::sqrt(x * x + y * y + z * z);
		}
		constexpr T sqrlen() const {
			return x * x + y * y + z * z;
		}
		constexpr void normalize() noexcept {
			T ilength = T(1.0l) / math::sqrt(x * x + y * y + z * z);
			x *= ilength;
			y *= ilength;
			z *= ilength;
		}
		constexpr Vector3 normalized() const noexcept {
			Vector3 v = *this;
			v.normalize();
			return v;
		}
		constexpr static Vector3 Lerp(const Vector3& a, const Vector3& b, const T t) noexcept {
			return Vector3(a.x + ( b.x - a.x ) * t, a.y + ( b.y - a.y ) * t, a.z + (b.z - a.z) * t);
		}
		constexpr static Vector3 Cerp(const Vector3& a, const Vector3& b, const T t) noexcept {
			T rt = T(0.5l) * ( T(1.0l) - math::cos(t * std::numbers::pi_v<T>) );
			return Vector3::Lerp(a, b, rt);
		}
		constexpr void clamp(const Vector3& vmin, const Vector3& vmax) noexcept {
			x = std::clamp(x, vmin.x, vmax.x);
			y = std::clamp(y, vmin.y, vmax.y);
			z = std::clamp(z, vmin.z, vmax.z);
		}
		constexpr static Vector3 Clamp(Vector3 v, const Vector3& vmin, const Vector3& vmax) noexcept {
			v.clamp(vmin, vmax);
			return v;
		}
		constexpr T min() const {
			return std::min(std::min(x, y), z);
		}
		constexpr static Vector3 Min(const Vector3& l, const Vector3& r) noexcept {
			return Vector3(std::min(l.x, r.x), std::min(l.y, r.y), std::min(l.z, r.z));
		}
		constexpr T max() const {
			return std::max(std::max(x, y), z);
		}
		constexpr static Vector3 Max(const Vector3& l, const Vector3& r) noexcept {
			return Vector3(std::max(l.x, r.x), std::max(l.y, r.y), std::max(l.z, r.z));
		}
		constexpr static std::pair<Vector3, Vector3> MinMax(const Vector3& a, const Vector3& b) noexcept {
			auto x_ = std::minmax(a.x, b.x);
			auto y_ = std::minmax(a.y, b.y);
			auto z_ = std::minmax(a.z, b.z);
			return { { x_.first, y_.first, z_.first }, { x_.second, y_.second, z_.second } };
		}
		constexpr void abs() noexcept {
			x = math::abs(x);
			y = math::abs(y);
			z = math::abs(z);
		}
		constexpr static Vector3 Abs(Vector3 v) noexcept {
			v.abs();
			return v;
		}

		inline constexpr static Vector3 Reflect(const Vector3& v, const Vector3& n) noexcept;

		constexpr Vector3& operator+=(const Vector3& rhs) noexcept {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
		constexpr Vector3& operator-=(const Vector3& rhs) noexcept {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}
		constexpr Vector3& operator*=(const Vector3& rhs) noexcept {
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
			return *this;
		}
		constexpr Vector3& operator*=(const T rhs) noexcept {
			x *= rhs;
			y *= rhs;
			z *= rhs;
			return *this;
		}
		constexpr Vector3& operator/=(const T rhs) noexcept {
			T irhs = T(1.0l) / rhs;
			return *this *= irhs;
		}
		constexpr Vector3& operator/=(const Vector3 v) noexcept {
			x /= v.x;
			y /= v.y;
			z /= v.z;
			return *this;
		}

		constexpr bool operator<(const Vector3 v) const noexcept {
			return x < v.x && y < v.y && z < v.z;
		}
		constexpr bool operator<=(const Vector3 v) const noexcept {
			return x <= v.x && y <= v.y && z <= v.z;
		}
		constexpr bool operator>(const Vector3 v) const noexcept {
			return x > v.x && y > v.y && z > v.z;
		}
		constexpr bool operator>=(const Vector3 v) const noexcept {
			return x >= v.x && y >= v.y && z >= v.z;
		}
		constexpr bool operator==(const Vector3 v) const noexcept {
			return x == v.x && y == v.y && z == v.z;
		}
		constexpr bool operator!=(const Vector3 v) const noexcept {
			return x != v.x && y != v.y && z != v.z;
		}
	};

	using Vector3f = Vector3<float>;
	using Vector3d = Vector3<double>;
	using Vector3l = Vector3<long double>;

	template<std::floating_point T>
	constexpr Vector3<T> operator +(const Vector3<T>& l, const Vector3<T>& r) noexcept {
		return Vector3(l.x + r.x, l.y + r.y, l.z + r.z);
	}
	template<std::floating_point T>
	constexpr Vector3<T> operator -(const Vector3<T>& l, const Vector3<T>& r) noexcept {
		return Vector3(l.x - r.x, l.y - r.y, l.z - r.z);
	}
	template<std::floating_point T>
	constexpr Vector3<T> operator *(const Vector3<T>& l, const T r) noexcept {
		return Vector3(l.x * r, l.y * r, l.z * r);
	}
	template<std::floating_point T>
	constexpr Vector3<T> operator *(const T l, const Vector3<T>& r) noexcept {
		return Vector3(l * r.x, l * r.y, l * r.z);
	}
	template<std::floating_point T>
	constexpr Vector3<T> operator *(const Vector3<T>& l, const Vector3<T>& r) noexcept {
		return Vector3(l.x * r.x, l.y * r.y, l.z * r.z);
	}
	template<std::floating_point T>
	constexpr Vector3<T> operator /(const Vector3<T>& l, const T r) noexcept {
		T ir = T(1.0l) / r;
		return Vector3(l.x * ir, l.y * ir, l.z * ir);
	}
	template<std::floating_point T>
	constexpr Vector3<T> operator /(const T l, const Vector3<T>& r) noexcept {
		return Vector3(l / r.x, l / r.y, l / r.z);
	}
	template<std::floating_point T>
	constexpr Vector3<T> operator +(const T l, const Vector3<T>& r) noexcept {
		return Vector3(l + r.x, l + r.y, l + r.z);
	}
	template<std::floating_point T>
	constexpr Vector3<T> operator +(const Vector3<T>& l, const T r) noexcept {
		return Vector3(l.x + r, l.y + r, l.z + r);
	}
	template<std::floating_point T>
	constexpr Vector3<T> operator -(const T l, const Vector3<T>& r) noexcept {
		return Vector3(l - r.x, l - r.y, l - r.z);
	}
	template<std::floating_point T>
	constexpr Vector3<T> operator -(const Vector3<T>& l, const T r) noexcept {
		return Vector3(l.x - r, l.y - r, l.z - r);
	}
	template<std::floating_point T>
	constexpr Vector3<T> operator /(const Vector3<T>& l, const Vector3<T>& r) noexcept {
		return Vector3(l.x / r.x, l.y / r.y, l.z / r.z);
	}
	template<std::floating_point T>
	constexpr Vector3<T> Vector3<T>::Reflect(const Vector3<T>& v, const Vector3<T>& n) noexcept {
		return v - ( T(2.0l) * Vector3::Dot(v, n) ) * n;
	}
	template<std::floating_point T>
	constexpr Vector3<T> operator-(const Vector3<T>& v) noexcept {
		return Vector3(-v.x, -v.y, -v.z);
	}
	
	template<std::floating_point T>
	std::ostream& operator<<(std::ostream& output, const Vector3<T>& v) noexcept;
	template<std::floating_point T>
	std::istream& operator>>(std::istream& input, Vector3<T>& v) noexcept;
}