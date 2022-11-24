#pragma once
#include "../types.h"

namespace evo::ogl
{
	class State;

	struct BlendEquation {
		gl_enum_t value() const noexcept { return m_value; }
		
		static BlendEquation Add()				noexcept;
		static BlendEquation Subtract()			noexcept;
		static BlendEquation ReverseSubtract()	noexcept;
		static BlendEquation Min()				noexcept;
		static BlendEquation Max()				noexcept;

		friend class State;

		constexpr bool operator==(const BlendEquation&) const = default;
		constexpr bool operator!=(const BlendEquation&) const = default;

	private:
		gl_enum_t m_value;

		constexpr BlendEquation(gl_enum_t value) : m_value(value) { }
	};

	struct BlendEquationSeparate {
		BlendEquation rgb, alpha;

		constexpr bool operator==(const BlendEquationSeparate&) const = default;
		constexpr bool operator!=(const BlendEquationSeparate&) const = default;

		constexpr BlendEquationSeparate(BlendEquation rgba) : rgb(rgba), alpha(rgba) { }
		constexpr BlendEquationSeparate(BlendEquation rgb, BlendEquation alpha) : rgb(rgb), alpha(alpha) { }
	};
}