#pragma once
#include <cmath>
#include "vector3.h"
#include "vector4.h"

namespace evo
{
	// stored row-major, treats vectors as columns
	template<std::floating_point T>
	struct Matrix4 {
	private:
		T m[16];

	public:
		Matrix4() noexcept = default;

		constexpr Matrix4(	T m1,	T m2,	T m3,	T m4,
							T m5,	T m6,	T m7,	T m8,
							T m9,	T m10,	T m11,	T m12,
							T m13,	T m14,	T m15,	T m16) noexcept
			: m{ m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16 } { }

		constexpr T& operator[](size_t i) noexcept {
			return m[i];
		}
		constexpr const T& operator[](size_t i) const noexcept {
			return m[i];
		}
		constexpr const T& operator()(size_t row, size_t column) const noexcept {
			return m[(row << 2) + column];
		}
		constexpr T& operator()(size_t row, size_t column) noexcept {
			return m[(row << 2) + column];
		}
		constexpr T* data() noexcept {
			return m;
		}
		constexpr const T* data() const noexcept {
			return m;
		}

		constexpr static Matrix4 multiplication_identity() {
			return Identity();
		}
		constexpr static Matrix4 summation_identity() {
			return Zero();
		}

		constexpr void transpose() noexcept {
			std::swap(m[1], m[4]);
			std::swap(m[2], m[8]);
			std::swap(m[3], m[12]);
			std::swap(m[6], m[9]);
			std::swap(m[7], m[13]);
			std::swap(m[11], m[14]);
		}
		constexpr Matrix4 transposed() const noexcept {
			return Matrix4(
				m[0], m[4], m[8], m[12],
				m[1], m[5], m[9], m[13],
				m[2], m[6], m[10], m[14],
				m[3], m[7], m[11], m[15]
			);
		}

		// same as M * vec4(v, 1)
		constexpr Vector3<T> transform_position(const Vector3<T>& v) const noexcept {
			return Vector3<T>(
				v.x * m[0] + v.y * m[1] + v.z * m[2] + m[3],
				v.x * m[4] + v.y * m[5] + v.z * m[6] + m[7],
				v.x * m[8] + v.y * m[9] + v.z * m[10] + m[11]
			);
		}
		// same as M * vec4(v, 0)
		constexpr Vector3<T> transform_direction(const Vector3<T>& v) const noexcept {
			return Vector3<T>(
				v.x * m[0] + v.y * m[1] + v.z * m[2],
				v.x * m[4] + v.y * m[5] + v.z * m[6],
				v.x * m[8] + v.y * m[9] + v.z * m[10]
			);
		}

