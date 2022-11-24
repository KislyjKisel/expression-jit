#pragma once
#include "../util/type_traits.h"

namespace evo
{
	namespace detail
	{
		template<template<typename... T> typename Pack, size_t N, typename Cur>
		struct TypePackDrop { };

		template<template<typename... T> typename Pack, size_t N, typename... Cur, typename T>
		struct TypePackDrop<Pack, N, Pack<T, Cur...>>
			: TypePackDrop<Pack, ( N - 1 ), Pack<Cur...>> { };

		template<template<typename... T> typename Pack, typename Cur>
		struct TypePackDrop<Pack, 0, Cur> { using type = Cur; };



		template<template<typename... T> typename Pack, size_t N, typename Cur, typename Rest>
		struct TypePackTake { };

		template<template<typename... T> typename Pack, size_t N, typename... Cur, typename T, typename... Rest>
		struct TypePackTake<Pack, N, Pack<Cur...>, Pack<T, Rest...>> 
			: TypePackTake<Pack, (N - 1), Pack<Cur..., T>, Pack<Rest...>> { };

		template<template<typename... T> typename Pack, typename Cur, typename Rest>
		struct TypePackTake<Pack, 0, Cur, Rest> { using type = Cur; };


		template<template<typename... T> typename Pack, typename T, typename Types>
		struct TypePackContains { };

		template<template<typename... T> typename Pack, typename T, typename Cur, typename... Types>
		struct TypePackContains<Pack, T, Pack<Cur, Types...>> 
			: std::bool_constant<std::is_same_v<T, Cur> || TypePackContains<Pack, T, Pack<Types...>>::value> {  };

		template<template<typename... T> typename Pack, typename T, typename Cur>
		struct TypePackContains<Pack, T, Pack<Cur>> : std::bool_constant<std::is_same_v<T, Cur>> { };
	}

	template<typename... Types>
	struct TypePack {
		static constexpr size_t Size = sizeof...( Types );

		template<size_t I>
		using At = indexed_t<I, Types...>;

		template<typename T>
		static constexpr size_t IndexOf = index_of_type<T, Types...>();

		template<template<typename... Ts> typename T>
		using Convert = T < Types... >;

		template<template<typename T> typename F>
		using Morph = TypePack<F<Types>...>;
		
		template<size_t N>
		using Drop = typename detail::TypePackDrop<TypePack, N, TypePack<Types...>>::type;

		template<size_t N>
		using Take = typename detail::TypePackTake<TypePack, N, TypePack<>, TypePack>::type;

		template<typename T>
		using PushBack = TypePack<Types..., T>;

		template<typename T>
		using PushFront = TypePack<T, Types...>;

		using Front = At<0>;
		using Back = At<( sizeof...( Types ) - 1 )>;

		using PopFront = Drop<1>;
		using PopBack = Take<( sizeof...( Types ) - 1 )>;

		template<typename T>
		inline constexpr static bool Contains = detail::TypePackContains<TypePack, T, TypePack<Types...>>::value;

		constexpr TypePack() {};
	};

	using IntegralTypes = TypePack<unsigned char, signed char, unsigned short, signed short, unsigned int, signed int, unsigned long, signed long, unsigned long long, signed long long>;

	using FloatingTypes = TypePack<float, double, long double>;
}