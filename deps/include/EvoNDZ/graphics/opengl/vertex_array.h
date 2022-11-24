#pragma once
#include "types.h"

namespace evo::ogl {
	class State;

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();
		VertexArray& operator=(VertexArray&& other) noexcept;

		VertexArray(VertexArray&& other) noexcept : m_vao(other.m_vao) {
			other.m_vao = 0;
		}

		void enable_attribute(gl_uint_t index);
		void disable_attribute(gl_uint_t index);
		void generate();

		bool valid() const {
			return m_vao > 0;
		}

		friend class State;

	private:
		gl_uint_t m_vao;

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
	};
}