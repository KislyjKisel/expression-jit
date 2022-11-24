#pragma once
#include <string_view>
#include <string>
#include "../util/array.h"

namespace evo
{
	template<size_t SizeNull>
	class SString {
	public:
		inline constexpr static size_t Size = SizeNull - 1;
		std::array<char, SizeNull> data;

		constexpr SString(const char(&s)[SizeNull])
			: data(make_array_of<SizeNull>([](const char(&s)[SizeNull], size_t i) { return s[i]; }, s)) { }

		constexpr SString(const char fill = ' ') : data(make_array_of<SizeNull>(fill)) { data[Size] = '\0'; }

		constexpr auto begin() const { return data.cbegin(); }
		constexpr auto end() const { return data.cend() - 1; }
		constexpr char operator[](size_t i) const { return data.at(i); }
		constexpr char& operator[](size_t i) { return data.at(i); }
		constexpr size_t size() const { return Size; }
		constexpr bool empty() const { return Size == 0; }

		constexpr operator std::string_view() const {
			return { data.data(), Size };
		}

		constexpr bool contains(const char c) const {
			for (size_t i = 0; i < Size; ++i) if (c == data[i]) return true;
			return false;
		}

		template<size_t Pos, size_t Count> requires (Pos + Count <= SizeNull)
		constexpr SString<( Count + 1 )> substr() const {
			SString<( Count + 1 )> r;
			for (size_t i = Pos, j = 0; j < Count; ++i, ++j) {
				r[j] = data[i];
			}
			return r;
		}

		template<size_t ASize>
		constexpr friend std::strong_ordering operator<=>(const SString& a, const SString<ASize>& b) {
			for (size_t i = 0; i < std::min(a.size(), b.size()); ++i) {
				auto ord = a[i] <=> b[i];
				if (ord != std::strong_ordering::equal) return ord;
			}
			return a.size() <=> b.size();
		}

		template<size_t ASize>
		constexpr bool operator==(const SString<ASize>& b) const {
			return *this <=> b == std::strong_ordering::equal;
		}

		template<size_t ASize>
		constexpr friend SString<( ASize + Size )> operator+(const SString& a, const SString<ASize>& b) {
			SString<( ASize + Size )> r;
			size_t i = 0;
			for (; i < a.size(); ++i) {
				r[i] = a[i];
			}
			for (size_t j = 0; j < b.size(); ++j, ++i) {
				r[i] = b[j];
			}
			return r;
		}

	};
}