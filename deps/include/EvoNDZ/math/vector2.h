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
	struct Vector2 {
		T x;
		T y;

		Vector2() noexcept = default;
		constexpr explicit Vector2(T v) noexcept : x(v), y(v) { };
		constexpr Vector2(T x, T y) noexcept : x(x), y(y) { };
		template<std::floating_point U>
		constexpr Vector2(const Vector2<U> v) : x(v.x), y(v.y) { }

		constexpr static Vector2 Zero() noexcept {
			return Vector2(T(0.0));
		}

		constexpr void reset() noexcept {
			x = y = T(0);
		}
		constexpr T rotation() const noexcept {
			return math::atan2(y, x);
		}
		constexpr Vector2 inversed_x() const noexcept {
			return Vector2(-x, y);
		}
		constexpr Vector2 inversed_y() const noexcept {
			return Vector2(x, -y);
		}
		constexpr static Vector2 X(const T x) noexcept {
			return Vector2(x, T(0));
		}
		constexpr static Vector2 Y(const T y) noexcept {
			return Vector2(T(0), y);
		}
		constexpr static Vector2 Unit(const T angle) noexcept {
			return Vector2(math::cos(angle), math::sin(angle));
		}
		constexpr static Vector2 LengthAngle(T length, T angle) noexcept {
			return Vector2(length * math::cos(angle), length * math::sin(angle));
		}
		constexpr void rotate(const T ang) noexcept {
			T cosa = math::cos(ang);
			T sina = math::sin(ang);
			T nx = x * cosa - y * sina;
			T ny = x * sina + y * cosa;
			x = nx;
			y = ny;
		}
		constexpr static Vector2 Rotate(Vector2 v, const T ang) noexcept {
			v.rotate(ang);
			return v;
		}
		constexpr static T Dot(const Vector2 l, const Vector2 r) noexcept {
			return l.x * r.x + l.y * r.y;
		}
		constexpr static T Cross(const Vector2 l, const Vector2 r) noexcept {
			return l.x * r.y - l.y * r.x;
		}
		constexpr T length() const {
			return math::sqrt(x * x + y * y);
		}
		constexpr T sqrlen() const {
			return x * x + y * y;
		}
		constexpr void normalize() noexcept {
			T ilength = T(1.0l) / math::sqrt(x * x + y * y);
			x *= ilength;
			y *= ilength;
		}
		constexpr Vector2 normalized() const noexcept {
			Vector2 v = *this;
			v.normalize();
			return v;
		}
		constexpr static Vector2 Lerp(const Vector2 a, const Vector2 b, T t) noexcept {
			return Vector2(a.x + ( b.x - a.x ) * t, a.y + ( b.y - a.y ) * t);
		}
		constexpr static Vector2 Cerp(const Vector2 a, const Vector2 b, const T t) noexcept {
			T rt = T(0.5l) * (T(1.0l) - math::cos(t * std::numbers::pi_v<T>));
			return Vector2::Lerp(a, b, rt);
		}
		/// <summary> Interpolates between p1 and p2, using p0 and p3. </summary>
		constexpr static Vector2 Cubic(const Vector2 p0, const Vector2 p1, const Vector2 p2, const Vector2 p3, T t) noexcept {
			T sqrT = t * t;
			T cubT = sqrT * t;
			T x0 = p3.x - p2.x - p0.x + p1.x;
			T x1 = p0.x - p1.x - x0;
			T x2 = p2.x - p0.x;
			T y0 = p3.y - p2.y - p0.y + p1.y;
			T y1 = p0.y - p1.y - y0;
			T y2 = p2.y - p0.y;
			return Vector2(
				x0 * cubT + x1 * sqrT + x2 * t + p1.x,
				y0 * cubT + y1 * sqrT + y2 * t + p1.y
			);
		}
		/// <summary> Interpolates between p1 and p2, using p0 and p3. </summary>
		constexpr static Vector2 Cubic(const Vector2* p, const T t) noexcept {
			T sqrT = t * t;
			T cubT = sqrT * t;
			T x0 = p[3].x - p[2].x - p[0].x + p[1].x;
			T x1 = p[0].x - p[1].x - x0;
			T x2 = p[2].x - p[0].x;
			T y0 = p[3].y - p[2].y - p[0].y + p[1].y;
			T y1 = p[0].y - p[1].y - y0;
			T y2 = p[2].y - p[0].y;
			return Vector2(
				x0 * cubT + x1 * sqrT + x2 * t + p[1].x,
				y0 * cubT + y1 * sqrT + y2 * t + p[1].y
			);
		}
		constexpr void clamp(const Vector2 vmin, const Vector2 vmax) noexcept {
			x = std::clamp(x, vmin.x, vmax.x);
			y = std::clamp(y, vmin.y, vmax.y);
		}
		constexpr static Vector2 Clamp(Vector2 v, const Vector2 vmin, const Vector2 vmax) noexcept {
			v.clamp(vmin, vmax);
			return v;
		}
		constexpr T min() const {
			return std::min(x, y);
		}
		constexpr static Vector2 Min(const Vector2 l, const Vector2 r) noexcept {
			return Vector2(std::min(l.x, r.x), std::min(l.y, r.y));
		}
		constexpr T max() const {
			return std::max(x, y);
		}
		constexpr static Vector2 Max(const Vector2 l, const Vector2 r) noexcept {
			return Vector2(std::max(l.x, r.x), std::max(l.y, r.y));
		}
		constexpr static std::pair<Vector2, Vector2> MinMax(const Vector2 a, const Vector2 b) noexcept {
			auto x_ = std::minmax(a.x, b.x);
			auto y_ = std::minmax(a.y, b.y);
			return { { x_.first, y_.first }, { x_.second, y_.second } };
		}
		constexpr void abs() noexcept {
			x = math::abs(x);
			y = math::abs(y);
		}
		constexpr static Vector2 Abs(Vector2 v) noexcept {
			v.abs();
			return v;
		}

		constexpr Vector2& operator+=(const Vector2 rhs) noexcept {
			x += rhs.x;
			y += rhs.y;
			return *this;
		}
		constexpr Vector2& operator-=(const Vector2 rhs) noexcept {
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}
		constexpr Vector2& operator*=(const Vector2 rhs) noexcept {
			x *= rhs.x;
			y *= rhs.y;
			return *this;
		}
		constexpr Vector2& operator*=(const T rhs) noexcept {
			x *= rhs;
			y *= rhs;
			return *this;
		}
		constexpr Vector2& operator/=(const T rhs) noexcept {
			T irhs = T(1.0l) / rhs;
			return *this *= irhs;
		}
		constexpr Vector2& operator/=(const Vector2 v) noexcept {
			x /= v.x;
			y /= v.y;
			return *this;
		}

		constexpr bool operator<(const Vector2 v) const noexcept {
			return x < v.x && y < v.y;
		}
		constexpr bool operator<=(const Vector2 v) const noexcept {
			return x <= v.x && y <= v.y;
		}
		constexpr bool operator>(const Vector2 v) const noexcept {
			return x > v.x && y > v.y;
		}
		constexpr bool operator>=(const Vector2 v) const noexcept {
			return x >= v.x && y >= v.y;
		}
		constexpr bool operator==(const Vector2 v) const noexcept {
			return x == v.x && y == v.y;
		}
		constexpr bool operator!=(const Vector2 v) const noexcept {
			return x != v.x && y != v.y;
		}
	};

	using Vector2f = Vector2<float>;
	using Vector2d = Vector2<double>;
	using Vector2l = Vector2<long double>;

	template<std::floating_point T>
	constexpr Vector2<T> operator +(const Vector2<T> l, const Vector2<T> r) noexcept {
		return Vector2(l.x + r.x, l.y + r.y);
	}
	template<std::floating_point T>
	constexpr Vector2<T> operator -(const Vector2<T> l, const Vector2<T> r) noexcept {
		return Vector2(l.x - r.x, l.y - r.y);
	}
	template<std::floating_point T>
	constexpr Vector2<T> operator *(const Vector2<T> l, const T r) noexcept {
		return Vector2(l.x * r, l.y * r);
	}
	template<std::floating_point T>
	constexpr Vector2<T> operator *(const T l, const Vector2<T> r) noexcept {
		return Vector2(l * r.x, l * r.y);
	}
	template<std::floating_point T>
	constexpr Vector2<T> operator *(const Vector2<T> l, const Vector2<T> r) noexcept {
		return Vector2(l.x * r.x, l.y * r.y);
	}
	template<std::floating_point T>
	constexpr Vector2<T> operator /(const Vector2<T> l, const T r) noexcept {
		T ir = T(1.0l) / r;
		return Vector2(l.x * ir, l.y * ir);
	}
	template<std::floating_point T>
	constexpr Vector2<T> operator/(const T v, const Vector2<T> k) noexcept {
		return Vector2(v / k.x, v / k.y);
	}
	template<std::floating_point T>
	constexpr Vector2<T> operator /(const Vector2<T> l, const Vector2<T> r) noexcept {
		return Vector2(l.x / r.x, l.y / r.y);
	}
	template<std::floating_point T>
	constexpr Vector2<T> operator +(const Vector2<T> l, const T r) noexcept {
		return Vector2(l.x + r, l.y + r);
	}
	template<std::floating_point T>
	constexpr Vector2<T> operator +(const T l, const Vector2<T> r) noexcept {
		return Vector2(l + r.x, l + r.y);
	}
	template<std::floating_point T>
	constexpr Vector2<T> operator -(const Vector2<T> l, const T r) noexcept {
		return Vector2(l.x - r, l.y - r);
	}
	template<std::floating_point T>
	constexpr Vector2<T> operator -(const T l, const Vector2<T> r) noexcept {
		return Vector2(l - r.x, l - r.y);
	}
	template<std::floating_point T>
	constexpr Vector2<T> operator -(const Vector2<T> v) noexcept {
		return Vector2(-v.x, -v.y);
	}
	
	template<std::floating_point T>
	std::ostream& operator<<(std::ostream& output, const Vector2<T> v) noexcept;
	template<std::floating_point T>
	std::istream& operator>>(std::istream& input, Vector2<T>& v) noexcept;
}