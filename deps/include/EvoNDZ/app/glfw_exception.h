#pragma once
#include "../../include/EvoNDZ/util/exception.h"

namespace evo
{
	struct GlfwException : Exception {
		GlfwException(const char* text) : Exception(text), m_text(text) { }

		const char* what() const override { return m_text; }

	private:
		const char* m_text;
	};
}