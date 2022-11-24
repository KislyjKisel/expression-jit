#pragma once
#include <algorithm>
#include <numbers>
#include <iosfwd>
#include "../math/math.h"

namespace evo
{
	template<std::floating_point T>
	struct Color3 {
		T r;
		T g;
		T b;

		Color3() noexcept = default;
		constexpr Color3(const T r, const T g, const T b) noexcept : r(r), g(g), b(b) { }
		constexpr explicit Color3(const T value) noexcept : r(value), g(value), b(value) { }
		constexpr Color3(unsigned char r, unsigned char g, unsigned char b) 
			: r(r * m_inv255), g(g * m_inv255), b(b * m_inv255) { }

		constexpr static Color3 White() noexcept { return { 1.f, 1.f, 1.f }; }
		constexpr static Color3 Black() noexcept { return { 0.f, 0.f, 0.f }; }

		/// <summary> https://stackoverflow.com/a/6930407 </summary>
		/// <param name="h"> hue        [0, 2pi] </param>
		/// <param name="s"> saturation [0,   1] </param>
		/// <param name="v"> value      [0,   1] </param>
		constexpr static Color3 HSV(const T h, const T s, const T v) noexcept {
			const T h_ = h * ( 3.f * std::numbers::inv_pi_v<T> ); //H / 60deg
			const int i = int(h_);
			const T ff = h_ - i;
			const T p = v * ( 1.f - s );
			//const T q = v * ( 1.f - s * ff );
			//const T t = v * ( 1.f - s * ( 1.f - ff ) );
			
			//0, 2, 4 - t; 1, 3, 5 - q

			const T qt = i & 1 ? ( v * ( 1.f - s * ff ) ) : ( v * ( 1.f - s * ( 1.f - ff ) ) );

			switch (i) {
				case 0:
					return { v, qt, p };
				case 1:
					return { qt, v, p };
				case 2:
					return { p, v, qt };
				case 3:
					return { p, qt, v };
				case 4:
					return { qt, p, v };
				default:
					return { v, p, qt };
			}

		}
		constexpr static Color3 R(const T r = 1.f) noexcept {
			return { r, 0.f, 0.f };
		}
		constexpr static Color3 G(const T g = 1.f) noexcept {
			return { 0.f, g, 0.f };
		}
		constexpr static Color3 B(const T b = 1.f) noexcept {
			return { 0.f, 0.f, b };
		}

		/// <summary> https://stackoverflow.com/a/6930407 </summary>
		/// <param name="h"> hue        [0, 2pi] </param>
		/// <param name="s"> saturation [0,   1] </param>
		/// <param name="v"> value      [0,   1] </param>
		constexpr void to_hsv(T& h, T& s, T& v) const noexcept {
			const T m = min(), M = max();
			v = M;
			T delta = M - m;
			if (delta < 1e-5f) {
				s = 0.f;
				h = 0.f; // ind
				return;
			}
			if (M > 0.f) {
				s = delta / M;
			}
			else {
				s = 0.f;
				h = std::numeric_limits<T>::quiet_NaN();
				return;
			}
			T h_;
			if (r >= M) {
				h_ = ( g - b ) / delta;
			}
			else if (g >= M) {
				h_ = 2.f + ( b - r ) / delta;
			}
			else h_ = 4.f + ( r - g ) / delta;

			h = h_ * std::numbers::pi_v<T> / 3.f;
			if (h < 0.f) h += 2.f * std::numbers::pi_v<T>;
		}
		constexpr T luminance() const noexcept {
			return 0.2126f * r + 0.7152f * g + 0.0722f * b;
		}
		constexpr void fill(const T value) noexcept { 
			r = g = b = value;
		}
		constexpr static Color3 Lerp(const Color3& a, const Color3& b, const T t) noexcept {
			return Color3(
				a.r + ( b.r - a.r ) * t,
				a.g + ( b.g - a.g ) * t,
				a.b + ( b.b - a.b ) * t
			);
		}
		constexpr static Color3 Cerp(const Color3& a, const Color3& b, const T t) noexcept {
			T rt = 0.5f * ( 1.0f - math::cos(t * std::numbers::pi_v<T>) );
			return Color3::Lerp(a, b, rt);
		}
		constexpr static T Dot(const Color3& a, const Color3& b) noexcept {
			return a.r * b.r + a.g * b.g + a.b * b.b;
		}
		constexpr void clamp(const Color3& vmin, const Color3& vmax) noexcept {
			r = std::clamp(r, vmin.r, vmax.r);
			g = std::clamp(g, vmin.g, vmax.g);
			b = std::clamp(b, vmin.b, vmax.b);
		}
		constexpr static Color3 Clamp(Color3 c, const Color3& cmin, const Color3& cmax) noexcept {
			c.clamp(cmin, cmax);
			return c;
		}
		constexpr T min() const noexcept {
			return std::min(std::min(r, g), b);
		}
		constexpr static Color3 Min(const Color3& a, const Color3& b) noexcept {
			return Color3(std::min(a.r, b.r), std::min(a.g, b.g), std::min(a.b, b.b));
		}
		constexpr T max() const noexcept {
			return std::max(std::max(r, g), b);
		}
		constexpr static Color3 Max(const Color3& a, const Color3& b) noexcept {
			return Color3(std::max(a.r, b.r), std::max(a.g, b.g), std::max(a.b, b.b));
		}
		constexpr static std::pair<Color3, Color3> MinMax(const Color3& a, const Color3& b) noexcept {
			auto r_ = std::minmax(a.r, b.r);
			auto g_ = std::minmax(a.g, b.g);
			auto b_ = std::minmax(a.b, b.b);
			return { { r_.first, g_.first, b_.first }, { r_.second, g_.second, b_.second } };
		}
		constexpr void abs() noexcept {
			r = math::abs(r);
			g = math::abs(g);
			b = math::abs(b);
		}
		constexpr static Color3 Abs(Color3 c) noexcept {
			c.abs();
			return c;
		}

