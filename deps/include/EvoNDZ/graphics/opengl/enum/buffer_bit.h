#pragma once
#include "../types.h"

namespace evo::ogl
{
	struct BufferBit {
		gl_enum_t value() const noexcept { return m_value; }

		static BufferBit Color()		noexcept;
		static BufferBit Depth()		noexcept;
		static BufferBit Stencil()		noexcept;

		constexpr BufferBit operator|(BufferBit bit) const noexcept {
			return BufferBit(m_value | bit.m_value);
		}
		constexpr BufferBit operator&(BufferBit bit) const noexcept {
			return BufferBit(m_value & bit.m_value);
		}

	private:
		gl_enum_t m_value;

		constexpr BufferBit(gl_enum_t value) : m_value(value) { }
	};
}