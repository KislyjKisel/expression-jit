#pragma once
#include <concepts>
#include <vector>
#include <memory>
#include <array>
#include <span>
#include "../../util/color4.h"
#include "render_target.h"
#include "parameters.h"
#include "enum/logical_pixel_operation.h"
#include "enum/blend_equation.h"
#include "enum/depth_function.h"
#include "enum/blend_factor.h"
#include "enum/capability.h"
#include "enum/buffer_bit.h"

namespace evo::ogl {
	class Texture2D;
	class VertexBuffer;
	class VertexArray;
	class Technique;
	class IndexBuffer;
}

namespace evo::ogl 
{
	struct ColorMask {
		gl_boolean_t r, g, b, a;
		ColorMask() : r(gl_true), g(gl_true), b(gl_true), a(gl_true) { }
		ColorMask(gl_boolean_t red, gl_boolean_t green, gl_boolean_t blue, gl_boolean_t alpha) 
			: r(red), g(green), b(blue), a(alpha) { }
	};

	class State final {
	private:
		template<typename T, typename F>
		class Storage {
		public:
			Storage() = default;
			
			void set(const T& value) {
				T last = m_values.back();
				m_values.push_back(value);
				if (last != value) m_function(value);
			}
			void reset(const T& value) {
				T last = m_values.back();
				m_values.back() = value;
				if (last != value) m_function(value);
			}
			void revert() {
				T last = m_values.back();
				m_values.pop_back();
				const T& value = m_values.back();
				if (last != value) m_function(value);
			}
			const T& get() const {
				return m_values.back();
			}

			friend class State;
			
		private:
			template<typename... FInitArgs>
			void initialize(const T& value, FInitArgs... finitargs) {
				m_values.push_back(value);
				m_function.initialize(finitargs...);
			}

			void initialize(const T& value) {
				m_values.push_back(value);
			}

			std::vector<T> m_values;
			[[no_unique_address]] F m_function;
		};

	private:
#pragma region State set functions
		struct IndexedFuncBase {
			size_t m_buf;
			void initialize(size_t buf) { m_buf = buf; }
		};
		struct BlendEquationFunc		: IndexedFuncBase	{ void operator()(BlendEquationSeparate) const; };
		struct ScissorTestFunc		: IndexedFuncBase	{ void operator()(gl_boolean_t) const;			};
		struct BlendFunc				: IndexedFuncBase	{ void operator()(gl_boolean_t) const;			};
		struct BlendFunctionFunc		: IndexedFuncBase	{ void operator()(BlendFunction) const;			};
		struct ColorMaskFunc			: IndexedFuncBase	{ void operator()(ColorMask) const;				};
		struct BlendColorFunc		{ void operator()(const Color4f&) const;			};
		struct ClearColorFunc		{ void operator()(const Color4f&) const;			};
		struct ClearDepthFunc		{ void operator()(gl_double_t) const;			};
		struct DepthMaskFunc			{ void operator()(gl_boolean_t) const;			};
		struct DepthFunctionFunc		{ void operator()(DepthFunction) const;			};
		struct LogicOpFunc			{ void operator()(LogicalPixelOperation) const; };
#pragma endregion

	public:
		State();
		~State();

		void clear(BufferBit);

#pragma region Storage getters
		auto& blend_equation(size_t buf)		{ return m_blendEquation[buf];	}
		auto& scissor_test(size_t buf)		{ return m_scissorTest[buf];		}
		auto& blend(size_t buf)				{ return m_blend[buf];			}
		auto& blend_function(size_t buf)		{ return m_blendFunction[buf];	}
		auto& color_mask(size_t buf)			{ return m_colorWritemask[buf];	}
		auto& blend_color()					{ return m_blendColor;			}
		auto& clear_color()					{ return m_clearColor;			}
		auto& clear_depth()					{ return m_clearDepth;			}
		auto& depth_mask()					{ return m_depthWritemask;		}
		auto& depth_function()				{ return m_depthFunction;		}
		auto& logical_pixel_operation()		{ return m_logicalOperation;		}
#pragma endregion
#pragma region Bind/Unbind/Rebind/Revert
		void bind_texture(const ::evo::ogl::Texture2D& texture, int textureUnitIndex);
		void unbind_texture(int textureUnitIndex);
		void rebind_texture(const evo::ogl::Texture2D& texture, int textureUnitIndex);
		void revert_texture(int textureUnitIndex);

		void bind_vertex_buffer(const ::evo::ogl::VertexBuffer& buf);
		void unbind_vertex_buffer();
		void rebind_vertex_buffer(const ::evo::ogl::VertexBuffer& buf);
		void revert_vertex_buffer();

		void bind_vertex_array(const ::evo::ogl::VertexArray& buf);
		void unbind_vertex_array();
		void rebind_vertex_array(const ::evo::ogl::VertexArray& buf);
		void revert_vertex_array();

		void bind_index_buffer(const ::evo::ogl::IndexBuffer& buf);
		void unbind_index_buffer();
		void rebind_index_buffer(const ::evo::ogl::IndexBuffer& buf);
		void revert_index_buffer();

		void bind_technique(const ::evo::ogl::Technique& tch);
		void unbind_technique();
		void rebind_technique(const ::evo::ogl::Technique& tch);
		void revert_technique();
#pragma endregion

		void enable(Capability);
		void disable(Capability);
		void revert(Capability);
		bool is_enabled(Capability) const;

		void rebind_render_target(RenderTarget*);
		void bind_render_target(RenderTarget*);
		void revert_render_target();
		RenderTarget* render_target() const;

	private:
		std::unique_ptr<Storage<BlendEquationSeparate, BlendEquationFunc>[]> m_blendEquation;
		std::unique_ptr<Storage<gl_boolean_t, ScissorTestFunc>[]> m_scissorTest;
		std::unique_ptr<Storage<gl_boolean_t, BlendFunc>[]> m_blend;
		std::unique_ptr<Storage<BlendFunction, BlendFunctionFunc>[]> m_blendFunction;
		std::unique_ptr<Storage<ColorMask, ColorMaskFunc>[]> m_colorWritemask;
		Storage<Color4f, BlendColorFunc> m_blendColor;
		Storage<Color4f, ClearColorFunc> m_clearColor;
		Storage<gl_double_t, ClearDepthFunc> m_clearDepth;
		Storage<gl_boolean_t, DepthMaskFunc> m_depthWritemask;
		Storage<DepthFunction, DepthFunctionFunc> m_depthFunction;
		Storage<LogicalPixelOperation, LogicOpFunc> m_logicalOperation;

		Parameters m_parameters;
		std::vector<gl_boolean_t> m_capabilities[array_size<decltype( Capability::Capabilities() )>];
		std::vector<RenderTarget*> m_renderTarget;
		std::unique_ptr<std::vector<gl_uint_t>[]> m_texture;
		std::vector<gl_uint_t> m_vertexArray;
		std::vector<gl_uint_t> m_vertexBuffer;
		std::vector<gl_uint_t> m_indexBuffer;
		std::vector<gl_uint_t> m_technique;
	};
}