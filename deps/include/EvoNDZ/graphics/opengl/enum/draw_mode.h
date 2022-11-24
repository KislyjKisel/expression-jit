#pragma once
#include "../types.h"

namespace evo::ogl
{
	struct DrawMode {
		gl_enum_t value() const noexcept { return m_value; }

		static DrawMode Points();
		static DrawMode LineStrip();
		static DrawMode LineLoop();
		static DrawMode Lines();
		static DrawMode LineStripAdjacency();
		static DrawMode LinesAdjacency();
		static DrawMode TriangleStrip();
		static DrawMode TriangleFan();
		static DrawMode Triangles();
		static DrawMode TriangleStripAdjacency();
		static DrawMode TrianglesAdjacency();
		static DrawMode Patches();

	private:
		gl_enum_t m_value;

		DrawMode(gl_enum_t value) : m_value(value) { }
	};
}