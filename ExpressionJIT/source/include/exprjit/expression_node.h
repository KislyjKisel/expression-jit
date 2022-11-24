#pragma once
#include <cstdint>
#include "data_type.h"

namespace exprjit
{
	struct ExpressionNode {
		enum class Type {
			Binop,
			Unop,
			Literal,
			Argument
		};
		enum class Binop {
			Add, Subtract, Multiply, Divide, Modulo
		};
		enum class Unop {
			IToF, FToI, Negate, Abs, Sin, Cos, Floor
		};

		Type type;

		union {
			struct {
				size_t lhs;
				size_t rhs;
				Binop op;
			} binop;

			struct {
				size_t operand;
				Unop op;
			} unop;

			struct {
				uint64_t value;
				DataType type;
			} literal;

			struct {
				unsigned index;
				DataType type;
			} argument;
		};

		static ExpressionNode makeBinop(Binop op, size_t lhs, size_t rhs) {
			ExpressionNode node;
			node.type = Type::Binop;
			node.binop.op = op;
			node.binop.lhs = lhs;
			node.binop.rhs = rhs;
			return node;
		}
		static ExpressionNode makeUnop(Unop op, size_t operand) {
			ExpressionNode node;
			node.type = Type::Unop;
			node.unop.op = op;
			node.unop.operand = operand;
			return node;
		}
		static ExpressionNode makeLiteral(uint64_t value, DataType type) {
			ExpressionNode node;
			node.type = Type::Literal;
			node.literal.value = value;
			node.literal.type = type;
			return node;
		}
		static ExpressionNode makeArgument(unsigned argindex, DataType type) {
			ExpressionNode node;
			node.type = Type::Argument;
			node.argument.index = argindex;
			node.argument.type = type;
			return node;
		}

	private:
		ExpressionNode() = default;
	};
}