		constexpr void fill(const T v) noexcept {
			m[0] = m[1] = m[2] = m[3] = m[4] = m[5] = m[6] = m[7] = m[8] = m[9] = m[10] = m[11] = m[12] = m[13] = m[14] = m[15] = v;
		}
		constexpr void set_zero() noexcept {
			fill(0.l);
		}
		constexpr static Matrix4 Zero() noexcept {
			Matrix4 m{};
			m.set_zero();
			return m;
		}
		constexpr void set_identity() noexcept {
			m[0] = T(1.0l);
			m[1] = T(0);
			m[2] = T(0);
			m[3] = T(0);
			m[4] = T(0);
			m[5] = T(1.0l);
			m[6] = T(0);
			m[7] = T(0);
			m[8] = T(0);
			m[9] = T(0);
			m[10] = T(1.0l);
			m[11] = T(0);
			m[12] = T(0);
			m[13] = T(0);
			m[14] = T(0);
			m[15] = T(1.0l);
		}
		constexpr static Matrix4 Identity() noexcept {
			Matrix4 m{};
			m.set_identity();
			return m;
		}
		constexpr void set_scale(const Vector3<T>& scale) noexcept {
			m[0] = scale.x;
			m[1] = T(0);
			m[2] = T(0);
			m[3] = T(0);
			m[4] = T(0);
			m[5] = scale.y;
			m[6] = T(0);
			m[7] = T(0);
			m[8] = T(0);
			m[9] = T(0);
			m[10] = scale.z;
			m[11] = T(0);
			m[12] = T(0);
			m[13] = T(0);
			m[14] = T(0);
			m[15] = T(1.0l);
		}
		constexpr static Matrix4 Scale(const Vector3<T>& scale) noexcept {
			Matrix4 m {};
			m.set_scale(scale);
			return m;
		}
		constexpr void set_translation(const Vector3<T>& translation) noexcept {
			m[0] = T(1.0l);
			m[1] = T(0);
			m[2] = T(0);
			m[3] = translation.x;
			m[4] = T(0);
			m[5] = T(1.0l);
			m[6] = T(0);
			m[7] = translation.y;
			m[8] = T(0);
			m[9] = T(0);
			m[10] = T(1.0l);
			m[11] = translation.z;
			m[12] = T(0);
			m[13] = T(0);
			m[14] = T(0);
			m[15] = T(1.0l);
		}
		constexpr static Matrix4 Translation(const Vector3<T>& translation) noexcept {
			Matrix4 m {};
			m.set_translation(translation);
			return m;
		}
		constexpr void set_rotation_x(const T angle) noexcept {
			T cosa = math::cos(angle);
			T sina = math::sin(angle);
			m[0] = T(1.0l);
			m[1] = T(0);
			m[2] = T(0);
			m[3] = T(0);
			m[4] = T(0);
			m[5] = cosa;
			m[6] = -sina;
			m[7] = T(0);
			m[8] = T(0);
			m[9] = sina;
			m[10] = cosa;
			m[11] = T(0);
			m[12] = T(0);
			m[13] = T(0);
			m[14] = T(0);
			m[15] = T(1.0l);
		}
		constexpr static Matrix4 RotationX(const T angle) noexcept {
			Matrix4 m {};
			m.set_rotation_x(angle);
			return m;
		}
		constexpr void set_rotation_y(const T angle) noexcept {
			T cosa = math::cos(angle);
			T sina = math::sin(angle);
			m[0] = cosa;
			m[1] = T(0);
			m[2] = sina;
			m[3] = T(0);
			m[4] = T(0);
			m[5] = T(1.0l);
			m[6] = T(0);
			m[7] = T(0);
			m[8] = -sina;
			m[9] = T(0);
			m[10] = cosa;
			m[11] = T(0);
			m[12] = T(0);
			m[13] = T(0);
			m[14] = T(0);
			m[15] = T(1.0l);
		}
		constexpr static Matrix4 RotationY(const T angle) noexcept {
			Matrix4 m {};
			m.set_rotation_y(angle);
			return m;
		}
		constexpr void set_rotation_z(const T angle) noexcept {
			T cosa = math::cos(angle);
			T sina = math::sin(angle);
			m[0] = cosa;
			m[1] = -sina;
			m[2] = T(0);
			m[3] = T(0);
			m[4] = sina;
			m[5] = cosa;
			m[6] = T(0);
			m[7] = T(0);
			m[8] = T(0);
			m[9] = T(0);
			m[10] = T(1.0l);
			m[11] = T(0);
			m[12] = T(0);
			m[13] = T(0);
			m[14] = T(0);
			m[15] = T(1.0l);
		}
		constexpr static Matrix4 RotationZ(const T angle) noexcept {
			Matrix4 m {};
			m.set_rotation_z(angle);
			return m;
		}
		constexpr void set_projection_perspective(T fieldOfView, T aspectRatio, T nearPlane, T farPlane) noexcept {
			T a;
			if (std::is_constant_evaluated()) {
				const T hfov = fieldOfView * T(0.5l);
				a = math::sin(hfov) / math::cos(hfov); // todo: constexpr (sin/cos) -> constexpr tan 
			}
			else {
				a = std::tan(fieldOfView * T(0.5l));
			}
			T b = T(1.0l) / ( nearPlane - farPlane );
			m[0] = T(1.0l) / ( a * aspectRatio );
			m[1] = T(0);
			m[2] = T(0);
			m[3] = T(0);
			m[4] = T(0);
			m[5] = 1.0 / a;
			m[6] = T(0);
			m[7] = T(0);
			m[8] = T(0);
			m[9] = T(0);
			m[10] = ( farPlane + nearPlane ) * b;
			m[11] = -T(1.0l);
			m[12] = T(0);
			m[13] = T(0);
			m[14] = T(2.0l) * ( nearPlane * farPlane ) * b;
			m[15] = T(0);
		}
		constexpr static Matrix4 ProjectionPerspective(T fov, T aspectRatio, T near, T far) noexcept {
			Matrix4 m {};
			m.set_projection_perspective(fov, aspectRatio, near, far);
			return m;
		}
		constexpr void set_look_at_inverted(const Vector3<T>& position, const Vector3<T>& target, const Vector3<T>& up) noexcept {
			Vector3<T> nForward = ( position - target ).normalized();
			Vector3<T> nRight = Vector3<T>::Cross(up, nForward).normalized();
			Vector3<T> nUp = Vector3<T>::Cross(nForward, nRight);
			m[0] = nRight.x;
			m[1] = nRight.y;
			m[2] = nRight.z;
			m[3] = -Vector3<T>::Dot(nRight, position);
			m[4] = nUp.x;
			m[5] = nUp.y;
			m[6] = nUp.z;
			m[7] = -Vector3<T>::Dot(nUp, position);
			m[8] = nForward.x;
			m[9] = nForward.y;
			m[10] = nForward.z;
			m[11] = -Vector3<T>::Dot(nForward, position);
			m[12] = T(0);
			m[13] = T(0);
			m[14] = T(0);
			m[15] = T(1.0l);
		}
		constexpr static Matrix4 LookAtInverted(const Vector3<T>& position, const Vector3<T>& target, const Vector3<T>& up) noexcept {
			Matrix4 m {};
			m.set_look_at_inverted(position, target, up);
			return m;
		}
		constexpr void set_look_at(const Vector3<T>& position, const Vector3<T>& target, const Vector3<T>& up) noexcept {
			Vector3<T> nForward = (target - position).normalized();
			Vector3<T> nRight = Vector3<T>::Cross(up, nForward).normalized();
			Vector3<T> nUp = Vector3<T>::Cross(nForward, nRight);
			m[0] = nRight.x;
			m[1] = nUp.x;
			m[2] = nForward.x;
			m[3] = position.x;
			m[4] = nRight.y;
			m[5] = nUp.y;
			m[6] = nForward.y;
			m[7] = position.y;
			m[8] = nRight.z;
			m[9] = nUp.z;
			m[10] = nForward.z;
			m[11] = position.z;
			m[12] = T(0);
			m[13] = T(0);
			m[14] = T(0);
			m[15] = T(1.0l);
		}
		constexpr static Matrix4 LookAt(const Vector3<T>& position, const Vector3<T>& target, const Vector3<T>& up) noexcept {
			Matrix4 m {};
			m.set_look_at(position, target, up);
			return m;
		}
		constexpr void set_projection_orthographic(T left, T right, T bottom, T top, T near, T far) {
			const T a = T(1.0l) / ( right - left );
			const T b = T(1.0l) / ( top - bottom );
			const T c = T(1.0l) / ( near - far );
			m[0] = T(2.0l) * a;
			m[1] = T(0);
			m[2] = T(0);
			m[3] = -( right + left ) * a;
			m[4] = T(0);
			m[0] = T(2.0l) * b;
			m[6] = T(0);
			m[7] = -( top + bottom ) * b;
			m[8] = T(0);
			m[9] = T(0);
			m[10] = T(2.0l) * c;
			m[11] = ( far + near ) * c;
			m[12] = T(0);
			m[13] = T(0);
			m[14] = T(0);
			m[15] = T(1.0l);
		}
		constexpr static Matrix4 ProjectionOrthographic(
			const T left, const T right, const T bottom, const T top, const T near, const T far
		) noexcept {
			Matrix4 m {};
			m.set_projection_orthographic(left, right, bottom, top, near, far);
			return m;
		}
		constexpr static void Multiply(Matrix4& m, const Matrix4& l, const Matrix4& r) noexcept {
			m[0] = l.m[0] * r.m[0] + l.m[1] * r.m[4] + l.m[2] * r.m[8] + l.m[3] * r.m[12];
			m[1] = l.m[0] * r.m[1] + l.m[1] * r.m[5] + l.m[2] * r.m[9] + l.m[3] * r.m[13];
			m[2] = l.m[0] * r.m[2] + l.m[1] * r.m[6] + l.m[2] * r.m[10] + l.m[3] * r.m[14];
			m[3] = l.m[0] * r.m[3] + l.m[1] * r.m[7] + l.m[2] * r.m[11] + l.m[3] * r.m[15];
			m[4] = l.m[4] * r.m[0] + l.m[5] * r.m[4] + l.m[6] * r.m[8] + l.m[7] * r.m[12];
			m[5] = l.m[4] * r.m[1] + l.m[5] * r.m[5] + l.m[6] * r.m[9] + l.m[7] * r.m[13];
			m[6] = l.m[4] * r.m[2] + l.m[5] * r.m[6] + l.m[6] * r.m[10] + l.m[7] * r.m[14];
			m[7] = l.m[4] * r.m[3] + l.m[5] * r.m[7] + l.m[6] * r.m[11] + l.m[7] * r.m[15];
			m[8] = l.m[8] * r.m[0] + l.m[9] * r.m[4] + l.m[10] * r.m[8] + l.m[11] * r.m[12];
			m[9] = l.m[8] * r.m[1] + l.m[9] * r.m[5] + l.m[10] * r.m[9] + l.m[11] * r.m[13];
			m[10] = l.m[8] * r.m[2] + l.m[9] * r.m[6] + l.m[10] * r.m[10] + l.m[11] * r.m[14];
			m[11] = l.m[8] * r.m[3] + l.m[9] * r.m[7] + l.m[10] * r.m[11] + l.m[11] * r.m[15];
			m[12] = l.m[12] * r.m[0] + l.m[13] * r.m[4] + l.m[14] * r.m[8] + l.m[15] * r.m[12];
			m[13] = l.m[12] * r.m[1] + l.m[13] * r.m[5] + l.m[14] * r.m[9] + l.m[15] * r.m[13];
			m[14] = l.m[12] * r.m[2] + l.m[13] * r.m[6] + l.m[14] * r.m[10] + l.m[15] * r.m[14];
			m[15] = l.m[12] * r.m[3] + l.m[13] * r.m[7] + l.m[14] * r.m[11] + l.m[15] * r.m[15];
		}
	
