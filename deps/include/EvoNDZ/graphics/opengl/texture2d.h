#pragma once
#include <vector>
#include "../../util/color4.h"
#include "types.h"

namespace evo::ogl {
	class State;

	class Texture2D {
	public:
		Texture2D(gl_sizei_t width, gl_sizei_t height, gl_enum_t internalFormat, gl_int_t minFilter, gl_int_t magFilter, gl_int_t wraps, gl_int_t wrapt);
		Texture2D(Texture2D&& t) noexcept : m_texture(t.m_texture) { 
			t.m_texture = 0;
		}
		Texture2D& operator=(Texture2D&& other) noexcept;

		~Texture2D();

		void set_border_color(evo::Color4f borderColor);
		void sub_image(gl_int_t xoffset, gl_int_t yoffset, gl_sizei_t width, gl_sizei_t height, gl_enum_t format, gl_enum_t type, const void* pixels);

		
		friend class State;

	private:
		gl_uint_t m_texture;

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
	};
}