#pragma once
#include <cmath>
#include "vector3.h"
#include "vector2.h"

namespace evo
{
	// stored row-major, treats vectors as columns
	// (there are v*M and M*v operators defined though)
	template<std::floating_point T>
	struct Matrix3 {
	private:
		T m[9];

	public:
		using value_type = T;

		Matrix3() noexcept = default;

		constexpr Matrix3(	T m1, T m2, T m3, 
							T m4, T m5, T m6, 
							T m7, T m8, T m9)
			: m { m1, m2, m3, m4, m5, m6, m7, m8, m9 } { }

		constexpr T& operator[](size_t i) noexcept {
			return m[i];
		}
		constexpr const T& operator[](size_t i) const noexcept {
			return m[i];
		}
		constexpr const T& operator()(size_t row, size_t column) const noexcept {
			return m[(row << 1) + row + column];
		}
		constexpr T& operator()(size_t row, size_t column) noexcept {
			return m[(row << 1) + row + column];
		}
		constexpr T* data() noexcept {
			return m;
		}
		constexpr const T* data() const noexcept {
			return m;
		}

		constexpr static Matrix3 multiplication_identity() {
			return Identity();
		}
		constexpr static Matrix3 summation_identity() {
			return Zero();
		}

		constexpr void transpose() noexcept {
			std::swap(m[1], m[3]);
			std::swap(m[2], m[6]);
			std::swap(m[5], m[7]);
		}
		constexpr Matrix3 transposed() const noexcept {
			return Matrix3(
				m[0], m[3], m[6],
				m[1], m[4], m[7],
				m[2], m[5], m[8]
			);
		}
		constexpr T determinant() const noexcept {
			return
				m[0] * ( m[4] * m[8] - m[5] * m[7] )
				- m[1] * ( m[3] * m[8] - m[5] * m[6] )
				+ m[2] * ( m[3] * m[7] - m[4] * m[6] );
		}
		constexpr Matrix3 cofactor() const noexcept {
			return {
				m[4] * m[8] - m[5] * m[7], m[5] * m[6] - m[3] * m[8], m[3] * m[7] - m[4] * m[6],
				m[2] * m[7] - m[1] * m[8], m[0] * m[8] - m[2] * m[6], m[1] * m[6] - m[0] * m[7],
				m[1] * m[5] - m[4] * m[2], m[2] * m[3] - m[0] * m[5], m[0] * m[4] - m[1] * m[3]
			};
		}
		constexpr Matrix3 adjugate() const noexcept {
			Matrix3 cof = cofactor();
			cof.transpose();
			return cof;
		}
		constexpr Matrix3 inverted() const noexcept {
			Matrix3 adj = adjugate();
			T det = determinant();
			return adj / det;
		}
		constexpr void invert() noexcept {
			T det = determinant();
			*this = adjugate();
			*this /= det;
		}

		// same as M * vec3(v, 1)
		constexpr Vector2<T> transform_position(const Vector2<T> v) const noexcept {
			return Vector2<T>(
				v.x * m[0] + v.y * m[1] + m[2],
				v.x * m[3] + v.y * m[4] + m[5]
			);
		}
		// same as M * vec3(v, 0)
		constexpr Vector2<T> transform_direction(const Vector2<T> v) const noexcept {
			return Vector2<T>(
				v.x * m[0] + v.y * m[1],
				v.x * m[3] + v.y * m[4]
			);
		}

		template<typename F> requires std::is_invocable_r_v<T, F, T> 
		constexpr Matrix3 map(F&& f) const noexcept(noexcept(F(T{}))){
			return Matrix3(f(m[0]), f(m[1]), f(m[2]), f(m[3]), f(m[4]), f(m[5]), f(m[6]), f(m[7]), f(m[8]));
		}

