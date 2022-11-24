#pragma once
#include <vector>
#include "../opengl/enum/draw_mode.h"
#include "../opengl/types.h"
#include "../../math/vector2.h"
#include "../../math/vector3.h"
#include "../../util/color4.h"
#include "plain_color_2d_batch.h"

namespace evo::s2d
{
	template<bool UseAlpha>
	class TriangleBatch {
	public:
		using color_t = std::conditional_t<UseAlpha, Color4f, Color3f>;

		TriangleBatch() : m_batch(ogl::DrawMode::Triangles(), true, true, UseAlpha) { 
			static constexpr Vector2f vertices[] {
					{ 0.f, -0.5f },
					{ 1.f, 0.f },
					{ 0.f, 0.5f }
			};
			static constexpr IndexType indices[] { 0, 1, 2 };
			m_batch.set_mesh(vertices, 3, indices, sizeof(indices) / sizeof(IndexType));
		}

		void add(Vector2f position, Vector2f size, float rotation = 0.f, color_t color = color_t::White(), float depth = 0.f) {
			m_triangles.push_back({ position, color, size, depth, rotation });
		}

		void render(const Matrix3f& view) {
			m_batch.render(view, m_triangles.data(), sizeof(Triangle) * m_triangles.size(), m_triangles.size());
			m_triangles.clear();
		}

	private:
		using IndexType = uint8_t;
		struct Triangle {
			Vector2f position;
			color_t color;
			Vector2f size;
			float depth;
			float rotation;
		};

		PlainColor2dBatch m_batch;
		std::vector<Triangle> m_triangles;
	};

	template<bool UseAlpha>
	class CircleBatch {
	public:
		using color_t = std::conditional_t<UseAlpha, Color4f, Color3f>;

		CircleBatch(size_t triangles = 6) : m_batch(ogl::DrawMode::TriangleFan(), false, false, UseAlpha) {
			if (triangles < 3) throw std::exception("There must be at least 3 triangles in a circle.");
			std::vector<Vector2f> dv { { 0, 0 }, { 1, 0 } };
			dv.reserve(triangles + 2);
			std::vector<IndexType> di { 0, 1 };
			di.reserve(triangles + 3);

			{
				float angle = 0.0f;
				float step = 2.0 * std::numbers::pi / triangles;
				for (size_t i = 2; i <= triangles; ++i) {
					angle += step;
					dv.push_back({ std::cos(angle), std::sin(angle) });
					di.push_back(i);
				}
				di.push_back(1);
			}

			m_batch.set_mesh( dv.data(), dv.size(), di.data(), di.size() );
		}

		void add(Vector2f position, float size, color_t color = color_t::White(), float depth = 0.f) {
			m_circles.push_back({ position, color, size, depth });
		}

		void render(const Matrix3f& view) {
			m_batch.render(view, m_circles.data(), sizeof(Circle) * m_circles.size(), m_circles.size());
			m_circles.clear();
		}

	private:
		using IndexType = uint8_t;
		struct Circle {
			Vector2f position;
			color_t color;
			float size;
			float depth;
		};

		PlainColor2dBatch m_batch;
		std::vector<Circle> m_circles;
	};

	template<bool UseAlpha>
	class LineBatch {
	public:
		using color_t = std::conditional_t<UseAlpha, Color4f, Color3f>;

		LineBatch() : m_batch(ogl::DrawMode::TriangleStrip(), true, true, UseAlpha) {
			static constexpr Vector2f vertices[] { { 0.f, -0.5f }, { 1.f, -0.5f }, { 0.f, 0.5f }, { 1.f, 0.5f } };
			static constexpr IndexType indices[] { 0, 1, 2, 3 };
			m_batch.set_mesh(vertices, 4, indices, sizeof(indices) / sizeof(IndexType));
		}

		void add(Vector2f a, Vector2f b, float width, color_t color = color_t::White(), float depth = 0.f) {
			Vector2 r = b - a;
			m_lines.push_back({ a, color, { r.length() , width }, depth, r.rotation() });
		}

		void add(Vector2f a, float angle, float length, float width, color_t color = color_t::White(), float depth = 0.f) {
			m_lines.push_back({ a, color, { length, width }, depth, angle });
		}

		void render(const Matrix3f& view) {
			m_batch.render(view, m_lines.data(), sizeof(Line) * m_lines.size(), m_lines.size());
			m_lines.clear();
		}

	private:
		using IndexType = uint8_t;
		struct Line {
			Vector2f position;
			color_t color;
			Vector2f size;
			float depth;
			float rotation;
		};

		PlainColor2dBatch m_batch;
		std::vector<Line> m_lines;
	};

	template<bool UseAlpha>
	class RectangleBatch {
	public:
		using color_t = std::conditional_t<UseAlpha, Color4f, Color3f>;

		RectangleBatch() : m_batch(ogl::DrawMode::TriangleStrip(), true, true, UseAlpha) {
			static constexpr Vector2f vertices[] {
				{ -.5f, -.5f },
				{ .5f, -.5f },
				{ -.5f, .5f },
				{ .5f, .5f }
			};
			static constexpr IndexType indices[] { 0, 1, 2, 3 };
			m_batch.set_mesh(vertices, 4, indices, sizeof(indices) / sizeof(IndexType));
		}

		void add(Vector2f position, Vector2f size, float rotation = 0.f, color_t color = color_t::White(), float depth = 0.f) {
			m_rectangles.push_back({ position, color, size, depth, rotation });
		}

		void render(const Matrix3f& view) {
			m_batch.render(view, m_rectangles.data(), sizeof(Rectangle) * m_rectangles.size(), m_rectangles.size());
			m_rectangles.clear();
		}

	private:
		using IndexType = uint8_t;

		struct Rectangle {
			Vector2f position;
			color_t color;
			Vector2f size;
			float depth;
			float rotation;
		};

		PlainColor2dBatch m_batch;
		std::vector<Rectangle> m_rectangles;
	};
}