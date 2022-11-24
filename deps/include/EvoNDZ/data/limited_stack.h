#pragma once
#include <stdexcept>
#include <utility>
#include <memory>
#include <tuple>
#include <new>
#include "util.h"
#include "../util/concepts.h"

namespace evo
{
	template<typename T, size_t Capacity>
	class LimitedStack {
	public:
#pragma region Typedefs

		using value_type = T;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using difference_type = ptrdiff_t;
		using size_type = size_t;

#pragma endregion 

#pragma region Iterators

		//todo: LimitedStack (up-to) iterators

#pragma endregion

#pragma region Constructors & Destructors & op=

		LimitedStack() noexcept : m_size(0) { }

		template<size_t C> requires (Capacity >= C)
		LimitedStack(const LimitedStack<T, C>& other) noexcept( std::is_nothrow_copy_constructible_v<T> ) : m_size(other.m_size) {
			for (size_t i = 0; i < m_size; ++i) {
				copy_construct_(i, other[i]);
			}
		}

		template<size_t C> requires (Capacity >= C)
		LimitedStack(LimitedStack<T, C>&& other) noexcept( std::is_nothrow_move_constructible_v<T> ) : m_size(other.m_size) {
			for (size_t i = 0; i < m_size; ++i) {
				std::construct_at(ptr_(i), std::move(other[i]));
				std::destroy_at(other.ptr_(i));
			}
			other.m_size = 0;
		}

		template<typename... Ts> requires AllSame<T, Ts...> && ( sizeof...( Ts ) <= Capacity )
		LimitedStack(const Ts&... values) noexcept( std::is_nothrow_copy_constructible_v<T> ) : m_size(sizeof...( Ts )) {
			copy_construct_(0, values...);
		}

		~LimitedStack() noexcept( std::is_nothrow_destructible_v<T> ) {
			destroy_();
		}

		template<size_t C> requires (Capacity >= C)
		LimitedStack& operator=(LimitedStack<T, C>&& other) noexcept( std::is_nothrow_move_constructible_v<T> ) {
			destroy_();
			m_size = other.m_size;
			for (size_t i = 0; i < m_size; ++i) {
				std::construct_at(ptr_(i), std::move(other[i]));
			}
			other.m_size = 0;
			return *this;
		}

		template<size_t C> requires ( Capacity >= C )
		LimitedStack& operator=(const LimitedStack<T, C>& s) noexcept( std::is_nothrow_copy_assignable_v<T> ) {
			if constexpr (!std::is_trivially_destructible_v<T>) {
				for (size_t i = m_size; i-- > s.m_size; ) std::destroy_at(ptr_(i));
			}
			m_size = s.m_size;
			for (size_t i = 0; i < s.m_size; ++i) ( *this )[i] = s[i];
			return *this;
		}

#pragma endregion 

#pragma region Observers

		constexpr size_t size() const noexcept { 
			return m_size; 
		}

		constexpr size_t capacity() const noexcept {
			return Capacity;
		}

		constexpr bool empty() const noexcept {
			return m_size == 0;
		}

#pragma endregion

#pragma region Modifiers

		void push_back(const T& value) noexcept(std::is_copy_constructible_v<T>) {
			std::construct_at(ptr_(m_size++), value);
		}

		template<typename... TArgs>
		void emplace_back(TArgs&&... args) noexcept( std::is_constructible_v<T, TArgs...> ) {
			std::construct_at(ptr_(m_size++), std::forward<TArgs>(args)...);
		}

		void pop_back() noexcept( std::is_nothrow_destructible_v<T> ) {
			if constexpr(std::is_trivially_destructible_v<T>) {
				--m_size;
			}
			else {
				std::destroy_at(ptr_(--m_size));
			}
		}

#pragma endregion

#pragma region Storage

		void resize(size_type s) {
			while (m_size > s) std::destroy_at(ptr_(--m_size));
			while (m_size < s) std::construct_at(ptr_(++m_size));
		}

#pragma endregion

#pragma region Element access

		const T& front() const noexcept {
			return this->operator[](0);
		}

		T& front() noexcept {
			return this->operator[](0);
		}

		const T& back() const noexcept {
			return this->operator[](m_size - 1);
		}

		T& back() noexcept {
			return this->operator[](m_size - 1);
		}

		const T& operator[](size_t i) const noexcept {
			return *ptr_(i);
		}

		T& operator[](size_t i) noexcept {
			return *ptr_(i);
		}

		const T& at(size_t i) const { 
			if (i >= m_size) throw std::out_of_range();
			return *ptr_(i);
		}

		T& at(size_t i) {
			if (i >= m_size) throw std::out_of_range();
			return *ptr_(i);
		}

#pragma endregion

	private:
		std::aligned_storage_t<sizeof(T), alignof( T )> m_data[Capacity];
		size_t m_size;

		// ? what's launder ? needed? wakaranai(
		T* ptr_(size_t i) {
			return std::launder(reinterpret_cast<T*>( &m_data[i] ));
		}
		const T* ptr_(size_t i) const {
			return std::launder(reinterpret_cast<const T*>( &m_data[i] ));
		}

		void destroy_() noexcept( std::is_nothrow_destructible_v<T> ) {
			if constexpr (!std::is_trivially_destructible_v<T>) {
				for (size_t i = m_size; i-- > 0; ) std::destroy_at(ptr_(i));
			}
		}

		void copy_construct_(size_t i, const T& value) noexcept( std::is_nothrow_copy_constructible_v<T> ) {
			std::construct_at(ptr_(i), value);
		}

		template<typename... Ts>
		void copy_construct_(size_t i, const T& value, const Ts&... others) noexcept( std::is_nothrow_copy_constructible_v<T> ) {
			copy_construct_(i, value);
			copy_construct_(i + 1, others...);
		}
	};

	template<typename T, size_t Ca, size_t Cb>
	constexpr bool operator==(const LimitedStack<T, Ca>& a, const LimitedStack<T, Cb>& b) {
		if (a.size() != b.size()) return false;
		for (size_t i = 0; i < a.size(); ++i) {
			if (a[i] != b[i]) return false;
		}
		return true;
	}

	template<typename T, size_t Ca, size_t Cb>
	constexpr bool operator!=(const LimitedStack<T, Ca>& a, const LimitedStack<T, Cb>& b) {
		return !(a == b);
	}
}