		constexpr void fill(T v) noexcept {
			m[0] = m[1] = m[2] = m[3] = m[4] = m[5] = m[6] = m[7] = m[8] = v;
		}
		constexpr void set_zero() noexcept {
			m[0] = m[1] = m[2] = m[3] = m[4] = m[5] = m[6] = m[7] = m[8] = T(0.l);
		}
		constexpr static Matrix3 Zero() noexcept {
			Matrix3 m{};
			m.set_zero();
			return m;
		}
		constexpr void set_identity() noexcept {
			m[0] = T(1.0l);
			m[1] = T(0);
			m[2] = T(0);
			m[3] = T(0);
			m[4] = T(1.0l);
			m[5] = T(0);
			m[6] = T(0);
			m[7] = T(0);
			m[8] = T(1.0l);
		}
		constexpr static Matrix3 Identity() noexcept {
			Matrix3 m{};
			m.set_identity();
			return m;
		}
		constexpr void set_scale(const Vector2<T> scale) noexcept {
			m[0] = scale.x;
			m[1] = T(0.l);
			m[2] = T(0.l);
			m[3] = T(0.l);
			m[4] = scale.y;
			m[5] = T(0.l);
			m[6] = T(0.l);
			m[7] = T(0.l);
			m[8] = T(1.l);
		}
		constexpr static Matrix3 Scale(const Vector2<T> s) noexcept {
			Matrix3 m{};
			m.set_scale(s);
			return m;
		}
		constexpr void set_translation(const Vector2<T> t) noexcept {
			m[0] = T(1.0l);
			m[1] = T(0);
			m[2] = t.x;
			m[3] = T(0);
			m[4] = T(1.0l);
			m[5] = t.y;
			m[6] = T(0);
			m[7] = T(0);
			m[8] = T(1.0l);
		}
		constexpr static Matrix3 Translation(const Vector2<T> t) {
			Matrix3 m{};
			m.set_translation(t);
			return m;
		}
		constexpr void set_scale_translation(const Vector2<T> s, const Vector2<T> t) {
			m[0] = s.x;
			m[1] = T(0.l);
			m[2] = t.x;
			m[3] = T(0.l);
			m[4] = s.y;
			m[5] = t.y;
			m[6] = T(0.l);
			m[7] = T(0.l);
			m[8] = T(1.l);
		}
		constexpr static Matrix3 ScaleTranslation(const Vector2<T> s, const Vector2<T> t) {
			Matrix3 m{};
			m.set_scale_translation(s, t);
			return m;
		}
	    void set_rotation(T angle) noexcept {
			T sina = sin(angle), cosa = cos(angle);
			m[0] = cosa;
			m[1] = -sina;
			m[2] = T(0.l);
			m[3] = sina;
			m[4] = cosa;
			m[5] = T(0.l);
			m[6] = T(0.l);
			m[7] = T(0.l);
			m[8] = T(1.l);
		}
		static Matrix3 Rotation(T angle) {
			Matrix3 m{};
			m.set_rotation(angle);
			return m;
		}
		//TRS - Scale, Rotate, Translate (reversed order because vectors are treated as columns)
		void set_world(const Vector2<T> scale, T rotation, const Vector2<T> translation) {
			const T c = cos(rotation), s = sin(rotation);
			const auto [x, y] = translation;
			const auto [l, k] = scale;
			m[0] = c * l;
			m[1] = -k * s;
			m[2] = x;
			m[3] = l * s;
			m[4] = c * k;
			m[5] = y;
			m[6] = m[7] = T(0.l);
			m[8] = T(1.l);
		}
		static Matrix3 World(const Vector2<T> scale, T rotation, const Vector2<T> translation) {
			Matrix3 m{};
			m.set_world(scale, rotation, translation);
			return m;
		}
		constexpr static void Multiply(Matrix3& res, const Matrix3& a, const Matrix3& b) noexcept {
			res.m[0] = a.m[0] * b.m[0] + a.m[1] * b.m[3] + a.m[2] * b.m[6];
			res.m[1] = a.m[0] * b.m[1] + a.m[1] * b.m[4] + a.m[2] * b.m[7];
			res.m[2] = a.m[0] * b.m[2] + a.m[1] * b.m[5] + a.m[2] * b.m[8];
			res.m[3] = a.m[3] * b.m[0] + a.m[4] * b.m[3] + a.m[5] * b.m[6];
			res.m[4] = a.m[3] * b.m[1] + a.m[4] * b.m[4] + a.m[5] * b.m[7];
			res.m[5] = a.m[3] * b.m[2] + a.m[4] * b.m[5] + a.m[5] * b.m[8];
			res.m[6] = a.m[6] * b.m[0] + a.m[7] * b.m[3] + a.m[8] * b.m[6];
			res.m[7] = a.m[6] * b.m[1] + a.m[7] * b.m[4] + a.m[8] * b.m[7];
			res.m[8] = a.m[6] * b.m[2] + a.m[7] * b.m[5] + a.m[8] * b.m[8];
		}
		//inv(W) = inv(TRS) = inv(S)inv(R)inv(T) ~
		void set_view_projection(const Vector2<T> scale, const T rotation, const Vector2<T> translation, T aspectRatio) {
			const auto[x, y] = translation;
			const auto[l, k] = T(1.0l) / scale;
			const T r = T(1.0l) / aspectRatio;
			const T c = cos(rotation), s = sin(rotation); // -rotation - WHY?
			const T lr = l * r, kc = k * c, ks = k * s, lrc = lr * c, lrs = lr * s;
			m[0] = lrc;
			m[1] = lrs;
			m[2] = -lrc * x - lrs * y;
			m[3] = -ks;
			m[4] = kc;
			m[5] = ks * x - kc * y;
			m[6] = m[7] = T(0.l);
			m[8] = T(1.l);
		}
		static Matrix3 ViewProjection(const Vector2<T> position, const Vector2<T> scale, const T angle, T aspectRatio) noexcept {
			Matrix3 m{};
			m.set_view_projection(scale, angle, position, aspectRatio);
			return m;
		}

