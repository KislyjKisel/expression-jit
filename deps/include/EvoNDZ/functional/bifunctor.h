#pragma once
#include <utility>
#include "../meta/assumption.h"
#include "../util/util.h"

namespace evo
{
	template<template<typename U, typename V> typename T>
	concept Bifunctor = requires( cref_t<T<﹖a, ﹖b>> t, some_function<﹖c, ﹖a> f, some_function<﹖d, ﹖b> g ) {
		{ bimap(f, g, t) } -> std::convertible_to<T<﹖c, ﹖d>>;
		{ map_first(f, t) } -> std::convertible_to<T<﹖c, ﹖b>>;
		{ map_second(g, t) } -> std::convertible_to<T<﹖a, ﹖d>>;
	};

#pragma region Bifunctor : std::pair
	template<typename U, typename V, typename F, typename G> requires std::is_invocable_v<F, U> && std::is_invocable_v<G, V>
	constexpr std::pair<std::invoke_result_t<F, U>, std::invoke_result_t<G, V>> bimap(F&& f, G&& g, const std::pair<U, V>& a) {
		return { f(a.first), g(a.second) };
	}

	template<typename U, typename V, typename F> requires std::is_invocable_v<F, U>
	constexpr std::pair<std::invoke_result_t<F, U>, V> map_first(F&& f, const std::pair<U, V>& a) {
		return { f(a.first), a.second };
	}

	template<typename U, typename V, typename G> requires std::is_invocable_v<G, V>
	constexpr std::pair<U, std::invoke_result_t<G, V>> map_second(G&& g, const std::pair<U, V>& a) {
		return { a.first, g(a.second) };
	}
#pragma endregion
}