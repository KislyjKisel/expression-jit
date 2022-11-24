#pragma once
#include <vector>
#include "types.h"

namespace evo::ogl
{
	class State;

	class Technique {
	public:
		class Builder {
		public:
			static const gl_uint_t VertexShader;
			static const gl_uint_t FragmentShader;
			static const gl_uint_t GeometryShader;
			static const gl_uint_t ComputeShader;

			Builder();
			char* attach_shader(gl_enum_t type, const char* source);
			char* attach_shader(gl_enum_t type, const char* const* sourceLines, size_t sourceLineCount);
			Technique link();

		private:
			struct AttachedShader {
				gl_uint_t shader;
				bool owned;

				AttachedShader(gl_uint_t shader_, bool owned_) : shader(shader_), owned(owned_) { }
			};

			gl_uint_t m_program;
			gl_int_t m_compiled;
			std::vector<AttachedShader> m_shaders;

			Builder(const Builder&) = delete;
			Builder& operator=(const Builder&) = delete;
		};

		~Technique();
		Technique(Technique&& other) noexcept : m_program(other.m_program) {
			other.m_program = 0;
		}
		Technique& operator=(Technique&& other) noexcept;

		gl_int_t uniform_location(const gl_char_t* name) const;
		gl_int_t attrib_location(const gl_char_t* name) const;

		friend class State;

	protected:
		Technique(gl_uint_t program);

		gl_uint_t m_program;

		Technique(const Technique&) = delete;
		Technique& operator=(const Technique&) = delete;
	};
}