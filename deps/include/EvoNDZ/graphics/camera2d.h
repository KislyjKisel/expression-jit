#pragma once
#include "../math/vector2.h"
#include "../math/matrix3.h"
#include "../app/application.h"
#include "../data/lazy.h"

namespace evo
{
	template<std::floating_point T>
	struct Camera2D {
	public:
		Vector2<T> position() const noexcept {
			return m_position;
		}
		Vector2<T> scale() const noexcept {
			return m_scale;
		}
		T rotation() const noexcept {
			return m_rotation;
		}
		T aspect_ratio() const noexcept {
			return m_aspectRatio;
		}
		void move_to(const Vector2<T> position) noexcept {
			invalidate();
			m_position = position;
		}
		void move_on(const Vector2<T> delta) noexcept {
			invalidate();
			m_position += delta;
		}
		void set_scale(const Vector2<T> scale) noexcept {
			invalidate();
			m_scale = scale;
		}
		void set_rotation(const T rotation) noexcept {
			invalidate();
			m_rotation = rotation;
		}
		void rotate(const T delta) noexcept {
			invalidate();
			m_rotation += delta;
		}
		void set_aspect_ratio(const T aspectRatio) noexcept {
			invalidate();
			m_aspectRatio = aspectRatio;
		}
		
		// camera view & projection matrix = PV = Pinv(W) = Pinv(TRS) = Pinv(S)inv(R)inv(T). Matrix P scales x by aspectRatio 
		const Matrix3<T>& matrix() const noexcept {
			return m_transform.value(*this);
		}
		// camera inverted view & projection matrix = inv(PV) = inv(V)inv(P) = Winv(P). Matrix P scales x by aspectRatio
		const Matrix3<T>& inverted_matrix() const noexcept {
			return m_invetedTransform.value(*this);
		}

		// returns world space position of the point on screen; point coordinates must be within the range of [0, 1]
		Vector2<T> screen_to_world(const Vector2<T> screenPosNorm) const noexcept {
			return inverted_matrix().transform_position(screenPosNorm);
		}
		// returns normalized screen space coordinates of the point in the world
		Vector2<T> world_to_screen(const Vector2<T> worldPos) const noexcept {
			return (matrix().transform_position(worldPos) + 1.0f) * 0.5f;
		}

		// camera/screen up vector in world space
		Vector2<T> up() const noexcept {
			return m_sup.value(*this);
		}
		// camera/screen down vector in world space
		Vector2<T> down() const noexcept {
			return m_sdown.value(*this);
		}
		// camera/screen left vector in world space
		Vector2<T> left() const noexcept {
			return m_sleft.value(*this);
		}
		// camera/screen right vector in world space
		Vector2<T> right() const noexcept {
			return m_sright.value(*this);
		}

		// world up vector in screen space
		Vector2<T> world_up() const noexcept {
			return m_wup.value(*this);
		}
		// world down vector in screen space
		Vector2<T> world_down() const noexcept {
			return m_wdown.value(*this);
		}
		// world left vector in screen space
		Vector2<T> world_left() const noexcept {
			return m_wleft.value(*this);
		}
		// world right vector in screen space
		Vector2<T> world_right() const noexcept {
			return m_wright.value(*this);
		}

		// idk if it works
		// returns scale in world space, that is equivalent to specified scale in screen space
		T screen_to_world_scale(const T size) {
			return inverted_matrix().transform_direction(Vector2<T>::X(size)).length();
		}

	private:
		struct CamTransformEvaluator {
			void operator()(Matrix3<T>* m, const Camera2D& parent) const noexcept {
				m->set_view_projection(parent.m_scale, parent.m_rotation, parent.m_position, parent.m_aspectRatio);
			}
		};
		struct CamInvertedTransformEvaluator {
			void operator()(Matrix3<T>* m, const Camera2D& parent) const noexcept {
				*m = parent.m_transform.value(parent);
				m->invert();
			}
		};
		template<Vector2<T> ScreenDir>
		struct CamScreenDirectionEvaluator {
			void operator()(Vector2<T>* d, const Camera2D& parent) const noexcept {
				*d = parent.inverted_matrix().transform_direction(ScreenDir).normalized();
			}
		};
		template<Vector2<T> WorldDir>
		struct CamWorldDirectionEvaluator {
			void operator()(Vector2<T>* d, const Camera2D& parent) const noexcept {
				*d = parent.matrix().transform_direction(WorldDir).normalized();
			}
		};

		mutable Lazy<Matrix3<T>, CamTransformEvaluator> m_transform;
		mutable Lazy<Matrix3<T>, CamInvertedTransformEvaluator> m_invetedTransform;
#pragma region Lazy Direction Vectors
		mutable Lazy<Vector2<T>, CamScreenDirectionEvaluator<Vector2(T( 0.0l), T( 1.0l))>> m_sup;
		mutable Lazy<Vector2<T>, CamScreenDirectionEvaluator<Vector2(T( 0.0l), T(-1.0l))>> m_sdown;
		mutable Lazy<Vector2<T>, CamScreenDirectionEvaluator<Vector2(T(-1.0l), T( 0.0l))>> m_sleft;
		mutable Lazy<Vector2<T>, CamScreenDirectionEvaluator<Vector2(T( 1.0l), T( 0.0l))>> m_sright;

		mutable Lazy<Vector2<T>, CamWorldDirectionEvaluator<Vector2<T>(T( 0.0l), T( 1.0l))>> m_wup;
		mutable Lazy<Vector2<T>, CamWorldDirectionEvaluator<Vector2<T>(T( 0.0l), T(-1.0l))>> m_wdown;
		mutable Lazy<Vector2<T>, CamWorldDirectionEvaluator<Vector2<T>(T(-1.0l), T( 0.0l))>> m_wleft;
		mutable Lazy<Vector2<T>, CamWorldDirectionEvaluator<Vector2<T>(T( 1.0l), T( 0.0l))>> m_wright;
#pragma endregion
		Vector2<T> m_position = { T(0), T(0) };
		Vector2<T> m_scale = { T(1.0l), T(1.0l) };
		T m_rotation = T(0);
		T m_aspectRatio = T(1.0l);

		void invalidate() noexcept {
			m_transform.destroy();
			m_invetedTransform.destroy();
			m_sup.destroy();
			m_sdown.destroy();
			m_sleft.destroy();
			m_sright.destroy();
			m_wup.destroy();
			m_wdown.destroy();
			m_wleft.destroy();
			m_wright.destroy();
		}
	};
}