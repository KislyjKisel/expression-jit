#pragma once
#include "name.h"

namespace evo::content
{
	template<typename T>
	struct Reference {
		union {
			Name name;
			const T* pointer;
		};

		Reference(std::string_view archive, std::string_view entry) : name(archive, entry) { }
	};
}