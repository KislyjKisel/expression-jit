#pragma once
#include "../types.h"

namespace evo::ogl
{
	class State;
	
	struct LogicalPixelOperation {
		gl_enum_t value() const noexcept { return m_value; }

		static LogicalPixelOperation Clear() noexcept;
		static LogicalPixelOperation Set() noexcept;
		static LogicalPixelOperation Copy() noexcept;
		static LogicalPixelOperation CopyInverted() noexcept;
		static LogicalPixelOperation Noop() noexcept;
		static LogicalPixelOperation Invert() noexcept;
		static LogicalPixelOperation And() noexcept;
		static LogicalPixelOperation Nand() noexcept;
		static LogicalPixelOperation Or() noexcept;
		static LogicalPixelOperation Nor() noexcept;
		static LogicalPixelOperation Xor() noexcept;
		static LogicalPixelOperation Equiv() noexcept;
		static LogicalPixelOperation AndReverse() noexcept;
		static LogicalPixelOperation AndInverted() noexcept;
		static LogicalPixelOperation OrReverse() noexcept;
		static LogicalPixelOperation OrInverted() noexcept;

		friend class State;

	private:
		gl_enum_t m_value;

		constexpr LogicalPixelOperation(gl_enum_t value) : m_value(value) { }
	};
}