#pragma once
#include <concepts>
#include <cstdint>
#include <vector>
#include <span>
#include <ranges>
#include <string_view>
#include <bit>
#include "../util/platform.h"

namespace evo::data
{
	class Serializer;

	template<typename T>
	concept Serializable = requires( Serializer & s, const T & a ) {
		{ s << a } -> std::convertible_to<Serializer&>;
	};

	class Serializer final {
	public:
		Serializer() : m_buffer() { }

		void reserve(size_t cap) { 
			m_buffer.reserve(cap); 
		}
		size_t size() const { 
			return m_buffer.size(); 
		}
		const unsigned char* data() const {
			return m_buffer.data();
		}
		void clear() {
			m_buffer.clear();
		}
		
		template<std::integral T>
		Serializer& operator<<(T i) {
			union {
				T iv;
				unsigned char bv[sizeof(T)];
			};
			iv = i;
			if constexpr (std::endian::native == GlobalByteOrder) {
				for (size_t bi = 0; bi < sizeof(T); ++bi) m_buffer.push_back(bv[bi]);
			}
			else {
				for (size_t bi = sizeof(T); bi-- > 0;) m_buffer.push_back(bv[bi]);
			}
			return *this;
		}

		Serializer& operator<<(const std::string_view str) {
			*this << str.size();
			m_buffer.reserve(m_buffer.size() + str.size());
			for (char c : str) m_buffer.push_back(c);
			return *this;
		}

		template<std::ranges::input_range Range>
		Serializer& operator<<(const Range& r) 
		{
			if constexpr (requires( ) { { r.size() } -> std::same_as<typename Range::size_type>; }) {
				*this << r.size();
			}
			else {
				*this << std::distance(r.begin(), r.end());
			}

			for (const auto& v : r) {
				*this << v;
			}
			return *this;
		}

		template<Serializable T, size_t Extent>
		Serializer& operator<<(std::span<const T, Extent> buf) {
			for (const T& v : buf) *this << v;
			return *this;
		}

	private:
		std::vector<unsigned char> m_buffer;
	};
}