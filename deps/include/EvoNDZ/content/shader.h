#pragma once
#include "../opengl/util.h"

namespace evo::data {
	class Shader {
	public:
		Shader(evo::ogl::GLenum type);
		GLuint shader() const {
			return m_shader;
		}
		Shader& operator=(Shader&& other) noexcept;
		Shader(Shader&& other) noexcept : m_shader(other.m_shader) {
			other.m_shader = 0;
		}
		static Shader Load(unsigned char* data, uint64_t length);
	private:
		evo::ogl::GLuint m_shader;

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
	};
}