		constexpr Color3& operator+=(const Color3& rhs) noexcept {
			r += rhs.r;
			g += rhs.g;
			b += rhs.b;
			return *this;
		}
		constexpr Color3& operator-=(const Color3& rhs) noexcept {
			r -= rhs.r;
			g -= rhs.g;
			b -= rhs.b;
			return *this;
		}
		constexpr Color3& operator*=(const Color3& rhs) noexcept {
			r *= rhs.r;
			g *= rhs.g;
			b *= rhs.b;
			return *this;
		}
		constexpr Color3& operator*=(const T rhs) noexcept {
			r *= rhs;
			g *= rhs;
			b *= rhs;
			return *this;
		}
		constexpr Color3& operator/=(const T rhs) noexcept {
			T irhs = 1.0f / rhs;
			return *this *= irhs;
		}

	private:
		inline static constexpr T m_inv255 = 1.0f / 255.0f;
	};

	using Color3f = Color3<float>;
	using Color3d = Color3<double>;
	using Color3l = Color3<long double>;

	template<std::floating_point T>
	constexpr Color3<T> operator+(const Color3<T>& l, const Color3<T>& r) {
		return Color3(l.r + r.r, l.g + r.g, l.b + r.b);
	}
	template<std::floating_point T>
	constexpr Color3<T> operator-(const Color3<T>& l, const Color3<T>& r) {
		return Color3(l.r - r.r, l.g - r.g, l.b - r.b);
	}
	template<std::floating_point T>
	constexpr Color3<T> operator*(const Color3<T>& l, T r) {
		return Color3(l.r * r, l.g * r, l.b * r);
	}
	template<std::floating_point T>
	constexpr Color3<T> operator*(T l, const Color3<T>& r) {
		return Color3(l * r.r, l * r.g, l * r.b);
	}
	template<std::floating_point T>
	constexpr Color3<T> operator*(const Color3<T>& l, const Color3<T>& r) {
		return Color3(l.r * r.r, l.g * r.g, l.b * r.b);
	}
	template<std::floating_point T>
	constexpr Color3<T> operator/(const Color3<T>& l, T r) {
		T ir = 1.f / r;
		return Color3(l.r * ir, l.g * ir, l.b * ir);
	}

	template<std::floating_point T>
	std::ostream& operator<<(std::ostream& output, const Color3<T>& v);
	template<std::floating_point T>
	std::istream& operator>>(std::istream& input, Color3<T>& v);
}