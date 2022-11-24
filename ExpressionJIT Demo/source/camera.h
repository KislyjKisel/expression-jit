#pragma once
#include <EvoNDZ/math/vector3.h>
#include <EvoNDZ/math/matrix4.h>
#include <EvoNDZ/math/quaternion.h>
#include <EvoNDZ/input/input.h>
#include <EvoNDZ/data/lazy.h>

namespace ed
{
	class Camera {
	public:
		constexpr Camera(evo::Vector3f position) :
			m_orientation(evo::Quaternion<float>::Identity()), m_position(position),
			m_forward(sForward()), 
			m_up(sUp()),
			m_right(sRight()) { }

		void rotate(float yaw, float pitch, float roll) {
			invalidate();
			m_orientation = evo::Quaternion<float>::FromAxisAngle(sUp(), yaw) * m_orientation;
			m_orientation = evo::Quaternion<float>::FromAxisAngle(m_right, pitch) * m_orientation;
			m_orientation = evo::Quaternion<float>::FromAxisAngle(m_forward, roll) * m_orientation;

			m_up = m_orientation.transform(sUp());
			m_forward = m_orientation.transform(sForward());
			m_right = evo::Vector3f::Cross(m_forward, sUp());
		}
		void normalize() {
			invalidate();
			m_orientation.normalize();
		}

		constexpr evo::Vector3f right() const noexcept {
			return m_right;
		}
		constexpr evo::Vector3f left() const noexcept {
			return -m_right;
		}
		constexpr evo::Vector3f forward() const noexcept {
			return m_forward;
		}
		constexpr evo::Vector3f back() const noexcept {
			return -m_forward;
		}
		constexpr evo::Vector3f up() const noexcept {
			return m_up;
		}
		constexpr evo::Vector3f down() const noexcept {
			return -m_up;
		}
		constexpr evo::Vector3f position() const noexcept {
			return m_position;
		}
		void set_position(evo::Vector3f position) noexcept {
			invalidate();
			m_position = position;
		}
		evo::Quaternion<float> orientation() const noexcept {
			return m_orientation;
		}
		void set_orientation(const evo::Quaternion<float>& orientation) noexcept {
			invalidate();
			m_orientation = orientation;
		}

		const evo::Matrix4f& view_matrix() const {
			return m_view.value(*this);
		}

	private:
		struct CamViewMatrixEval {
			void operator()(evo::Matrix4f* m, const Camera& c) const noexcept {
				m->set_look_at_inverted(c.m_position, c.m_position + c.m_forward, sUp());
			}
		};
		mutable evo::Lazy<evo::Matrix4f, CamViewMatrixEval> m_view;
		evo::Quaternion<float> m_orientation;
		evo::Vector3f m_position;
		evo::Vector3f m_forward;
		evo::Vector3f m_up;
		evo::Vector3f m_right;

		void invalidate() noexcept {
			m_view.destroy();
		}

		inline static constexpr evo::Vector3f sRight()	 noexcept { return { -1, 0,  0 }; }
		inline static constexpr evo::Vector3f sUp()		 noexcept { return {  0, 1,  0 }; }
		inline static constexpr evo::Vector3f sForward() noexcept { return {  0, 0, -1 }; }
	};
}