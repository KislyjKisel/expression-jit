#pragma once
#include <vector>
#include <memory>
#include "opengl/enum/draw_mode.h"
#include "opengl/enum/shader_type.h"
#include "opengl/vertex_buffer.h"
#include "opengl/index_buffer.h"
#include "opengl/vertex_array.h"
#include "opengl/technique.h"
#include "opengl/types.h"


namespace evo
{
	class Batch {
	public:
		Batch(ogl::DrawMode drawMode) : m_drawMode(drawMode) { }
		virtual ~Batch() { }

		void render(const void* data, size_t dataSize, unsigned instances);

		struct AttributeInfo {
			const char* glslName;
			ogl::ShaderType glslType;

			AttributeInfo(const char* glslName, ogl::ShaderType glslType) : glslName(glslName), glslType(glslType) { }

			AttributeInfo(const AttributeInfo&) = default;
			AttributeInfo& operator=(const AttributeInfo&) = default;

			friend class Batch;

		private:
			ogl::gl_sizei_t offset = 0;
			ogl::gl_int_t location = 0;
		};


	protected:
		void replace_static_attribs(const std::vector<AttributeInfo>&);
		void replace_dynamic_attribs(const std::vector<AttributeInfo>&);
		void initialize_static_attribs(const std::vector<AttributeInfo>&);
		void initialize_dynamic_attribs(const std::vector<AttributeInfo>&);

		template<ogl::Index T>
		void set_static_mesh(const void* data, ogl::gl_sizeiptr_t dataSize, const T* indices, size_t indexCount);
		void set_technique(ogl::Technique*);

		virtual void set_uniforms() = 0;

	private:
		std::vector<AttributeInfo> m_attrStatic;
		std::vector<AttributeInfo> m_attrDynamic;

		ogl::VertexBuffer m_vbStatic;
		ogl::VertexBuffer m_vbDynamic;
		ogl::IndexBuffer m_indexBuffer;
		ogl::VertexArray m_vertexArray;
		std::unique_ptr<ogl::Technique> m_technique = nullptr;

		size_t m_vbDynamicSize = 0;
		
		ogl::DrawMode m_drawMode;

		ogl::gl_sizei_t m_strideStatic = 0;
		ogl::gl_sizei_t m_strideDynamic = 0;
		ogl::gl_enum_t m_indexType = 0;
		size_t m_indexCount = 0;

		void setup_static_attribs();
		void setup_dynamic_attribs();
		void disable_static_attribs();
		void disable_dynamic_attribs();
		void init_static_attribs();
		void init_dynamic_attribs();

		Batch(const Batch&) = delete;
		Batch& operator=(const Batch&) = delete;
	};
}