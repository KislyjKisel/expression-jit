#pragma once
#include <cstdint>
#include <vector>
#include "../math/vector3.h"
#include "../math/vector2.h"
#include "default_format.h"

namespace evo::ogl {
	class Mesh;
}

namespace evo::data {
	class Mesh { //todo: create* funcs
	public:
		bool hasNormals() const {
			return m_hasNormals;
		}
		bool hasTexcoords() const {
			return m_hasTexcoords;
		}
		size_t vertexCount() const {
			return m_positions.size();
		}
		const std::vector<evo::math::Vector3>& positions() const {
			return m_positions;
		}
		const std::vector<evo::math::Vector3>& normals() const {
			return m_normals;
		}
		const std::vector<evo::math::Vector2>& texcoords() const {
			return m_texcoords;
		}
		size_t indexCount() const {
			return m_faces.size() * 3;
		}
		const uint16_t* indices() const {
			return reinterpret_cast<const uint16_t*>(m_faces.data());
		}

		static Mesh Load(unsigned char* data, uint64_t length);
	private:
		struct Face {
			uint16_t p[3];
		};

		bool m_hasNormals;
		bool m_hasTexcoords;
		std::vector<evo::math::Vector3> m_positions;
		std::vector<evo::math::Vector3> m_normals;
		std::vector<evo::math::Vector2> m_texcoords;
		std::vector<Face> m_faces;

		Mesh(unsigned char format, size_t faceCount, size_t vertexCount) : 
			m_hasNormals(format == MeshFormat_PN || format == MeshFormat_PNT), 
			m_hasTexcoords(format == MeshFormat_PT || format == MeshFormat_PNT),
			m_faces(faceCount), m_positions(vertexCount),
			m_normals(m_hasNormals ? vertexCount : 0), m_texcoords(m_hasTexcoords ? vertexCount : 0) { }
	};
}