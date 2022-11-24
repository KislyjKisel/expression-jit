#include "include/exprjit/function_allocator.h"
#include <Windows.h>

namespace exprjit
{
	void* FunctionAllocator::allocate(const std::span<unsigned char>& m_data) {
		void* execmem = VirtualAlloc(nullptr, m_data.size(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		std::copy(m_data.begin(), m_data.end(), (unsigned char*)execmem);
		DWORD oldprotect;
		if (execmem == nullptr) {
			throw VirtualAllocException(GetLastError());
		}
		if (!VirtualProtect(execmem, m_data.size(), PAGE_EXECUTE_READ, &oldprotect)) {
			throw VirtualProtectException(GetLastError());
		}
		return execmem;
	}

	bool FunctionAllocator::free(void* ptr) noexcept {
		return VirtualFree(ptr, 0, MEM_RELEASE); 
	}
}