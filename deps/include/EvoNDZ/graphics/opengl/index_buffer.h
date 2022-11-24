#pragma once
#include <cstdint>
#include "enum/storage_flags.h"
#include "enum/buffer_usage.h"
#include "index_type.h"
#include "types.h"

namespace evo::ogl
{
	class State;

	class IndexBuffer {
	public:
		IndexBuffer();
		~IndexBuffer();
		IndexBuffer& operator=(IndexBuffer&& other) noexcept;

		IndexBuffer(IndexBuffer&& vb) noexcept : m_buffer(vb.m_buffer) {
			vb.m_buffer = 0;
		}

		template<Index T>
		void data(const T* indices, size_t count, BufferUsage usage);
		template<Index T>
		void sub_data(const T* indices, size_t count, gl_intptr_t offset);
		template<Index T>
		void storage(const T* indices, size_t count, StorageFlags);

		bool valid() const noexcept {
			return m_buffer > 0;
		}

		friend class State;

	private:
		gl_uint_t m_buffer;

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
	};
}