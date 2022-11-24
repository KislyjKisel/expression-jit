#pragma once
#include <concepts>
#include <array>
#include "type_traits.h"

namespace evo
{
	namespace detail
	{
		template<size_t S, typename T, size_t... Is>
		constexpr std::array<T, S> make_array_of_impl(const T& value, std::index_sequence<Is...>) {
			return { { ( static_cast<void>( Is ), value )... } };
		};

		template<size_t S, typename F, typename T, size_t... Is>
		constexpr auto make_array_of_impl(F&& f, T&& value, std::index_sequence<Is...>) 
			-> std::array<std::invoke_result_t<F, T, size_t>, S> 
		{
			return { { f(value, Is)... } };
		};

		template<typename T, typename U, typename F, typename... Us, size_t... Is>
		constexpr std::array<T, sizeof...( Us )> make_array_impl(F&& f, std::index_sequence<Is...>, Us... list) {
			return std::array<T, sizeof...( Us )>{ { f(list, Is)... } };
		}

		template<typename T, size_t Size, typename F, size_t... Is>
		constexpr std::array<std::invoke_result_t<F, T>, Size> transform_array_impl(
			const std::array<T, Size>& a, F&& f, std::index_sequence<Is...>
		) {
			return { { ( static_cast<void>( Is ), f(a[Is]) )... } };
		}



		template<typename Array> constexpr size_t array_size_impl = 0;
		template<typename T, size_t N> constexpr size_t array_size_impl<std::array<T, N>> = N;
	}

	// returns array of copies of some value
	template<size_t S, typename T>
	constexpr std::array<T, S> make_array_of(const T& value) {
		return detail::make_array_of_impl<S, T>(value, std::make_index_sequence<S>{});
	}

	template<size_t S, typename F, typename T>
	constexpr auto make_array_of(F&& f, T&& value) {
		return detail::make_array_of_impl<S>(std::forward<F>(f), std::forward<T>(value), std::make_index_sequence<S>{});
	}

	// returns array of results of F invokation with every element of list as first argument, and index as second
	template<typename T, typename U, typename F, typename... Us>
	//requires (std::convertible_to<Us, U> && ...) && std::is_invocable_r_v<T, F, U, size_t>
	constexpr std::array<T, sizeof...( Us )> make_array(F&& f, Us... list) {
		return detail::make_array_impl<T, U>(std::forward<F>(f), std::make_index_sequence<sizeof...( Us )>{}, list...);
	}

	template<typename T, size_t Size, typename F>
	constexpr std::array<std::invoke_result_t<F, T>, Size> transform_array(const std::array<T, Size>& a, F&& f) {
		return detail::transform_array_impl(a, f, std::make_index_sequence<Size>{});
	}

	template<typename Array>
	constexpr size_t array_size = detail::array_size_impl<std::decay_t<Array>>;
}