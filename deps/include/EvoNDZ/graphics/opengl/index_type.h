#pragma once
#include <type_traits>
#include <cstdint>
#include "types.h"

namespace evo::ogl
{
	template<typename T>
	concept Index = std::is_same_v<T, uint8_t> || std::is_same_v<T, uint16_t> || std::is_same_v<T, uint32_t>;

	template<Index T>
	gl_enum_t index_type_id() noexcept;
}