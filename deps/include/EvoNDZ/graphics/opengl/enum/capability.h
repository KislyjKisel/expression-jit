#pragma once
#include <array>
#include "../types.h"

namespace evo::ogl
{
	struct Capability {
		gl_enum_t value() const noexcept { return m_value; }
		size_t index() const noexcept { return m_index; }

		static Capability ClipDistance0() noexcept { return Capabilities()[0]; }
		static Capability ClipDistance1() noexcept { return Capabilities()[1]; }
		static Capability ClipDistance2() noexcept { return Capabilities()[2]; }
		static Capability ClipDistance3() noexcept { return Capabilities()[3]; }
		static Capability ClipDistance4() noexcept { return Capabilities()[4]; }
		static Capability ClipDistance5() noexcept { return Capabilities()[5]; }
		static Capability ClipDistance6() noexcept { return Capabilities()[6]; }
		static Capability ClipDistance7() noexcept { return Capabilities()[7]; }
		static Capability ColorLogicOp() noexcept { return Capabilities()[8]; }
		static Capability CullFace() noexcept { return Capabilities()[9]; }
		static Capability DebugOutput() noexcept { return Capabilities()[10]; }
		static Capability DebugOutputSync() noexcept { return Capabilities()[11]; }
		static Capability DepthClamp() noexcept { return Capabilities()[12]; }
		static Capability DepthTest() noexcept { return Capabilities()[13]; }
		static Capability Dither() noexcept { return Capabilities()[14]; }
		static Capability FramebufferSRGB() noexcept { return Capabilities()[15]; }
		static Capability LineSmooth() noexcept { return Capabilities()[16]; }
		static Capability Multisample() noexcept { return Capabilities()[17]; }
		static Capability PolygonOffsetFill() noexcept { return Capabilities()[18]; }
		static Capability PolygonOffsetLine() noexcept { return Capabilities()[19]; }
		static Capability PolygonOffsetPoint() noexcept { return Capabilities()[20]; }
		static Capability PolygonSmooth() noexcept { return Capabilities()[21]; }
		static Capability PrimitiveRestart() noexcept { return Capabilities()[22]; }
		static Capability PrimitiveRestartFixedIndex() noexcept { return Capabilities()[23]; }
		static Capability RasterizerDiscard() noexcept { return Capabilities()[24]; }
		static Capability SampleAlphaToCoverage() noexcept { return Capabilities()[25]; }
		static Capability SampleAlphaToOne() noexcept { return Capabilities()[26]; }
		static Capability SampleCoverage() noexcept { return Capabilities()[27]; }
		static Capability SampleShading() noexcept { return Capabilities()[28]; }
		static Capability SampleMask() noexcept { return Capabilities()[29]; }
		static Capability StencilTest() noexcept { return Capabilities()[30]; }
		static Capability TextureCubeMapSeamless() noexcept { return Capabilities()[31]; }
		static Capability ProgramPointSize() noexcept { return Capabilities()[32]; }

		static const std::array<Capability, 33>& Capabilities();

	private:
		gl_enum_t m_value;
		size_t m_index;
		//todo: INDEXED? true/false. If not indexed - dont use in State::enable/disable. otherwise consider storing range and make generic enable(cap, i)

		constexpr Capability(gl_enum_t value, size_t index) : m_value(value), m_index(index) { }
	};
}