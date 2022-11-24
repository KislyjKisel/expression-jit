#pragma once
#include <array>
#include "types.h"
#include "enum/texture_format.h"
#include "../../util/array.h"

/*
Depth only - ok
Stencil only - ok
Depth+Stencil separated - not ok
Depth+Stencil combined - ok
*/

namespace evo::ogl {
	class RenderTarget {
	public:
		inline static constexpr size_t MaxColorAttachments = 8; //todo: const->glparam

		RenderTarget(uint32_t width, uint32_t height);
		~RenderTarget();

		bool is_complete() const;
		gl_enum_t status() const;

		uint32_t width() const {
			return m_width;
		}
		uint32_t height() const {
			return m_height;
		}
		static void Bind(RenderTarget* rt);
		static void unbind();

		void attach_color_texture(TextureFormat format, gl_uint_t attachmentIndex = 0);
		void attach_color_renderbuffer(TextureFormat format, gl_uint_t attachmentIndex = 0);
		void attach_depth_texture(TextureFormat format);
		void attach_depth_renderbuffer(TextureFormat format);
		void attach_stencil_texture(TextureFormat format);
		void attach_stencil_renderbuffer(TextureFormat format);
		void attach_depth_stencil_texture(TextureFormat format);
		void attach_depth_stencil_renderbuffer(TextureFormat format);
		gl_uint_t color_texture(gl_uint_t attachmentIndex = 0);
		gl_uint_t color_renderbuffer(gl_uint_t attachmentIndex = 0);
		gl_uint_t move_color_texture(gl_uint_t attachmentIndex = 0);
		gl_uint_t move_color_renderbuffer(gl_uint_t attachmentIndex = 0);
		gl_uint_t depth_texture();
		gl_uint_t depth_renderbuffer();
		gl_uint_t move_depth_texture();
		gl_uint_t move_depth_renderbuffer();
		gl_uint_t stencil_texture();
		gl_uint_t stencil_renderbuffer();
		gl_uint_t move_stencil_texture();
		gl_uint_t move_stencil_renderbuffer();
		gl_uint_t depth_stencil_texture();
		gl_uint_t depth_stencil_renderbuffer();
		gl_uint_t move_depth_stencil_texture();
		gl_uint_t move_depth_stencil_renderbuffer();
		void copy_depth_texture_to(gl_uint_t dest);
		void copy_stencil_texture_to(gl_uint_t dest);
		void copy_depth_stencil_texture_to(gl_uint_t dest);

	private:
		struct ColorAttachment {
			bool texture;
			gl_uint_t name;

			constexpr ColorAttachment(bool istexture, gl_uint_t id) : texture(istexture), name(id) {}
		};

		struct DepthStencilAttachment {
			bool isSeparate = false;

			union {
				struct {
					bool textureDepthStencil;
					gl_uint_t depthStencil;
				} combined;

				struct {
					bool textureDepth;
					bool textureStencil;
					gl_uint_t depth;
					gl_uint_t stencil;
				} separated;
			};

			DepthStencilAttachment() { 
				isSeparate = false;
				combined.depthStencil = gl_invalid;
				combined.textureDepthStencil = false;
			}
		};

		gl_uint_t m_framebuffer;
		uint32_t m_width, m_height;
		std::array<ColorAttachment, MaxColorAttachments> m_colorAttachments 
			= make_array_of<MaxColorAttachments>(ColorAttachment(false, gl_invalid));

		DepthStencilAttachment m_depthStencilAttachment;

		void attach_texture(gl_uint_t* texture, gl_enum_t format, gl_enum_t attachment, gl_int_t minFilter, gl_int_t magFilter);
		void attach_renderbuffer(gl_uint_t* renderbuffer, gl_enum_t format, gl_enum_t attachment);

		static gl_enum_t ColorAttachmentByIndex(gl_uint_t index);
	};
}