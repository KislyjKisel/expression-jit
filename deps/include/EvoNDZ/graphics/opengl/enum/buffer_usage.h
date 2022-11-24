#pragma once
#include "../types.h"

namespace evo::ogl
{
	struct BufferUsage {
		gl_enum_t value() const noexcept { return m_value; }

		static BufferUsage DynamicDraw();
		static BufferUsage StaticDraw();
		static BufferUsage StreamDraw();
		static BufferUsage DynamicRead();
		static BufferUsage StaticRead();
		static BufferUsage StreamRead();
		static BufferUsage DynamicCopy();
		static BufferUsage StaticCopy();
		static BufferUsage StreamCopy();

	private:
		gl_enum_t m_value;

		BufferUsage(gl_enum_t value) : m_value(value) { }
	};
}