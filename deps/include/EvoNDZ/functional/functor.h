#pragma once
#include <optional>
#include "../meta/assumption.h"
#include "../util/util.h"

namespace evo
{
	template<template<typename> typename T>
	concept Functor = requires( cref_t<T<﹖a>> t, some_function<﹖b, ﹖a> f ) {
		{ fmap(f, t) } -> std::convertible_to<T<﹖b>>;
	};

	template<typename U, typename F> requires std::is_invocable_v<F, U>
	constexpr std::optional<std::invoke_result_t<F, U>> fmap(F&& f, const std::optional<U>& a) {
		if (a.has_value()) {
			return std::make_optional(f(a.value()));
		}
		else {
			return std::nullopt;
		}
	}

	// todo: applicative, monad
}