		constexpr Vector3<T> operator*(const Vector3<T>& v) const {
			return Vector3<T>(
				v.x * m[0] + v.y * m[1] + v.z * m[2],
				v.x * m[3] + v.y * m[4] + v.z * m[5],
				v.x * m[6] + v.y * m[7] + v.z * m[8]
			);
		}
		constexpr friend Vector3<T> operator*(const Vector3<T>& v, const Matrix3& m) {
			return Vector3<T>(
				v.x * m.m[0] + v.y * m.m[3] + v.z * m.m[6],
				v.x * m.m[1] + v.y * m.m[4] + v.z * m.m[7],
				v.x * m.m[2] + v.y * m.m[5] + v.z * m.m[8]
			);
		}
		constexpr Matrix3 operator*(const Matrix3& m) const noexcept {
			Matrix3 res{};
			Multiply(res, *this, m);
			return res;
		}
		constexpr Matrix3 operator+(const Matrix3& m) const noexcept {
			return {
				this->m[0] + m.m[0], this->m[1] + m.m[1], this->m[2] + m.m[2],
				this->m[3] + m.m[3], this->m[4] + m.m[4], this->m[5] + m.m[5],
				this->m[6] + m.m[6], this->m[7] + m.m[7], this->m[8] + m.m[8]
			};
		}
		constexpr Matrix3 operator-(const Matrix3& m) const noexcept {
			return {
				this->m[0] - m.m[0], this->m[1] - m.m[1], this->m[2] - m.m[2],
				this->m[3] - m.m[3], this->m[4] - m.m[4], this->m[5] - m.m[5],
				this->m[6] - m.m[6], this->m[7] - m.m[7], this->m[8] - m.m[8]
			};
		}
		constexpr Matrix3 operator-() const noexcept {
			return {
				-m[0], -m[1], -m[2],
				-m[3], -m[4], -m[5],
				-m[6], -m[7], -m[8]
			};
		}
		constexpr Matrix3 operator*(T k) const noexcept {
			return {
				m[0] * k, m[1] * k, m[2] * k,
				m[3] * k, m[4] * k, m[5] * k,
				m[6] * k, m[7] * k, m[8] * k
			};
		}
		constexpr Matrix3 operator/(T v) const noexcept {
			T k = T(1.0l) / v;
			return *this * k;
		}
		constexpr friend Matrix3 operator*(T k, const Matrix3& m) {
			return m * k;
		}
		constexpr Matrix3& operator*=(T k) noexcept {
			m[0] *= k;
			m[1] *= k;
			m[2] *= k;
			m[3] *= k;
			m[4] *= k;
			m[5] *= k;
			m[6] *= k;
			m[7] *= k;
			m[8] *= k;
			return *this;
		}
		constexpr Matrix3& operator/=(T v) noexcept {
			return *this *= (T(1.0l) / v);
		}
		constexpr Matrix3& operator*=(const Matrix3& k) noexcept {
			return *this = *this * k;
		}
		constexpr Matrix3& operator+=(const Matrix3& k) noexcept {
			for (size_t i = 0; i < 9; ++i) m[i] += k.m[i];
			return *this;
		}
		constexpr Matrix3& operator-=(const Matrix3& k) noexcept {
			for (size_t i = 0; i < 9; ++i) m[i] -= k.m[i];
			return *this;
		}


