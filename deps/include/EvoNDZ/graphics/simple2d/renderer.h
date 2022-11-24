#pragma once
#include "../../math/vector3.h"
#include "../camera2d.h"
#include "plain_color_2d_batch.h"
#include "shape_batch.h"

namespace evo::s2d
{
	class Renderer {
	public:
		Renderer();
		void render(const Camera2D<float>&);

		void triangle(Vector2f pos, Vector2f size, Color3f cl = Color3f::White(), float angle = 0.f, float depth = 0.f) {
			m_opaqueTriangleBatch.add(pos, size, angle, cl, depth);
		}

		void triangle(Vector2f pos, Vector2f size, Color4f color, float rotation = 0.f, float depth = 0.f) {
			m_transparentTriangleBatch.add(pos, size, rotation, color, depth);
		}

		void circle(Vector2f pos, float radius, Color3f color = Color3f::White(), float depth = 0.f) {
			m_opaqueCircleBatch.add(pos, radius, color, depth);
		}

		void circle(Vector2f pos, float radius, Color4f color, float depth = 0.f) {
			m_transparentCircleBatch.add(pos, radius, color, depth);
		}

		void line(Vector2f a, Vector2f b, float width, Color3f color = Color3f::White(), float depth = 0.f) {
			m_opaqueLineBatch.add(a, b, width, color, depth);
		}

		void line(Vector2f a, Vector2f b, float width, Color4f color, float depth = 0.f) {
			m_transparentLineBatch.add(a, b, width, color, depth);
		}

		void rectangle(Vector2f pos, Vector2f siz, float angle = 0.f, Color3f cl = Color3f::White(), float depth = 0.f) {
			m_opaqueRectangleBatch.add(pos, siz, angle, cl, depth);
		}

		void rectangle(Vector2f pos, Vector2f siz, float angle, Color4f color, float depth = 0.f) {
			m_transparentRectangleBatch.add(pos, siz, angle, color, depth);
		}

		void arrow(Vector2f a, Vector2f b, float width, Vector2f head, Color3f cl = Color3f::White(), float depth = 0.f) {
			Vector2f r = b - a;
			float fullLen = r.length();
			float angle = r.rotation();
			m_opaqueLineBatch.add(a, angle, fullLen - head.x, width, cl, depth);
			m_opaqueTriangleBatch.add(b - r * (head.x / fullLen), head, angle, cl, depth);
		}

		void arrow(Vector2f a, Vector2f b, float width, Vector2f head, Color4f cl = Color4f::White(), float depth = 0.f) {
			Vector2f r = b - a;
			float fullLen = r.length();
			float angle = r.rotation();
			m_transparentLineBatch.add(a, angle, fullLen - head.x, width, cl, depth);
			m_transparentTriangleBatch.add(b - r * ( head.x / fullLen ), head, angle, cl, depth);
		}

	private:
		s2d::RectangleBatch<false> m_opaqueRectangleBatch;
		s2d::TriangleBatch<false> m_opaqueTriangleBatch;
		s2d::CircleBatch<false> m_opaqueCircleBatch;
		s2d::LineBatch<false> m_opaqueLineBatch;
		s2d::RectangleBatch<true> m_transparentRectangleBatch;
		s2d::TriangleBatch<true> m_transparentTriangleBatch;
		s2d::CircleBatch<true> m_transparentCircleBatch;
		s2d::LineBatch<true> m_transparentLineBatch;
	};
}