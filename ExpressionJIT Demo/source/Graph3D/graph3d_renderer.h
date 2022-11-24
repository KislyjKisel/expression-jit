#pragma once
#include <vector>
#include <memory>
#include <EvoNDZ/graphics/opengl/types.h>
#include <EvoNDZ/math/vector3.h>
#include <EvoNDZ/math/matrix4.h>
#include "../vertex.h"
#include "../camera.h"

namespace evo::ogl
{
	class VertexBuffer;
	class IndexBuffer;
	class VertexArray;
	class Technique;
}

namespace ed
{
	class Graph3dRenderer {
	public:
		Graph3dRenderer();
		~Graph3dRenderer();
		void data(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
		void render(const Camera&, evo::Vector3f light);

	private:
		evo::Matrix4f view;
		evo::Matrix4f projection;
		evo::Matrix4f viewProjection;

		std::unique_ptr<evo::ogl::VertexBuffer> vbo;
		std::unique_ptr<evo::ogl::IndexBuffer> ebo;
		std::unique_ptr<evo::ogl::VertexArray> vao;
		std::unique_ptr<evo::ogl::Technique> technique;

		size_t indexCount = 0;

		evo::ogl::gl_int_t locViewProj;
		evo::ogl::gl_int_t locColor;
		evo::ogl::gl_int_t locLight;

		Graph3dRenderer(const Graph3dRenderer&) = delete;
		Graph3dRenderer& operator=(const Graph3dRenderer&) = delete;
	};
}