#pragma once
#include <cmath>
#include "matrix4.h"
#include "vector3.h"

namespace evo
{
	template<std::floating_point T>
	struct Quaternion {
		T s, x, y, z;

		Quaternion() noexcept = default;
		constexpr Quaternion(T s, T x, T y, T z) noexcept : s(s), x(x), y(y), z(z) { }
		constexpr Quaternion(T s, const Vector3<T>& v) noexcept : s(s), x(v.x), y(v.y), z(v.z) { }
		constexpr Quaternion(const Quaternion& q) noexcept : s(q.s), x(q.x), y(q.y), z(q.z) { }

		constexpr T norm() const {
			return math::sqrt(s * s + x * x + y * y + z * z);
		}
		constexpr T sqrnorm() const noexcept {
			return s * s + x * x + y * y + z * z;
		}
		constexpr void normalize() {
			T in = 1.f / this->norm();
			s *= in;
			x *= in;
			y *= in;
			z *= in;
		}
		constexpr Quaternion normalized() const {
			Quaternion qc = *this;
			qc.normalize();
			return qc;
		}
		constexpr void inverse() noexcept {
			T invSqrNorm = 1.f / ( s * s + x * x + y * y + z * z );
			s = s * invSqrNorm;
			invSqrNorm = -invSqrNorm;
			x = x * invSqrNorm;
			y = y * invSqrNorm;
			z = z * invSqrNorm;
		}
		constexpr static Quaternion Inverse(const Quaternion& q) {
			T insNorm = -1.f / ( q.s * q.s + q.x * q.x + q.y * q.y + q.z * q.z );
			return Quaternion(-q.s * insNorm, q.x * insNorm, q.y * insNorm, q.z * insNorm);
		}
		constexpr static Quaternion Conjugate(const Quaternion& q) noexcept {
			return Quaternion(q.s, -q.x, -q.y, -q.z);
		}
		constexpr void negate() noexcept {
			s = -s;
			x = -x;
			y = -y;
			z = -z;
		}
		constexpr static T Dot(const Quaternion& q1, const Quaternion q2) noexcept {
			return q1.s * q2.s + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
		}
		constexpr static Quaternion Identity() noexcept {
			return Quaternion(1.f, 0.f, 0.f, 0.f);
		}
		constexpr static Quaternion FromAxisAngle(const Vector3<T>& axis, T angle) noexcept {
			T half_angle = angle * .5f;
			T sinA = math::sin(half_angle);
			T cosA = math::cos(half_angle);
			return Quaternion(cosA, axis.x * sinA, axis.y * sinA, axis.z * sinA);
		}
		void to_axis_angle(Vector3<T>& axis, T& angle) {
			T k = sqrtf(1.f - this->s * this->s);
			if (k > 1e-6f) {
				k = 1.f / k;
				axis.x = this->x * k;
				axis.y = this->y * k;
				axis.z = this->z * k;
				angle = 2.f * acosf(this->s);
			} else {
				axis.x = this->x;
				axis.y = this->y;
				axis.z = this->z;
				angle = 2.f * acosf(this->s);
			}
		}
		constexpr static Quaternion YawPitchRoll(T yaw, T pitch, T roll) noexcept {
			T hRoll = roll * 0.5;
			T hYaw = yaw * 0.5;
			T hPitch = pitch * 0.5;
			T sinhR = math::sin(hRoll);
			T coshR = math::cos(hRoll);
			T sinhP = math::sin(hPitch);
			T coshP = math::cos(hPitch);
			T sinhY = math::sin(hYaw);
			T coshY = math::cos(hYaw);
			return Quaternion(
				( ( coshY * coshP ) * coshR ) + ( ( sinhY * sinhP ) * sinhR ),
				( ( coshY * sinhP ) * coshR ) + ( ( sinhY * coshP ) * sinhR ),
				( ( sinhY * coshP ) * coshR ) - ( ( coshY * sinhP ) * sinhR ),
				( ( coshY * coshP ) * sinhR ) - ( ( sinhY * sinhP ) * coshR )
			);
		}
		constexpr Vector3<T> transform(const Vector3<T>& v) const;
		static Quaternion Lerp(Quaternion pq1, Quaternion pq2, T t);
		// Expects normalized input quaternions.
		static Quaternion LerpN(Quaternion q1, Quaternion q2, T t);
		static Quaternion Slerp(Quaternion pq1, Quaternion pq2, T t);
		// Expects normalized input quaternions.
		static Quaternion SlerpN(Quaternion q1, Quaternion q2, T t);
		constexpr void to_matrix(Matrix4<T>& m) const noexcept;
		constexpr Matrix4<T> to_matrix() const noexcept;
		constexpr static Quaternion FromMatrix(const Matrix4<T>& m) noexcept;
	};

