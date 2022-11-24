#include "graph3d_renderer.h"
#include <numbers>
#include <glad/glad.h>
#include <EvoNDZ/math/vector3.h>
#include <EvoNDZ/graphics/opengl/state.h>
#include <EvoNDZ/graphics/opengl/vertex_buffer.h>
#include <EvoNDZ/graphics/opengl/vertex_array.h>
#include <EvoNDZ/graphics/opengl/index_buffer.h>
#include <EvoNDZ/graphics/opengl/technique.h>
#include <EvoNDZ/graphics/opengl/enum/buffer_usage.h>
#include <EvoNDZ/util/color4.h> 
#include "../params.h"

namespace ed
{
	Graph3dRenderer::Graph3dRenderer() {
		projection.set_projection_perspective(1.0, (float)WindowWidth / WindowHeight, 0.5, 100.0);
		evo::app::gl()->enable(evo::ogl::Capability::DepthTest());

		vbo = std::make_unique<evo::ogl::VertexBuffer>();
		ebo = std::make_unique<evo::ogl::IndexBuffer>();

		evo::ogl::Technique::Builder tchb;

		char* sherr1 = tchb.attach_shader(evo::ogl::Technique::Builder::VertexShader, R"(
			#version 460
			
			uniform mat4 ViewProjection;

			layout(location = 0) in vec3 Position;
			layout(location = 1) in vec3 Normal;

			out vec3 fNormal;
			out vec3 fPosition;

			void main() {
				fNormal = Normal;
				fPosition = Position;
				gl_Position = ViewProjection * vec4(Position, 1.0);
			}
		)");

		char* sherr2 = tchb.attach_shader(evo::ogl::Technique::Builder::FragmentShader, R"(
			#version 460

			uniform vec4 Color;
			uniform vec3 Light;

			in vec3 fNormal;
			in vec3 fPosition;
			out vec4 FragColor;

			void main() {
				vec3 ldir = normalize(Light - fPosition);
				FragColor = Color * clamp(abs(dot(fNormal, ldir)), 0.1, 1.0);
			}
		)");
		technique = std::make_unique<evo::ogl::Technique>(tchb.link());

		locViewProj = technique->uniform_location("ViewProjection");
		locColor = technique->uniform_location("Color");
		locLight = technique->uniform_location("Light");

		vao = std::make_unique<evo::ogl::VertexArray>();
		evo::app::gl()->bind_vertex_array(*vao);
		evo::app::gl()->bind_index_buffer(*ebo);
		evo::app::gl()->bind_vertex_buffer(*vbo);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, p));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, n));

		evo::app::gl()->revert_vertex_array();
		evo::app::gl()->revert_vertex_buffer();
		evo::app::gl()->revert_index_buffer();
	}


	Graph3dRenderer::~Graph3dRenderer() { }

	void Graph3dRenderer::data(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
		vbo->data(vertices.data(), vertices.size() * sizeof(Vertex), evo::ogl::BufferUsage::StaticDraw());
		ebo->data(indices.data(), indices.size(), evo::ogl::BufferUsage::StaticDraw());
		indexCount = indices.size();
	}

	void Graph3dRenderer::render(const Camera& cam, evo::Vector3f light) {
		evo::app::gl()->clear(evo::ogl::BufferBit::Color() | evo::ogl::BufferBit::Depth());

		if (indexCount == 0) return;

		evo::Matrix4f::Multiply(viewProjection, projection, cam.view_matrix());

		evo::app::gl()->bind_technique(*technique);
		evo::app::gl()->bind_vertex_array(*vao);

		glUniformMatrix4fv(locViewProj, 1, GL_TRUE, viewProjection.data());
		glUniform4f(locColor, 1.0, 0.0, 0.0, 1.0);
		glUniform3f(locLight, light.x, light.y, light.z);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

		evo::app::gl()->revert_vertex_array();
		evo::app::gl()->revert_technique();
	}
}