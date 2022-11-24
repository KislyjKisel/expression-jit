#pragma once
#include <algorithm>
#include <numbers>
#include <iosfwd>
#include "color3.h"

namespace evo
{
	//todo: mix
	template<std::floating_point T>
	struct Color4 {
		T r;
		T g;
		T b;
		T a;

		Color4() noexcept = default;
		constexpr Color4(const T r, const T g, const T b, const T a = 1.f) noexcept : r(r), g(g), b(b), a(a) { }
		constexpr Color4(const Color3<T>& cl, const T alpha = 1.f) noexcept : r(cl.r), g(cl.g), b(cl.b), a(alpha) { }

		constexpr static Color4 White(const T alpha = 1.f) noexcept { return { 1.f, 1.f, 1.f, alpha }; }
		constexpr static Color4 Black(const T alpha = 1.f) noexcept { return { 0.f, 0.f, 0.f, alpha }; }
		constexpr static Color4 Transparent() noexcept { return { 0.f, 0.f, 0.f, 0.f }; }

		constexpr static Color4 HSV(const T h, const T s, const T v, const T alpha = 1.f) noexcept {
			return { Color3<T>::HSV(h, s, v), alpha };
		}
		constexpr static Color4 R(const T r) noexcept {
			return { r, 0.f, 0.f, 1.f };
		}
		constexpr static Color4 G(const T g) noexcept {
			return { 0.f, g, 0.f, 1.f };
		}
		constexpr static Color4 B(const T b) noexcept {
			return { 0.f, 0.f, b, 1.f };
		}
		constexpr static Color4 A(const T a) noexcept {
			return { 0.f, 0.f, 0.f, a };
		}

		constexpr Color3<T> rgb() const noexcept {
			return { r, g, b };
		}
		constexpr operator Color3<T>() const noexcept {
			return { r, g, b };
		}

		constexpr static Color4 Lerp(const Color4& a, const Color4& b, const T t) noexcept {
			return Color4(
				Color3<T>::Lerp(a, b, t),
				a.a + ( b.a - a.a ) * t
			);
		}
		constexpr static Color4 Cerp(const Color4& a, const Color4& b, const T t) noexcept {
			T rt = 0.5f * ( 1.0f - math::cos(t * std::numbers::pi_v<T>) );
			return Color4::Lerp(a, b, rt);
		}
		constexpr static T Dot(const Color4& a, const Color4& b) noexcept {
			return Color3<T>::Dot(a, b) + a.a * b.a;
		}
		constexpr void clamp(const Color4& vmin, const Color4& vmax) noexcept {
			*this = { Color3<T>::Clamp(*this, vmin, vmax), std::clamp(a, vmin.a, vmax.a) };
		}
		constexpr static Color4 Clamp(Color4 c, const Color4& cmin, const Color4& cmax) noexcept {
			c.clamp(cmin, cmax);
			return c;
		}
		constexpr T min() const noexcept {
			return std::min(Color3<T>(*this).min(), a);
		}
		constexpr static Color4 Min(const Color4& a, const Color4& b) noexcept {
			return Color4(Color3<T>::Min(a, b), std::min(a.a, b.a));
		}
		constexpr T max() const noexcept {
			return std::max(Color3<T>(*this).max(), a);
		}
		constexpr static Color4 Max(const Color4& a, const Color4& b) noexcept {
			return Color4(Color3<T>::Max(a, b), std::max(a.a, b.a));
		}
		constexpr static std::pair<Color4, Color4> MinMax(const Color4& a, const Color4& b) noexcept {
			auto rgb_ = Color3<T>::MinMax(a, b);
			auto a_ = std::minmax(a.a, b.a);
			return { { rgb_.first, a_.first }, { rgb_.second, a_.second } };
		}
		constexpr void abs() noexcept {
			*this = { Color3<T>::Abs(*this), math::abs(a) };
		}
		constexpr static Color4 Abs(Color4 c) noexcept {
			c.abs();
			return c;
		}

		constexpr bool operator==(const Color4&) const = default;
		constexpr bool operator!=(const Color4&) const = default;

		constexpr Color4& operator+=(const Color4& rhs) noexcept {
			r += rhs.r;
			g += rhs.g;
			b += rhs.b;
			a += rhs.a;
			return *this;
		}
		constexpr Color4& operator-=(const Color4& rhs) noexcept {
			r -= rhs.r;
			g -= rhs.g;
			b -= rhs.b;
			a -= rhs.a;
			return *this;
		}
		constexpr Color4& operator*=(const Color4& rhs) noexcept {
			r *= rhs.r;
			g *= rhs.g;
			b *= rhs.b;
			a *= rhs.a;
			return *this;
		}
		constexpr Color4& operator*=(const T rhs) noexcept {
			r *= rhs;
			g *= rhs;
			b *= rhs;
			a *= rhs;
			return *this;
		}
		constexpr Color4& operator/=(const T rhs) noexcept {
			T irhs = 1.0f / rhs;
			return *this *= irhs;
		}
	};

	using Color4f = Color4<float>;
	using Color4d = Color4<double>;
	using Color4l = Color4<long double>;

	template<std::floating_point T>
	constexpr Color4<T> operator+(const Color4<T>& l, const Color4<T>& r) {
		return Color4<T>(l.rgb() + r.rgb(), l.a + r.a);
	}
	template<std::floating_point T>
	constexpr Color4<T> operator-(const Color4<T>& l, const Color4<T>& r) {
		return Color4<T>(l.rgb() - r.rgb(), l.a - r.a);
	}
	template<std::floating_point T>
	constexpr Color4<T> operator*(const Color4<T>& l, T r) {
		return Color4<T>(l.rgb() * r, l.a * r);
	}
	template<std::floating_point T>
	constexpr Color4<T> operator*(T l, const Color4<T>& r) {
		return Color4<T>(l * r.rgb(), l * r.a);
	}
	template<std::floating_point T>
	constexpr Color4<T> operator*(const Color4<T>& l, const Color4<T>& r) {
		return Color4<T>(l.rgb() * r.rgb(), l.a * r.a);
	}
	template<std::floating_point T>
	constexpr Color4<T> operator/(const Color4<T>& l, T r) {
		T ir = 1.f / r;
		return Color4<T>(l.rgb() * ir, l.a * ir);
	}

	template<std::floating_point T>
	std::ostream& operator<<(std::ostream& output, const Color4<T>& v);
	template<std::floating_point T>
	std::istream& operator>>(std::istream& input, Color4<T>& v);
}