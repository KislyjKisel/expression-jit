#pragma once
#include "../types.h"

namespace evo::ogl
{
	struct TextureFormat {
		gl_enum_t index() const noexcept { return m_index; }
		bool color() const noexcept { return m_color; }
		bool depth() const noexcept { return m_depth; }
		bool stencil() const noexcept { return m_stencil; }
		bool sized() const noexcept { return m_sized; }

		static TextureFormat RGBA2();
		static TextureFormat RGBA4();
		static TextureFormat RGBA8();
		static TextureFormat RGBA12();
		static TextureFormat RGBA16();
		static TextureFormat RGBA8S();
		static TextureFormat RGBA16S();
		static TextureFormat RGBA8UI();
		static TextureFormat RGBA16UI();
		static TextureFormat RGBA32UI();
		static TextureFormat RGBA8I();
		static TextureFormat RGBA16I();
		static TextureFormat RGBA32I();
		static TextureFormat RGBA16F();
		static TextureFormat RGBA32F();

		static TextureFormat RGB4();
		static TextureFormat RGB5();
		static TextureFormat RGB8();
		static TextureFormat RGB10();
		static TextureFormat RGB12();
		static TextureFormat RGB16();
		static TextureFormat RGB8S();
		static TextureFormat RGB16S();
		static TextureFormat RGB8UI();
		static TextureFormat RGB16UI();
		static TextureFormat RGB32UI();
		static TextureFormat RGB8I();
		static TextureFormat RGB16I();
		static TextureFormat RGB32I();
		static TextureFormat RGB16F();
		static TextureFormat RGB32F();

		static TextureFormat RG8();
		static TextureFormat RG16();
		static TextureFormat RG8S();
		static TextureFormat RG16S();
		static TextureFormat RG8UI();
		static TextureFormat RG16UI();
		static TextureFormat RG32UI();
		static TextureFormat RG8I();
		static TextureFormat RG16I();
		static TextureFormat RG32I();
		static TextureFormat RG16F();
		static TextureFormat RG32F();

		static TextureFormat R8();
		static TextureFormat R16();
		static TextureFormat R8S();
		static TextureFormat R16S();
		static TextureFormat R8UI();
		static TextureFormat R16UI();
		static TextureFormat R32UI();
		static TextureFormat R8I();
		static TextureFormat R16I();
		static TextureFormat R32I();
		static TextureFormat R16F();
		static TextureFormat R32F();

		static TextureFormat R3G3B2();
		static TextureFormat RGB5A1();
		static TextureFormat RGB10A2();
		static TextureFormat RGB10A2UI();
		static TextureFormat RG11B10F();
		static TextureFormat RGB9E5();
		static TextureFormat RGB565();

		static TextureFormat Depth16();
		static TextureFormat Depth24();
		static TextureFormat Depth32(); //non req
		static TextureFormat Depth32F();
		static TextureFormat Depth24Stencil8();
		static TextureFormat Depth32FStencil8();
		static TextureFormat Stencil1(); //non req
		static TextureFormat Stencil4(); //non req
		static TextureFormat Stencil8();
		static TextureFormat Stencil16(); //non req

		static TextureFormat Stencil();
		static TextureFormat Depth();
		static TextureFormat DepthStencil();
		static TextureFormat Red();
		static TextureFormat Green();
		static TextureFormat Blue();
		static TextureFormat RG();
		static TextureFormat RGB();
		static TextureFormat RGBA();
		static TextureFormat BGR();
		static TextureFormat BGRA();
		static TextureFormat RedI();
		static TextureFormat GreenI();
		static TextureFormat BlueI();
		static TextureFormat RGI();
		static TextureFormat RGBI();
		static TextureFormat RGBAI();
		static TextureFormat BGRI();
		static TextureFormat BGRAI();

	private:
		gl_enum_t m_index;
		bool m_color;
		bool m_depth;
		bool m_stencil;
		bool m_sized;

		TextureFormat(gl_enum_t index, bool color, bool depth, bool stencil, bool sized) 
			: m_index(index), m_color(color), m_depth(depth), m_stencil(stencil), m_sized(sized) { }
	};
}