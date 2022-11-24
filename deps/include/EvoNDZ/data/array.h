#pragma once
#include <type_traits>
#include <algorithm>
#include <stdexcept>
#include <compare>
#include <memory>
#include "../util/util.h"

namespace evo
{
	template<typename T, typename Allocator = std::allocator<T>> 
	class Array {
	public:
#pragma region Typedefs

		using value_type = T;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;
		using allocator_type = Allocator;

#pragma endregion

#pragma region Iterators

		template<typename TPointer, typename TReference>
		class iterator_impl {
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = typename Array::value_type;
			using reference = TReference;
			using pointer = TPointer;
			using iterator_category = std::random_access_iterator_tag;

			iterator_impl() : m_pointer(nullptr) { }

			reference operator*() const {
				return *m_pointer;
			}
			pointer operator->() const {
				return m_pointer;
			}

			iterator_impl& operator++() {
				++m_pointer;
				return *this;
			}
			iterator_impl operator++(int) {
				return m_pointer++;
			}
			iterator_impl& operator--() {
				--m_pointer;
				return *this;
			}
			iterator_impl operator--(int) {
				return m_pointer--;
			}
			iterator_impl& operator+=(size_type a) {
				m_pointer += a;
				return *this;
			}
			iterator_impl& operator-=(size_type a) {
				m_pointer -= a;
				return *this;
			}

			iterator_impl operator+(size_type a) const {
				return iterator_impl(m_pointer + a);
			}
			friend iterator_impl operator+(size_type a, iterator_impl it) {
				return iterator_impl(it.m_pointer + a);
			}
			iterator_impl operator-(size_type a) const {
				return iterator_impl(m_pointer - a);
			}
			difference_type operator-(iterator_impl it) const {
				return m_pointer - it.m_pointer;
			}

			constexpr auto operator<=>(const iterator_impl&) const noexcept = default;

			friend class Array;
		private:
			pointer m_pointer;

			iterator_impl(pointer p) : m_pointer(p) { }
		};

		using iterator = iterator_impl<pointer, reference>;
		using const_iterator = iterator_impl<const_pointer, const_reference>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		iterator begin() { return iterator(m_pointer); }
		iterator end() { return iterator(m_pointer + m_size); }
		reverse_iterator rbegin() { return reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }

		const_iterator cbegin()	const { return const_iterator(m_pointer); }
		const_iterator cend() const { return const_iterator(m_pointer + m_size); }
		const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }
		const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }

		const_iterator			begin	() const { return cbegin(); }
		const_iterator			end		() const { return cend();	}
		const_reverse_iterator	rbegin	() const { return crbegin();}
		const_reverse_iterator	rend		() const { return crend();	}

#pragma endregion

#pragma region Constructors & Destructors & op=

		~Array() { free_(); }

		template<typename... TArgs> requires std::is_constructible_v<T, const TArgs&...>
		explicit Array(size_type size, const TArgs&... args) : m_size(size)
		{ 
			m_pointer = m_alloc.allocate(size);
			uninitialized_fill(m_pointer, size, m_alloc, args...);
		}

		template<typename... TArgs> requires std::is_constructible_v<T, TArgs...>
		Array(size_type size, const allocator_type& alloc, TArgs&&... args) 
			: Array(size, std::forward<TArgs>(args)...), m_alloc(alloc) { }
		
		Array(const Array& other) noexcept( std::is_nothrow_copy_constructible_v<T> )
			: m_size(other.m_size), m_alloc(other.m_alloc)
		{
			alloc_copy_(other);
		}

		template<typename U> requires std::is_constructible_v<T, const U&>
		Array(const Array<U, allocator_type>& arr) noexcept( std::is_nothrow_constructible_v<T, const U&> )
			: m_size(arr.m_size), m_alloc(arr.m_alloc)
		{ 
			alloc_copy_(arr);
		}

		template<typename U, typename UAlloc> 
		requires std::is_constructible_v<T, const U&> && std::is_default_constructible_v<allocator_type>
		Array(const Array<U, UAlloc>& arr) noexcept(std::is_nothrow_constructible_v<T, const U&>)
			: m_size(arr.m_size)
		{ 
			alloc_copy_(arr);
		}

		template<std::input_iterator InputIt> 
		requires std::constructible_from<T, const typename std::iterator_traits<InputIt>::value_type&>
		Array(InputIt first, InputIt last, const allocator_type& alloc = {}) 
			noexcept(std::is_nothrow_constructible_v<T, const typename std::iterator_traits<InputIt>::value_type&>)
			: m_size(std::distance(first, last)), m_alloc(alloc)
		{
			m_pointer = m_alloc.allocate(m_size);
			for (auto p = m_pointer; first != last; ++first, ++p) {
				std::allocator_traits<allocator_type>::construct(m_alloc, p, *first); 
			}
		}

		Array(Array&& arr) noexcept : m_size(arr.m_size), m_pointer(arr.m_pointer), m_alloc(std::move(arr.m_alloc)) { 
			arr.m_pointer = nullptr; 
			arr.m_size = 0;
		}

		template<typename U, typename UAlloc> requires std::is_constructible_v<T, const U&>
		Array& operator=(const Array<U, UAlloc>& arr) noexcept(std::is_nothrow_constructible_v<T, const U&>) {
			free_();
			m_size = arr.m_size;			
			m_pointer = m_alloc.allocate(m_size);
			for (size_t i = 0; i < m_size; ++i) {
				std::allocator_traits<allocator_type>::construct(m_alloc, m_pointer + i, arr[i]);
			}
			return *this;
		}

		Array& operator=(Array&& arr) {
			free_();
			m_size = arr.m_size;
			m_pointer = arr.m_pointer;
			m_alloc = std::move(arr.m_alloc);
			arr.m_pointer = nullptr;
			arr.m_size = 0;
			return *this;
		}

