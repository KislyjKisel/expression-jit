#pragma once
#include <algorithm>
#include <concepts>
#include <numbers>
#include <iosfwd>
#include <cmath>
#include "math.h"
#include "vector3.h"

namespace evo
{
	template<std::floating_point T>
	struct Vector4 {
		T x;
		T y;
		T z;
		T w;

		Vector4() noexcept = default;
		constexpr explicit Vector4(T v) noexcept : x(v), y(v), z(v), w(v) { }
		constexpr Vector4(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w) { }
		template<std::floating_point U>
		constexpr Vector4(const Vector4<U> v) : x(v.x), y(v.y), z(v.z), w(v.w) { }

		constexpr static Vector4 Zero() noexcept {
			return Vector4(T(0), T(0), T(0), T(0));
		}

		constexpr Vector3<T> xyz() const noexcept { return { x, y, z }; }

		constexpr void reset() noexcept {
			x = y = z = w = T(0);
		}
		constexpr void fill(T v) noexcept {
			x = y = z = w = v;
		}
		constexpr Vector4 inversed_x() const noexcept {
			return Vector4(-x, y, z, w);
		}
		constexpr Vector4 inversed_y() const noexcept {
			return Vector4(x, -y, z, w);
		}
		constexpr Vector4 inversed_z() const noexcept {
			return Vector4(x, y, -z, w);
		}
		constexpr Vector4 inversed_w() const noexcept {
			return Vector4(x, y, z, -w);
		}
		constexpr static Vector4 X(const T x) noexcept {
			return Vector4(x, T(0), T(0), T(0));
		}
		constexpr static Vector4 Y(const T y) noexcept {
			return Vector4(T(0), y, T(0), T(0));
		}
		constexpr static Vector4 Z(const T z) noexcept {
			return Vector4(T(0), T(0), z, T(0));
		}
		constexpr static Vector4 W(const T w) noexcept {
			return Vector4(T(0), T(0), T(0), w);
		}
		constexpr static T Dot(const Vector4& l, const Vector4& r) noexcept {
			return l.x * r.x + l.y * r.y + l.z * r.z + l.w * r.w;
		}
		constexpr T length() const {
			return math::sqrt(x * x + y * y + z * z + w * w);
		}
		constexpr T sqrlen() const {
			return x * x + y * y + z * z + w * w;
		}
		constexpr void normalize() noexcept {
			T ilength = T(1.0l) / length();
			x *= ilength;
			y *= ilength;
			z *= ilength;
			w *= ilength;
		}
		constexpr Vector4 normalized() const noexcept {
			Vector4 v = *this;
			v.normalize();
			return v;
		}
		constexpr static Vector4 Lerp(const Vector4& a, const Vector4& b, const T t) noexcept {
			return Vector4(a.x + ( b.x - a.x ) * t, a.y + ( b.y - a.y ) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
		}
		constexpr static Vector4 Cerp(const Vector4& a, const Vector4& b, const T t) noexcept {
			T rt = T(0.5l) * ( T(1.0l) - math::cos(t * std::numbers::pi_v<T>) );
			return Vector4::Lerp(a, b, rt);
		}
		constexpr void clamp(const Vector4& vmin, const Vector4& vmax) noexcept {
			x = std::clamp(x, vmin.x, vmax.x);
			y = std::clamp(y, vmin.y, vmax.y);
			z = std::clamp(z, vmin.z, vmax.z);
			w = std::clamp(w, vmin.w, vmax.w);
		}
		constexpr static Vector4 Clamp(Vector4 v, const Vector4& vmin, const Vector4& vmax) noexcept {
			v.clamp(vmin, vmax);
			return v;
		}
		constexpr T min() const {
			return std::min(std::min(x, y), std::min(z, w));
		}
		constexpr static Vector4 Min(const Vector4& l, const Vector4& r) noexcept {
			return Vector4(std::min(l.x, r.x), std::min(l.y, r.y), std::min(l.z, r.z), std::min(l.w, r.w));
		}
		constexpr T max() const {
			return std::max(std::max(x, y), std::max(z, w));
		}
		constexpr static Vector4 Max(const Vector4& l, const Vector4& r) noexcept {
			return Vector4(std::max(l.x, r.x), std::max(l.y, r.y), std::max(l.z, r.z), std::max(l.w, r.w));
		}
		constexpr static std::pair<Vector4, Vector4> MinMax(const Vector4& a, const Vector4& b) noexcept {
			auto x_ = std::minmax(a.x, b.x);
			auto y_ = std::minmax(a.y, b.y);
			auto z_ = std::minmax(a.z, b.z);
			auto w_ = std::minmax(a.w, b.w);
			return { { x_.first, y_.first, z_.first, w_.first }, { x_.second, y_.second, z_.second, w_.second } };
		}
		constexpr void abs() noexcept {
			x = math::abs(x);
			y = math::abs(y);
			z = math::abs(z);
			w = math::abs(w);
		}
		constexpr static Vector4 Abs(Vector4 v) noexcept {
			v.abs();
			return v;
		}

		constexpr Vector4& operator+=(const Vector4& rhs) noexcept {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
			return *this;
		}
		constexpr Vector4& operator-=(const Vector4& rhs) noexcept {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
			return *this;
		}
		constexpr Vector4& operator*=(const Vector4& rhs) noexcept {
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
			w *= rhs.w;
			return *this;
		}
		constexpr Vector4& operator*=(const T rhs) noexcept {
			x *= rhs;
			y *= rhs;
			z *= rhs;
			w *= rhs;
			return *this;
		}
		constexpr Vector4& operator/=(const T rhs) noexcept {
			T irhs = T(1.0l) / rhs;
			return *this *= irhs;
		}
		constexpr Vector4& operator/=(const Vector4 v) noexcept {
			x /= v.x;
			y /= v.y;
			z /= v.z;
			w /= v.w;
			return *this;
		}

		constexpr bool operator<(const Vector4 v) const noexcept {
			return x < v.x && y < v.y && z < v.z && w < v.w;
		}
		constexpr bool operator<=(const Vector4 v) const noexcept {
			return x <= v.x && y <= v.y && z <= v.z && w <= v.w;
		}
		constexpr bool operator>(const Vector4 v) const noexcept {
			return x > v.x && y > v.y && z > v.z && w > v.w;
		}
		constexpr bool operator>=(const Vector4 v) const noexcept {
			return x >= v.x && y >= v.y && z >= v.z && w >= v.w;
		}
		constexpr bool operator==(const Vector4 v) const noexcept {
			return x == v.x && y == v.y && z == v.z && w == v.w;
		}
		constexpr bool operator!=(const Vector4 v) const noexcept {
			return x != v.x && y != v.y && z != v.z && w != v.w;
		}
	};

