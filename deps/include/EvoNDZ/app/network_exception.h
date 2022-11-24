#pragma once
#include "../util/exception.h"

namespace evo
{
	struct NetworkException : public Exception {
		NetworkException(const char* text) : Exception(text), m_text(text) { }

		const char* what() const override { return m_text; }

	private:
		const char* m_text;
	};
}