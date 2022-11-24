#pragma once
#include <type_traits>
#include "../util/type_traits.h"

namespace evo
{
	template<typename T, typename TClass, T TClass::* Member>
	class DefaultGetter {
		T operator()(TClass* p) const {
			return p->*Member;
		}
	};

	template<typename T, typename TClass, T TClass::* Member>
	class DefaultSetter {
		void operator()(TClass* p, T v) const {
			return p->*Member = v;
		}
	};

	template<typename T, typename TClass, typename Setter, typename Getter>
	class Property {
	public:
		constexpr Property(TClass* pointer, const Setter& setter = {}, const Getter& getter = {})
			noexcept( std::is_nothrow_copy_constructible_v<Getter>&& std::is_nothrow_copy_constructible_v<Getter> ) :
			m_pointer(pointer), m_getter(getter), m_setter(setter) { }

		constexpr Property(const Property&) = default;
		constexpr Property& operator=(const Property&) = default;
		constexpr Property(Property&&) = default;
		constexpr Property& operator=(Property&&) = default;

		constexpr Property& operator=(const T& value) {
			m_setter(m_pointer, value);
			return *this;
		}

		constexpr operator auto( ) const {
			return m_getter(m_pointer);
		}

	private:
		TClass* m_pointer;
		[[no_unique_address]] Getter m_getter;
		[[no_unique_address]] Setter m_setter;
	};

	template<typename T, typename TClass, T TClass::* Member, typename Setter = DefaultSetter<T, TClass, Member>, typename Getter = DefaultGetter<T, TClass, Member>>
	using MProperty = Property<T, TClass, Setter, Getter>;
}