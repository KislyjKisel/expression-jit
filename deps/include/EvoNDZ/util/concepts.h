#pragma once
#include <concepts>
#include <cstdint>
#include "type_traits.h"

namespace evo
{
	template<typename T, template<typename> class F, template<typename> class G>
	concept SatisfiesMorphed = F<typename G<T>::type>::value;

	template<typename T, typename... Ts>
	concept AllSame = ( std::same_as<T, Ts> && ... );

	template<typename T, typename... Ts>
	concept AnyOf = ( std::same_as<T, Ts> || ... );

	template<typename T, typename U>
	concept NotSameAs = !std::same_as<T, U>;

	template<typename T, typename U>
	concept DecaySameAs = std::same_as<std::decay_t<T>, U>;

	template<typename T, typename U>
	concept DecayConvertibleTo = std::convertible_to<std::decay_t<T>, U>;

	template<typename T>
	concept Ratio = requires( ) {
		{ T::num } -> std::integral;
		{ T::den } -> std::integral;
	};

	template<typename T>
	concept BooleanTestable = std::convertible_to<T, bool> && requires( T && a ) {
		{ !std::forward<T>(a) } -> std::convertible_to<bool>;
	};

	template<typename T, template<typename...> typename Template>
	concept Specialization = is_specialization_v<T, Template>;

	template<typename T>
	concept Transparent = requires { typename T::is_transparent; };
}