#pragma once
#include <span>
#include <exception>
#include <cstdint>

namespace exprjit
{
	struct FunctionAllocator {
		static void* allocate(const std::span<unsigned char>&);
		static bool free(void*) noexcept;
	};

	class VirtualProtectException : public std::exception {
	public:
		uint32_t error;
		VirtualProtectException(uint32_t error) noexcept : error(error) { }
	};

	class VirtualAllocException : public std::exception {
	public:
		uint32_t error;
		VirtualAllocException(uint32_t error) noexcept : error(error) { }
	};
}