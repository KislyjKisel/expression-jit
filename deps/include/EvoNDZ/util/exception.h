#pragma once
#include "../app/application.h"

namespace evo
{
	struct Exception : std::exception {
		Exception(const char* txt) noexcept {
			EVO_LOG_ERROR(txt, '\n');
		}
	};
}