		constexpr bool operator==(const Matrix3& m) const noexcept {
			for (size_t i = 0; i < 9; ++i) if (this->m[i] != m.m[i]) return false;
			return true;
		}
		constexpr bool operator!=(const Matrix3& m) const noexcept {
			return !( *this == m );
		}
		constexpr bool operator>(const Matrix3& m) const noexcept {
			for (size_t i = 0; i < 9; ++i) if (this->m[i] <= m.m[i]) return false;
			return true;
		}
		constexpr bool operator>=(const Matrix3& m) const noexcept {
			for (size_t i = 0; i < 9; ++i) if (this->m[i] < m.m[i]) return false;
			return true;
		}
		constexpr bool operator<(const Matrix3& m) const noexcept {
			for (size_t i = 0; i < 9; ++i) if (this->m[i] >= m.m[i]) return false;
			return true;
		}
		constexpr bool operator<=(const Matrix3& m) const noexcept {
			for (size_t i = 0; i < 9; ++i) if (this->m[i] < m.m[i]) return false;
			return true;
		}
		
		constexpr bool operator>(const T v) const noexcept {
			for (size_t i = 0; i < 9; ++i) if (this->m[i] <= v) return false;
			return true;
		}
		constexpr bool operator>=(const T v) const noexcept {
			for (size_t i = 0; i < 9; ++i) if (this->m[i] < v) return false;
			return true;
		}
		constexpr bool operator<(const T v) const noexcept {
			for (size_t i = 0; i < 9; ++i) if (this->m[i] >= v) return false;
			return true;
		}
		constexpr bool operator<=(const T v) const noexcept {
			for (size_t i = 0; i < 9; ++i) if (this->m[i] < v) return false;
			return true;
		}

		friend constexpr bool operator>(const T v, const Matrix3& m) noexcept {
			return m < v;
		}
		friend constexpr bool operator>=(const T v, const Matrix3& m) noexcept {
			return m <= v;
		}
		friend constexpr bool operator<(const T v, const Matrix3& m) noexcept {
			return m > v;
		}
		friend constexpr bool operator<=(const T v, const Matrix3& m) noexcept {
			return m >= v;
		}
	};

	using Matrix3f = Matrix3<float>;
	using Matrix3d = Matrix3<double>;
	using Matrix3l = Matrix3<long double>;
}

namespace evo::math
{
	template<std::floating_point T>
	constexpr Matrix3<T> abs(const Matrix3<T>& m) {
		return m.map([](T x) { return math::abs(x); });
	}
}