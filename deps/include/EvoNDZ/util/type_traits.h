#pragma once
#include <type_traits>

namespace evo
{
	/*template<template<typename U> class F, template<typename, typename...> class G, typename... Args>
	struct TraitCompose {
		template<typename T>
		inline constexpr static auto value = F<G<T, Args...>::type>::value;
	};*/

	template<typename T, typename... Ts>
	constexpr size_t index_of_type() {
		size_t i = 0;
		bool found = false;
		( ( found |= std::is_same_v<T, Ts>, i += !found ), ... );
		return i;
	}

	template<typename T>
	using cref_t = const std::remove_cvref_t<T>&;

	template<size_t I, typename T, typename... Ts>
	struct indexed : indexed<( I - 1 ), Ts...> { static_assert( sizeof...( Ts ) >= I ); };

	template<typename T, typename... Ts>
	struct indexed<0, T, Ts...> { using type = T; };

	template<size_t I, typename... Ts>
	using indexed_t = typename indexed<I, Ts...>::type;



	template<typename T, template<typename...> typename TTemplate >
	struct is_specialization : std::false_type { };

	template<template<typename...> typename Template, typename... TArgs>
	struct is_specialization<Template<TArgs...>, Template> : std::true_type { };

	template<typename T, template<typename...> typename TTemplate>
	constexpr bool is_specialization_v = is_specialization<T, TTemplate>::value;
}