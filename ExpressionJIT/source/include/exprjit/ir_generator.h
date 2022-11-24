#pragma once
#include <vector>
#include "data_type.h"
#include "expression_node.h"
#include "ir.h"

namespace exprjit::ir
{
	class Generator {
	public:
		Generator(const std::vector<ExpressionNode>& expr, size_t root, std::vector<ir::Instruction>& ir, DataType resultType) 
			: m_expression(expr), m_exprRoot(root), m_ir(ir), m_resultType(resultType) { }

		void operator()();

	private:
		const std::vector<ExpressionNode>& m_expression;
		std::vector<ir::Instruction>& m_ir;
		size_t m_exprRoot;
		DataType m_resultType;

		DataType gen(size_t) noexcept;
		void pop(VirtualRegister) noexcept;
		void push(VirtualRegister) noexcept;
		void itof(VirtualRegister d, VirtualRegister i) noexcept;
		void ftoi(VirtualRegister i, VirtualRegister d) noexcept;

		VirtualRegister popa(DataType type, DataType resT, int reg) noexcept;
	};
}