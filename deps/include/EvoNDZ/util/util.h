#pragma once
#include <type_traits>
#include <concepts>
#include "type_traits.h"

namespace evo
{
	template<typename Allocator, typename... Args>
	inline void uninitialized_fill(
		typename Allocator::value_type* first, typename Allocator::value_type* last, Allocator alloc, const Args&... args
	)
		noexcept(std::is_nothrow_constructible_v<typename Allocator::value_type>)
	{
		for (; first != last; ++first) {
			std::allocator_traits<Allocator>::construct(alloc, first, args...);
		}
	}

	template<typename Allocator, typename... Args>
	inline void uninitialized_fill(
		typename Allocator::value_type* first, const size_t n, Allocator alloc, const Args&... args
	)
		noexcept( std::is_nothrow_constructible_v<typename Allocator::value_type> ) 	{
		for (size_t i = 0; i < n; ++i) {
			std::allocator_traits<Allocator>::construct(alloc, first + i, args...);
		}
	}


	constexpr size_t hash_combine(size_t a, size_t b) {
		return a ^ ( b + 0x9e3779b9 + ( a << 6 ) + ( a >> 2 ) );
	}

	template<typename T>
	struct DefaultConstructor {
		void operator()(T* const ptr) const noexcept( std::is_nothrow_default_constructible_v<T> ) {
			std::construct_at(ptr);
		}
	};

	template<typename T>
	struct DefaultDestructor {
		void operator()(T* const ptr) const noexcept( std::is_nothrow_destructible_v<T> ) {
			std::destroy_at(ptr);
		}
	};

	template<typename... TArgs>
	struct NoopFunctionObject {
		void operator()(const TArgs&...) const noexcept { }
	};

	template<typename... Args>
	inline void(*noop_fn_ptr)(Args...) = [](Args... args) { };

	struct Empty { };

	template<bool Test, typename T>
	using ConditionalField = std::conditional_t<Test, T, Empty>;
}