#pragma once
#include "vertex_buffer.h"
#include "enum/buffer_usage.h"

namespace evo::ogl {
	class DynamicVertexBuffer {
	public:
		DynamicVertexBuffer(gl_sizeiptr_t capacity) : m_capacity(capacity) {
			m_buffer.data(nullptr, capacity, BufferUsage::DynamicDraw());
		}

		void write(const void* data, gl_sizeiptr_t size);

		void clear() {
			m_size = 0;
		}
		gl_sizeiptr_t size() {
			return m_size;
		}
		gl_sizeiptr_t capacity() {
			return m_capacity;
		}
		const VertexBuffer& buffer() {
			return m_buffer;
		}
		bool valid() const {
			return m_buffer.valid();
		}

		friend class VertexBuffer;

	private:
		VertexBuffer m_buffer;
		gl_sizeiptr_t m_size = 0;
		gl_sizeiptr_t m_capacity;
	};
}