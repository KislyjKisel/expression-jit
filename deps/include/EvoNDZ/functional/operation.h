#pragma once
#include <optional>
#include "../meta/assumption.h"
#include "../meta/type_pack.h"
#include "../util/concepts.h"

namespace evo::op
{
	template<typename Op, typename V>
	concept HasIdentity = requires {
		{ Op::identity } -> std::convertible_to<V>;
	};

	template<typename Op, typename V>
	concept HasAbsorbing = requires {
		{ Op::absorbing } -> std::convertible_to<V>;
	};

#pragma region Properties
	template<typename Prop>
	struct property { };

	struct injective			{ }; // ∀x₁∀x₂		[ (x₁ ≠ x₂) ⇒ (f(x₁) ≠ f(x₂))		] 
	struct surjective		{ }; // ∀y∃x		[ f(x) = y							]
	struct total				{ }; // ∀x∃y		[ f(x) = y							] ?
	struct associative		{ }; // ∀x₁∀x₂∀x₃	[ (x₁ @ x₂) @ x₃ ≡ x₁ @ (x₂ @ x₃)	]
	struct commutative		{ }; // ∀x₁∀x₂		[ x₁ @ x₂ ≡ x₂ @ x₁					]
	struct anticommutative	{ }; // ∀x₁∀x₂		[ x₁ @ x₂ ≡ -(x₂ @ x₁)				]
	struct idempotent		{ }; // ∀x			[ x₁ @ x₁ ≡ x₁						]
	
	template<typename Op, typename Prop>
	concept HasProperty = std::derived_from<Op, property<Prop>>;

	template<typename T, T Identity>
	struct has_identity { inline constexpr static T identity = Identity; };

	template<typename T, T Absorbing>
	struct has_absorbing { inline constexpr static T absorbing = Absorbing; };

	template<typename... Ops>
	struct distributive_over { 
		template<typename Op> inline constexpr static distributive = TypePack<Ops...>::template Contains<Op>; 
	};

#pragma endregion

#pragma region Unary

	template<typename Op>
	concept Unary = requires{
		typename Op::value_type;
		typename Op::result_type;

		NotSameAs<Op::value_type, void>;
		NotSameAs<Op::result_type, void>;

		{ Op::apply(std::declval<typename Op::value_type>()) } -> std::same_as<typename Op::result_type>;
	};

	template<typename Op>
	concept InternalUnary = Unary<Op> && std::same_as<Op::value_type, Op::result_type>;

	template<typename T>
	struct internal_unary {
		using value_type = T;
		using result_type = T;
	};

#pragma endregion

#pragma region Binary

	template<typename Op>
	concept Binary = requires {
		typename Op::left_type;
		typename Op::right_type;
		typename Op::result_type;

		NotSameAs<Op::left_type, void>;
		NotSameAs<Op::right_type, void>;
		NotSameAs<Op::result_type, void>;

		{ Op::apply(std::declval<typename Op::left_type>(), std::declval<typename Op::right_type>()) }
			-> std::same_as<Op::result_type>;
	};

	template<typename Op>
	concept InternalBinary = Binary<Op> && AllSame<typename Op::left_type, typename Op::right_type, typename Op::result_type>;

	template<typename T>
	struct internal_binary {
		using left_type = T;
		using right_type = T;
		using result_type = T;
	};

	template<typename Op>
	concept LeftExternalBinary = Binary<Op> && std::same_as<typename Op::right_type, typename Op::result_type>;

	template<typename T, typename Ext>
	struct left_external_binary {
		using left_type = Ext;
		using right_type = T;
		using result_type = T;
	};

	template<typename Op>
	concept RightExternalBinary = Binary<Op> && std::same_as<typename Op::left_type, typename Op::result_type>;

	template<typename T, typename Ext>
	struct right_external_binary {
		using left_type = T;
		using right_type = Ext;
		using result_type = T;
	};

#pragma endregion

	template<InternalUnary Op, unsigned N>
	constexpr typename Op::result_type apply_iterative(cref_t<typename Op::value_type> x) {
		if constexpr (N == 0) {
			return x;
		}
		else {
			return apply_iterative<Op, ( N - 1 )>(Op::apply(x));
		}
	}
}