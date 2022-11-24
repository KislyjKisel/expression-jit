#pragma once
#include "../types.h"

namespace evo::ogl
{
	class State;

	struct DepthFunction {
		gl_enum_t value() const noexcept { return m_value; }

		static DepthFunction Never()		noexcept;
		static DepthFunction Less()			noexcept;
		static DepthFunction Equal()		noexcept;
		static DepthFunction LessEqual()	noexcept;
		static DepthFunction Greater()		noexcept;
		static DepthFunction NotEqual()		noexcept;
		static DepthFunction GreaterEqual() noexcept;
		static DepthFunction Always()		noexcept;

		constexpr bool operator==(const DepthFunction& df) const noexcept {
			return m_value == df.m_value;
		}
		constexpr bool operator!=(const DepthFunction& df) const noexcept {
			return m_value != df.m_value;
		}

		friend class State;

	private:
		gl_enum_t m_value;

		constexpr DepthFunction(gl_enum_t value) : m_value(value) { }
	};
}