#pragma endregion

#pragma region Observers

		constexpr size_t size() const noexcept {
			return m_size;
		}

		constexpr std::make_signed_t<size_t> ssize() const noexcept {
			return static_cast<std::make_signed_t<size_t>>( m_size );
		}

		constexpr size_t bytes() const noexcept {
			return m_size * sizeof(T);
		}

		constexpr [[nodiscard]] bool empty() const noexcept {
			return m_size == 0;
		}

		constexpr pointer data() noexcept {
			return m_pointer;
		}
		
		constexpr const_pointer data() const noexcept {
			return m_pointer;
		}

		constexpr allocator_type get_allocator() const noexcept {
			return m_alloc;
		}

#pragma endregion

#pragma region Lookup

		constexpr reference operator[](size_type i) { 
#ifndef NDEBUG
			if (i >= m_size) throw std::out_of_range("evo::Array subscript out of range.");
#endif
			return m_pointer[i];
		}
		constexpr const_reference operator[](size_type i) const {
#ifndef NDEBUG
			if (i >= m_size) throw std::out_of_range("evo::Array subscript out of range.");
#endif
			return m_pointer[i];
		}

		constexpr reference at(size_type i) {
			if (i >= m_size) throw std::out_of_range("evo::Array subscript out of range.");
			return m_pointer[i];
		}
		constexpr const_reference at(size_type i) const {
			if (i >= m_size) throw std::out_of_range("evo::Array subscript out of range.");
			return m_pointer[i];
		}

#pragma endregion

		friend constexpr bool operator==(const Array& a, const Array& b) noexcept;
		friend constexpr bool operator!=(const Array& a, const Array& b) noexcept;

		constexpr void swap(Array& other) noexcept {
			using std::swap;
			swap(m_pointer, other.m_pointer);
			swap(m_size, other.m_size);
			swap(m_alloc, other.m_alloc);
		}

		friend constexpr void swap(Array& l, Array& r) noexcept;

	private:
		pointer m_pointer;
		size_type m_size;
		[[no_unique_address]] Allocator m_alloc;

		void free_() {
			if constexpr (!std::is_trivially_destructible_v<T>) {
				for (size_type i = m_size; i --> 0; ) 
					std::allocator_traits<allocator_type>::destroy(m_alloc, m_pointer + i); 
			}

			//operator delete[](m_pointer);
			m_alloc.deallocate(m_pointer, m_size);
		}

		template<typename U, typename UAlloc>
		void alloc_copy_(const evo::Array<U, UAlloc>& other) {
			m_pointer = m_alloc.allocate(m_size);
			for (size_t i = 0; i < m_size; ++i) {
				std::allocator_traits<allocator_type>::construct(m_alloc, m_pointer + i, other[i]);
			}
		}
	};

	template<typename T>
	constexpr bool operator==(const Array<T>& a, const Array<T>& b) noexcept {
		return a.m_pointer == b.m_pointer;
	}

	template<typename T>
	constexpr bool operator!=(const Array<T>& a, const Array<T>& b) noexcept {
		return a.m_pointer != b.m_pointer;
	}

	template<typename T>
	constexpr void swap(Array<T>& l, Array<T>& r) noexcept {
		l.swap(r);
	}
}