	using Vector4f = Vector4<float>;
	using Vector4d = Vector4<double>;
	using Vector4l = Vector4<long double>;

	template<std::floating_point T>
	constexpr Vector4<T> operator +(const Vector4<T>& l, const Vector4<T>& r) noexcept {
		return Vector4(l.x + r.x, l.y + r.y, l.z + r.z, l.w + r.w);
	}
	template<std::floating_point T>
	constexpr Vector4<T> operator -(const Vector4<T>& l, const Vector4<T>& r) noexcept {
		return Vector4(l.x - r.x, l.y - r.y, l.z - r.z, l.w - r.w);
	}
	template<std::floating_point T>
	constexpr Vector4<T> operator *(const Vector4<T>& l, const T r) noexcept {
		return Vector4(l.x * r, l.y * r, l.z * r, l.w * r);
	}
	template<std::floating_point T>
	constexpr Vector4<T> operator *(const T l, const Vector4<T>& r) noexcept {
		return Vector4(l * r.x, l * r.y, l * r.z, l * r.w);
	}
	template<std::floating_point T>
	constexpr Vector4<T> operator *(const Vector4<T>& l, const Vector4<T>& r) noexcept {
		return Vector4(l.x * r.x, l.y * r.y, l.z * r.z, l.w * r.w);
	}
	template<std::floating_point T>
	constexpr Vector4<T> operator /(const Vector4<T>& l, const T r) noexcept {
		T ir = T(1.0l) / r;
		return Vector4(l.x * ir, l.y * ir, l.z * ir, l.w * ir);
	}
	template<std::floating_point T>
	constexpr Vector4<T> operator /(const T l, const Vector4<T>& r) noexcept {
		return Vector4(l / r.x, l / r.y, l / r.z, l / r.w);
	}
	template<std::floating_point T>
	constexpr Vector4<T> operator +(const T l, const Vector4<T>& r) noexcept {
		return Vector4(l + r.x, l + r.y, l + r.z, l + r.w);
	}
	template<std::floating_point T>
	constexpr Vector4<T> operator +(const Vector4<T>& l, const T r) noexcept {
		return Vector4(l.x + r, l.y + r, l.z + r, l.w + r);
	}
	template<std::floating_point T>
	constexpr Vector4<T> operator -(const T l, const Vector4<T>& r) noexcept {
		return Vector4(l - r.x, l - r.y, l - r.z, l - r.w);
	}
	template<std::floating_point T>
	constexpr Vector4<T> operator -(const Vector4<T>& l, const T r) noexcept {
		return Vector4(l.x - r, l.y - r, l.z - r, l.w - r);
	}
	template<std::floating_point T>
	constexpr Vector4<T> operator /(const Vector4<T>& l, const Vector4<T>& r) noexcept {
		return Vector4(l.x / r.x, l.y / r.y, l.z / r.z, l.w / r.w);
	}
	template<std::floating_point T>
	constexpr Vector4<T> operator-(const Vector4<T>& v) noexcept {
		return Vector4(-v.x, -v.y, -v.z, -v.w);
	}
	
	template<std::floating_point T>
	std::ostream& operator<<(std::ostream& output, const Vector4<T>& v) noexcept;
	template<std::floating_point T>
	std::istream& operator>>(std::istream& input, Vector4<T>& v) noexcept;
}