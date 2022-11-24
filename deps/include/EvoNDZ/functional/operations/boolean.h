#pragma once
#include "../operation.h"
#include "../set.h"
#include "../category.h"

namespace evo::op
{
	struct Not : internal_unary<𝔹>, property<injective>, property<surjective>, property<total> {
		constexpr static 𝔹 apply(const 𝔹 x) noexcept { return !x; }

		constexpr Not() noexcept = default;
	};

	struct Xor;
	struct Or;

	struct And : internal_binary<𝔹>, property<associative>, property<commutative>, property<total>, 
				 has_identity<𝔹, true>, distributive_over<Or, Xor> 
	{
		constexpr static 𝔹 apply(const 𝔹 a, const 𝔹 b) noexcept { return a && b; }

		constexpr And() noexcept = default;
	};

	struct Or : internal_binary<𝔹>, property<associative>, property<commutative>, property<total>,
				has_identity<𝔹, false>, distributive_over<And> 	
	{
		constexpr static 𝔹 apply(const 𝔹 a, const 𝔹 b) noexcept { return a || b; }

		constexpr Or() noexcept = default;
	};

	struct Xor : internal_binary<𝔹>, property<associative>, property<commutative>, property<total>,
				 has_identity<𝔹, false>, distributive_over<> 	
	{
		constexpr static 𝔹 apply(const 𝔹 a, const 𝔹 b) noexcept { return a ^ b; }

		constexpr Xor() noexcept = default;
	};
}