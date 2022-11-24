#pragma once
#include "operation.h"
#include "..\util\concepts.h"
#include "function.h"

namespace evo::cat
{
	template<typename T, typename Op>
	concept IsMagma = op::InternalBinary<Op> && std::same_as<Op::left_type, T> && op::HasProperty<Op, op::total>;

	template<typename T, typename Op> requires IsMagma<T, Op>
	struct Magma {
		using domain = T;
		using operation = Op;

		//todo: operations on groups?
	};

	template<typename T, typename Op>
	concept IsSemigroup = IsMagma<T, Op> && op::HasProperty<Op, op::associative>;

	template<typename T, typename Op> requires IsMagma<T, Op>
	struct Semigroup : Magma<T, Op> { };

	template<typename T, typename Op>
	concept IsMonoid = IsSemigroup<T, Op> && op::HasIdentity<Op, Op::left_type>;

	template<typename T, typename Op> requires IsMagma<T, Op>
	struct Monoid : Semigroup<T, Op> { };

	template<typename T, typename Op>
	concept IsCommutativeMonoid = IsMonoid<T, Op> && op::HasProperty<Op, op::commutative>;

	template<typename InvOp>
	concept IsInverseOperation = op::InternalUnary<InvOp> && op::HasProperty<InvOp, op::total>;

	template<typename T, typename Op, typename InvOp>
	concept IsGroup = IsMonoid<T, Op> && IsInverseOperation<InvOp>;

	template<typename T, typename Op, typename InvOp> requires IsGroup<T, Op, InvOp>
	struct Group : Monoid<T, Op> { };
}