	template<std::floating_point T>
	Quaternion<T> operator +(const Quaternion<T>& l, const Quaternion<T>& r) {
		return Quaternion<T>(l.s + r.s, l.x + r.x, l.y + r.y, l.z + r.z);
	}
	template<std::floating_point T>
	constexpr Quaternion<T> operator -(const Quaternion<T>& l, const Quaternion<T>& r) {
		return Quaternion<T>(l.s - r.s, l.x - r.x, l.y - r.y, l.z - r.z);
	}
	template<std::floating_point T>
	constexpr Quaternion<T> operator *(const Quaternion<T>& l, T r) {
		return Quaternion<T>(l.s * r, l.x * r, l.y * r, l.z * r);
	}
	template<std::floating_point T>
	constexpr Quaternion<T> operator *(T l, const Quaternion<T>& r) {
		return Quaternion<T>(l * r.s, l * r.x, l * r.y, l * r.z);
	}
	template<std::floating_point T>
	constexpr Quaternion<T> operator /(const Quaternion<T>& l, T r) {
		T ir = 1.f / r;
		return l * ir;
	}
	template<std::floating_point T>
	constexpr Quaternion<T> operator -(const Quaternion<T>& r) {
		return Quaternion<T>(-r.s, -r.x, -r.y, -r.z);
	}
	template<std::floating_point T>
	constexpr Quaternion<T> operator *(const Quaternion<T>& l, const Quaternion<T>& r) {
		return Quaternion<T>(
			l.s * r.s - l.x * r.x - l.y * r.y - l.z * r.z,
			l.s * r.x + r.s * l.x + l.y * r.z - l.z * r.y,
			l.s * r.y + r.s * l.y + l.z * r.x - l.x * r.z,
			l.s * r.z + r.s * l.z + l.x * r.y - l.y * r.x
			);
	}
	// Returns multiplication of quaternion and pure quaternion from vector.
	template<std::floating_point T>
	constexpr Quaternion<T> operator *(const Quaternion<T>& l, const Vector3<T>& r) {
		return Quaternion<T>(
			-l.x * r.x - l.y * r.y - l.z * r.z,
			l.s * r.x + l.y * r.z - l.z * r.y,
			l.s * r.y + l.z * r.x - l.x * r.z,
			l.s * r.z + l.x * r.y - l.y * r.x
			);
	}
	// Returns multiplication of quaternion and pure quaternion from vector.
	template<std::floating_point T>
	constexpr Quaternion<T> operator *(const Vector3<T>& l, const Quaternion<T>& r) {
		return Quaternion<T>(
			-r.x * l.x - r.y * l.y - r.z * l.z,
			r.s * l.x + r.y * l.z - r.z * l.y,
			r.s * l.y + r.z * l.x - r.x * l.z,
			r.s * l.z + r.x * l.y - r.y * l.x
			);
	}

