#pragma once
#include <concepts>
#include "../operation.h"
#include "../../util/integer.h"

namespace evo::op
{
	template<std::unsigned_integral ℕ, typename ℤ = int_wider_than<std::make_signed_t<ℕ>>>
	struct Negate : property<injective>, property<surjective>, property<total> {
		using value_type = ℕ;
		using result_type = ℤ;

		constexpr static ℤ apply(const ℕ x) noexcept { return -ℤ(x); }

		constexpr Negate() noexcept = default;
	};
}