		constexpr Vector4<T> operator*(const Vector4<T>& v) const {
			return Vector4<T>(
				v.x * m[0] + v.y * m[1] + v.z * m[2] + v.w * m[3],
				v.x * m[4] + v.y * m[5] + v.z * m[6] + v.w * m[7],
				v.x * m[8] + v.y * m[9] + v.z * m[10] + v.w * m[11],
				v.x * m[12] + v.y * m[13] + v.z * m[14] + v.w * m[15]
				);
		}
		constexpr friend Vector4<T> operator*(const Vector4<T>& v, const Matrix4& m) {
			return Vector4<T>(
				v.x * m.m[0] + v.y * m.m[4] + v.z * m.m[8] + v.w * m.m[12],
				v.x * m.m[1] + v.y * m.m[5] + v.z * m.m[9] + v.w * m.m[13],
				v.x * m.m[2] + v.y * m.m[6] + v.z * m.m[10] + v.w * m.m[14],
				v.x * m.m[3] + v.y * m.m[7] + v.z * m.m[11] + v.w * m.m[15]
				);
		}
		constexpr Matrix4 operator*(const Matrix4& m) const noexcept {
			Matrix4 res{};
			Multiply(res, *this, m);
			return res;
		}
		constexpr Matrix4 operator+(const Matrix4& m) const noexcept {
			return {
				this->m[0] + m.m[0], this->m[1] + m.m[1], this->m[2] + m.m[2], this->m[3] + m.m[3],
				this->m[4] + m.m[4], this->m[5] + m.m[5], this->m[6] + m.m[6], this->m[7] + m.m[7],
				this->m[8] + m.m[8], this->m[9] + m.m[9], this->m[10] + m.m[10], this->m[11] + m.m[11],
				this->m[12] + m.m[12], this->m[13] + m.m[13], this->m[14] + m.m[14], this->m[15] + m.m[15]
			};
		}
		constexpr Matrix4 operator-(const Matrix4& m) const noexcept {
			return {
				this->m[0] - m.m[0], this->m[1] - m.m[1], this->m[2] - m.m[2], this->m[3] - m.m[3],
				this->m[4] - m.m[4], this->m[5] - m.m[5], this->m[6] - m.m[6], this->m[7] - m.m[7],
				this->m[8] - m.m[8], this->m[9] - m.m[9], this->m[10] - m.m[10], this->m[11] - m.m[11],
				this->m[12] - m.m[12], this->m[13] - m.m[13], this->m[14] - m.m[14], this->m[15] - m.m[15]
			};
		}
		constexpr Matrix4 operator-() const noexcept {
			return {
				-m[0], -m[1], -m[2], -m[3],
				-m[4], -m[5], -m[6], -m[7],
				-m[8], -m[9], -m[10], -m[11],
				-m[12], -m[13], -m[14], -m[15],
			};
		}
		constexpr Matrix4 operator*(T k) const noexcept {
			return {
				m[0] * k, m[1] * k, m[2] * k, m[3] * k, 
				m[4] * k, m[5] * k, m[6] * k, m[7] * k,
				m[8] * k, m[9] * k, m[10] * k, m[11] * k,
				m[12] * k, m[13] * k, m[14] * k, m[15] * k
			};
		}
		constexpr Matrix4 operator/(T v) const noexcept {
			T k = T(1.0l) / v;
			return *this * k;
		}
		constexpr friend Matrix4 operator*(T k, const Matrix4& m) {
			return m * k;
		}
		constexpr Matrix4& operator*=(T k) noexcept {
			m[0] *= k;
			m[1] *= k;
			m[2] *= k;
			m[3] *= k;
			m[4] *= k;
			m[5] *= k;
			m[6] *= k;
			m[7] *= k;
			m[8] *= k;
			m[9] *= k;
			m[10] *= k;
			m[11] *= k;
			m[12] *= k;
			m[13] *= k;
			m[14] *= k;
			m[15] *= k;
			return *this;
		}
		constexpr Matrix4& operator/=(T v) noexcept {
			return *this *= ( T(1.0l) / v );
		}
		constexpr Matrix4& operator*=(const Matrix4& k) noexcept {
			Matrix4 c = *this;
			Multiply(*this, c, k);
			return *this;
		}
	};

	using Matrix4f = Matrix4<float>;
	using Matrix4d = Matrix4<double>;
	using Matrix4l = Matrix4<long double>;
}