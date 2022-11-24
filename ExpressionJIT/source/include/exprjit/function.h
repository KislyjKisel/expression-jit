#pragma once
#include <span>
#include <type_traits>
#include <exception>
#include <cstdint>
#include "function_allocator.h"

namespace exprjit
{
	template<typename UnusedType>
	class Function;

	template<typename ReturnType, typename... ArgumentTypes>
	class Function<ReturnType(ArgumentTypes...)> {
	public:
		typedef ReturnType(*function_type)(ArgumentTypes...);

		Function(const std::span<unsigned char>& binary) { 
			m_memory = FunctionAllocator::allocate(binary);
		}

		Function(Function&& f) noexcept : m_memory(f.m_memory) {
			f.m_memory = nullptr;
		}

		Function& operator=(Function&& f) noexcept {
			if (m_memory != nullptr) FunctionAllocator::free(m_memory);

			m_memory = f.m_memory;
			f.m_memory = nullptr;
		}

		~Function() noexcept {
			FunctionAllocator::free(m_memory); // check errors
		}

		function_type ptr() {
			return m_function;
		}

		ReturnType operator()(ArgumentTypes... args) const noexcept {
			return m_function(args...);
		}

	private:
		union {
			ReturnType (*m_function)(ArgumentTypes...);
			void* m_memory;
		};
	};
}