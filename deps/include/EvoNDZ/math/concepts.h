#pragma once
#include <complex>
#include "../util/concepts.h"

namespace evo::math
{
	template<typename T>
	concept Arithmetic = std::is_arithmetic_v<T>;

	template<typename T>
	concept Number = std::is_arithmetic_v<T> || is_specialization_v<T, std::complex>;

	template<typename T>
	concept Multipliable = requires ( cref_t<T> a, cref_t<T> b, T& c ) { { a* b } -> std::convertible_to<T>; { c *= a }; };

	namespace detail
	{
		//todo: remove when VS supports `if constexpr(requires ...)`

		template<typename T>
		concept HasCustomMultiplicationIdentity = requires { { T::multiplication_identity() } -> std::convertible_to<T>; };

		template<typename T>
		concept HasCustomSummationIdentity = requires { { T::summation_identity() } -> std::convertible_to<T>; };
	}

	template<typename T>
	concept HasMultiplicationIdentity = Number<T> || requires { { T::multiplication_identity() } -> std::convertible_to<T>; };
	
	template<typename T>
	concept Summable = requires ( cref_t<T> a, cref_t<T> b, T& c ) { { a + b } -> std::convertible_to<T>; { c += a }; };

	template<typename T>
	concept HasSummationIdentity = Number<T> || requires { { T::summation_identity() } -> std::convertible_to<T>; };

	template<typename T>
	concept NonBoolIntegral = std::integral<T> && !std::same_as<bool, T>;

	template<typename T, typename U>
	concept PartiallyOrderedWith = requires( cref_t<T> a, cref_t<U> b ) {
		{ a < b } -> BooleanTestable;
		{ a <= b } -> BooleanTestable;
		{ a > b } -> BooleanTestable;
		{ a >= b } -> BooleanTestable;
		{ b < a } -> BooleanTestable;
		{ b <= a } -> BooleanTestable;
		{ b > a } -> BooleanTestable;
		{ b >= a } -> BooleanTestable;
	};

	template<typename T>
	concept PartiallyOrdered = PartiallyOrderedWith<T, T>;
}