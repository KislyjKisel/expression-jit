#pragma once
#include <vector>
#include <numbers>
#include "../opengl/enum/buffer_usage.h"
#include "../batch.h"
#include "../../util/concepts.h"
#include "../../math/matrix3.h"
#include "../../math/vector2.h"
#include "../../util/color4.h"

namespace evo::s2d
{
	class PlainColor2dBatch : public Batch {
	public:
		PlainColor2dBatch(evo::ogl::DrawMode dm, bool useAngle, bool separateSize, bool useAlpha);
		virtual ~PlainColor2dBatch() { }

		void render(const Matrix3f& view, const void* data, size_t dataSize, unsigned instances) {
			m_viewMatrix = &view;
			Batch::render(data, dataSize, instances);
		}

		template<evo::ogl::Index Idx>
		void set_mesh(const Vector2f* v, size_t vc, const Idx* i, size_t ic) {
			set_static_mesh(v, sizeof(Vector2f) * vc, i, ic);
			initialize_static_attribs({ { "offset", ogl::ShaderType::FVec2() } });
		}

	private:
		ogl::gl_int_t m_locViewMatrix;
		const Matrix3f* m_viewMatrix = nullptr;

		void set_uniforms() override;
	};
}