#pragma once
#include <type_traits>

namespace evo
{
	template<typename TEnum>
	struct enum_box {
		using type = std::underlying_type_t<TEnum>;
		constexpr enum_box(TEnum e) : m_value(type(e)) { }

		constexpr enum_box operator|(const enum_box b) const {
			return enum_box(m_value | b.m_value);
		}

		constexpr bool operator&(const enum_box b) const {
			return m_value & b.m_value;
		}

	private:
		type m_value;

		constexpr explicit enum_box(type v) : m_value(v) { }
	};
}