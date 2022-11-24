#pragma once
#include "attribute_info.h"
#include "vertex_buffer.h"
#include "vertex_array.h"
#include "index_buffer.h"
#include "../data/mesh.h"
#include "../vertex.h"
#include "../util.h"
#include "util.h"

namespace evo::ogl
{
	class Mesh {
	public:
		Mesh(GLenum mode) : m_mode(mode) { }
		Mesh(const evo::data::Mesh& mesh) : m_mode(GL_TRIANGLES), m_indicesCount(mesh.indexCount()) {
			size_t bufSize = mesh.vertexCount() * (3ui64 + mesh.hasNormals() * 3ui64 + mesh.hasTexcoords() * 2ui64);
			float* buffer = new float[bufSize];
			size_t bi = 0;
			for (size_t vi = 0; vi < mesh.vertexCount(); ++vi) {
				auto p = mesh.positions()[vi];
				buffer[bi++] = p.x;
				buffer[bi++] = p.y;
				buffer[bi++] = p.z;
				if (mesh.hasNormals()) {
					auto n = mesh.normals()[vi];
					buffer[bi++] = n.x;
					buffer[bi++] = n.y;
					buffer[bi++] = n.z;
				} 
				if (mesh.hasTexcoords()) {
					auto t = mesh.texcoords()[vi];
					buffer[bi++] = t.x;
					buffer[bi++] = t.y;
				}
			}
			vertexData(buffer, bufSize * sizeof(float), GL_STATIC_DRAW);
			delete[] buffer;
			indexData(mesh.indices(), mesh.indexCount() * sizeof(index_type), GL_STATIC_DRAW);
			setupAttributes(m_defaultAttribs[
				mesh.hasNormals() ? 
					(mesh.hasTexcoords() ? 2 : 1) :
					(mesh.hasTexcoords() ? 3 : 0)
			]);
		}
		void vertexData(const void* data, size_t size, GLenum usage) {
			m_vertexBuffer.data(data, size, usage);
		}
		void vertexSubData(const void* data, size_t size, intptr_t offset) {
			m_vertexBuffer.subData(data, size, offset);
		}
		void indexData(const index_type* indices, size_t size, GLenum usage) {
			m_indexBuffer.data(indices, size, usage);
		}
		void indexSubData(const index_type* indices, size_t size, intptr_t offset) {
			m_indexBuffer.subData(indices, size, offset);
		}
		Mesh(Mesh&& other) noexcept : m_mode(other.m_mode), m_indicesCount(other.m_indicesCount),
			m_vertexBuffer(std::move(other.m_vertexBuffer)), 
			m_vertexArray(std::move(other.m_vertexArray)),
			m_indexBuffer(std::move(other.m_indexBuffer)) {}
		Mesh& operator=(Mesh&& other) noexcept {
			if (this != &other) {
				m_mode = other.m_mode;
				m_indicesCount = other.m_indicesCount;
				m_vertexBuffer = std::move(other.m_vertexBuffer);
				m_vertexArray = std::move(other.m_vertexArray);
				m_indexBuffer = std::move(other.m_indexBuffer);
			}
		}
		size_t indicesCount() const {
			return m_indicesCount;
		}
		void setIndicesCount(size_t indicesCount) {
			m_indicesCount = indicesCount;
		}
		GLenum mode() const {
			return m_mode;
		}

		void setupAttributes(const std::vector<AttributeInfo>& attribs) {
			state::BindVertexArray(m_vertexArray);
			state::BindVertexBuffer(m_vertexBuffer);
			state::BindIndexBuffer(m_indexBuffer);
			for (size_t i = 0; i < attribs.size(); ++i) {
				m_vertexArray.enableAttribute(attribs[i].index);
				glVertexAttribPointer(attribs[i].index, attribs[i].size, attribs[i].type, attribs[i].normalized, attribs[i].stride, (const void*)attribs[i].offset);
			}
			state::RevertVertexArray();
			state::RevertVertexBuffer();
			state::RevertIndexBuffer();
		}

		//todo: for tests !
		const evo::ogl::VertexBuffer& vertexBuffer() const { return m_vertexBuffer; };
		const evo::ogl::VertexArray& vertexArray() const { return m_vertexArray; };
		const evo::ogl::IndexBuffer& indexBuffer() const { return m_indexBuffer; };

	private:
		evo::ogl::VertexBuffer m_vertexBuffer;
		evo::ogl::VertexArray m_vertexArray;
		evo::ogl::IndexBuffer m_indexBuffer;
		size_t m_indicesCount = 0;
		GLenum m_mode;

		static inline std::vector<AttributeInfo> m_defaultAttribs[4]{
			{ AttributeInfo(0, 3, GL_FLOAT, GL_FALSE, sizeof(evo::VertexP), 0)  },

			{ 
				AttributeInfo(0, 3, GL_FLOAT, GL_FALSE, sizeof(evo::VertexPN), 0), 
				AttributeInfo(1, 3, GL_FLOAT, GL_FALSE, sizeof(evo::VertexPN), 12) 
			},
			
			{
				AttributeInfo(0, 3, GL_FLOAT, GL_FALSE, sizeof(evo::VertexPNT), 0),
				AttributeInfo(1, 3, GL_FLOAT, GL_FALSE, sizeof(evo::VertexPNT), 12),
				AttributeInfo(2, 2, GL_FLOAT, GL_FALSE, sizeof(evo::VertexPNT), 24)
			},

			{
				AttributeInfo(0, 3, GL_FLOAT, GL_FALSE, sizeof(evo::VertexPT), 0),
				AttributeInfo(1, 2, GL_FLOAT, GL_FALSE, sizeof(evo::VertexPT), 12)
			}
		};
	};
}