	template<std::floating_point T>
	constexpr Vector3<T> Quaternion<T>::transform(const Vector3<T>& v) const {
		auto q = ( *this * v ) * Inverse(*this);
		return Vector3<T>(q.x, q.y, q.z);
	}
	template<std::floating_point T>
	Quaternion<T> Quaternion<T>::Lerp(Quaternion<T> pq1, Quaternion<T> pq2, T t) {
		auto q1 = pq1.normalized();
		auto q2 = pq2.normalized();
		return ( q1 + ( q2 - q1 ) * t ).normalized();
	}
	template<std::floating_point T>
	Quaternion<T> Quaternion<T>::LerpN(Quaternion<T> q1, Quaternion<T> q2, T t) {
		return ( q1 + ( q2 - q1 ) * t ).normalized();
	}
	template<std::floating_point T>
	Quaternion<T> Quaternion<T>::Slerp(Quaternion<T> pq1, Quaternion<T> pq2, T t) {
		auto q1 = pq1.normalized();
		auto q2 = pq2.normalized();
		T dotp = Dot(q1, q2);
		if (dotp < 0.0) {
			dotp = -dotp;
			q2.negate();
		}
		if (dotp > 0.9995f) {
			return LerpN(q1, q2, t);
		}
		T theta = acosf(dotp);
		T thetaT = theta * t;
		T s2 = sinf(thetaT) / sinf(theta);
		T s1 = cosf(thetaT) - dotp * s2;
		return q1 * s1 + q2 * s2;
	}
	template<std::floating_point T>
	Quaternion<T> Quaternion<T>::SlerpN(Quaternion<T> q1, Quaternion<T> q2, T t) {
		T dotp = Dot(q1, q2);
		if (dotp < 0.0) {
			dotp = -dotp;
			q2.negate();
		}
		if (dotp > 0.9995f) {
			return LerpN(q1, q2, t);
		}
		T theta = acosf(dotp);
		T thetaT = theta * t;
		T s2 = sinf(thetaT) / sinf(theta);
		T s1 = cosf(thetaT) - dotp * s2;
		return q1 * s1 + q2 * s2;
	}
	template<std::floating_point T>
	constexpr void Quaternion<T>::to_matrix(Matrix4<T>& m) const noexcept {
		const T x2 = this->x * this->x;
		const T y2 = this->y * this->y;
		const T z2 = this->z * this->z;
		const T xy = this->x * this->y;
		const T xz = this->x * this->z;
		const T yz = this->y * this->z;
		const T sx = this->s * this->x;
		const T sy = this->s * this->y;
		const T sz = this->s * this->z;

		m[0] = 1.f - 2.f * ( y2 + z2 );
		m[1] = 2.f * ( xy - sz );
		m[2] = 2.f * ( xz + sy );
		m[3] = 0.f;
		m[4] = 2.f * ( xy + sz );
		m[5] = 1.f - 2.f * ( x2 + z2 );
		m[6] = 2.f * ( yz - sx );
		m[7] = 0.f;
		m[8] = 2.f * ( xz - sy );
		m[9] = 2.f * ( yz + sx );
		m[10] = 1.f - 2.f * ( x2 + y2 );
		m[11] = 0.f;
		m[12] = 0.f;
		m[13] = 0.f;
		m[14] = 0.f;
		m[15] = 1.f;
	}
	template<std::floating_point T>
	constexpr Matrix4<T> Quaternion<T>::to_matrix() const noexcept {
		Matrix4<T> m {};
		to_matrix(m);
		return m;
	}
	template<std::floating_point T>
	constexpr Quaternion<T> Quaternion<T>::FromMatrix(const Matrix4<T>& m) noexcept {
		T s, x, y, z;
		T t = 0.0f;
		if (m[10] < 0) {
			if (m[0] > m[5]) {
				t = 1 + m[0] - m[5] - m[10];
				x = t;
				y = m[1] + m[4];
				z = m[8] + m[2];
				s = m[9] - m[6];
			}
			else {
				t = 1 - m[0] + m[5] - m[10];
				x = m[1] + m[4];
				y = t;
				z = m[6] + m[9];
				s = m[2] - m[8];
			}
		}
		else {
			if (m[0] < -m[5]) {
				t = 1 - m[0] - m[5] + m[10];
				x = m[8] + m[2];
				y = m[6] + m[9];
				z = t;
				s = m[4] - m[1];
			}
			else {
				t = 1 + m[0] + m[5] + m[10];
				x = m[9] - m[6];
				y = m[2] - m[8];
				z = m[4] - m[1];
				s = t;
			}
		}
		T k = 0.5f / math::sqrt(t);
		return Quaternion<T>(s * k, x * k, y * k, z * k);
	}
}