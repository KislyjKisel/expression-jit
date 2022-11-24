#pragma once
#include <cstddef>
#include "enum/storage_flags.h"
#include "enum/buffer_usage.h"
#include "types.h"

namespace evo::ogl
{
	class State;
	class DynamicVertexBuffer;

	class VertexBuffer {
	public:
		VertexBuffer();
		~VertexBuffer();
		VertexBuffer(DynamicVertexBuffer&& dvb) noexcept;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept;

		VertexBuffer(VertexBuffer&& vb) noexcept : m_buffer(vb.m_buffer) {
			vb.m_buffer = 0;
		}

		void data(const void* data, gl_sizeiptr_t size, BufferUsage usage);
		void sub_data(const void* data, gl_sizeiptr_t size, gl_intptr_t offset);
		void storage(const void* data, gl_sizeiptr_t size, StorageFlags);

		bool valid() const noexcept {
			return m_buffer > 0;
		}

		friend class State;
		friend class DynamicVertexBuffer;

	private:
		gl_uint_t m_buffer;

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer&	operator=(const VertexBuffer&) = delete;
	};
}