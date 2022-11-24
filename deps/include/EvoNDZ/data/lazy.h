#pragma once
#include <type_traits>
#include "../util/util.h"

namespace evo
{
	template<typename T, typename TConstructor = evo::DefaultConstructor<T>, typename TDestructor = evo::DefaultDestructor<T>>
	class Lazy {
	public:
		constexpr Lazy(const TConstructor& c = {}, const TDestructor& d = {}) 
			noexcept(std::is_nothrow_copy_constructible_v<TConstructor> && std::is_nothrow_copy_constructible_v<TDestructor> )
			: m_constructor(c), m_destructor(d) { }

		Lazy(const Lazy&) = default;
		Lazy& operator=(const Lazy&) = default;

		~Lazy() noexcept(std::is_nothrow_invocable_v<decltype(&Lazy::destroy), Lazy>) {
			if (m_flag) destroy();
		}

		void destroy() noexcept(std::is_nothrow_invocable_v<TDestructor, T*>) {
			m_destructor((T*)m_value);
			m_flag = false;
		}

		template<typename... TArgs>
		void construct(TArgs&&... args) noexcept( std::is_nothrow_invocable_v<TConstructor, T*, TArgs...> ) {
			m_constructor((T*)m_value, std::forward<TArgs>(args)...);
			m_flag = true;
		}

		template<typename... TArgs>
		T& value(TArgs&&... args) noexcept(std::is_nothrow_invocable_v<decltype(&Lazy::construct<TArgs...>), Lazy, TArgs...>) {
			if (!m_flag) construct(std::forward<TArgs>(args)...);
			return *(T*)m_value;
		}

		constexpr bool available() const {
			return m_flag;
		}

		operator T&() noexcept(std::is_nothrow_invocable_v<decltype( &Lazy::value), Lazy>) {
			return value();
		}

	private:
		char m_value[sizeof(T)];
		[[no_unique_address]] TConstructor m_constructor;
		[[no_unique_address]] TDestructor m_destructor;
		bool m_flag = false;
	};
}