#include "interpreter.h"
#include <bit>
#include <cmath>

namespace ed
{
	double RecursiveInterpreter::operator()(double arg) const noexcept {
		return eval(m_root, arg);
	}

	double RecursiveInterpreter::eval(size_t i, double arg) const noexcept {
		const auto& node = m_tree[i];

		switch (node.type) {
			case exprjit::ExpressionNode::Type::Argument:
				return arg;
			case exprjit::ExpressionNode::Type::Literal:
				return std::bit_cast<double>(node.literal.value);
			case exprjit::ExpressionNode::Type::Unop:
			{
				double oparg = eval(node.unop.operand, arg);
				switch (node.unop.op) {
					case exprjit::ExpressionNode::Unop::Negate:
						return -oparg;
					case exprjit::ExpressionNode::Unop::Abs:
						return std::abs(oparg);
					case exprjit::ExpressionNode::Unop::Sin:
						return std::sin(oparg);
					case exprjit::ExpressionNode::Unop::Cos:
						return std::cos(oparg);
					case exprjit::ExpressionNode::Unop::Floor:
						return std::floor(oparg);
				}
				break;
			}
			case exprjit::ExpressionNode::Type::Binop:
			{
				double lhs = eval(node.binop.lhs, arg);
				double rhs = eval(node.binop.rhs, arg);
				switch (node.binop.op) {
					case exprjit::ExpressionNode::Binop::Add:
						return lhs + rhs;
					case exprjit::ExpressionNode::Binop::Subtract:
						return lhs - rhs;
					case exprjit::ExpressionNode::Binop::Multiply:
						return lhs * rhs;
					case exprjit::ExpressionNode::Binop::Divide:
						return lhs / rhs;
					case exprjit::ExpressionNode::Binop::Modulo:
						return std::fmod(lhs, rhs);
				}
				break;
			}
		}
		return 0.;
	}

	double StackInterpreter::operator()(double arg) noexcept {
		m_st.clear();
		eval(m_root, arg);
		return m_st.popf();
	}

	void StackInterpreter::eval(size_t i, double arg) noexcept {
		const auto& node = m_tree[i];
		
		switch (node.type) {
			case exprjit::ExpressionNode::Type::Argument:
				m_st.push(arg);
				break;
			case exprjit::ExpressionNode::Type::Literal:
				m_st.push(node.literal.value);
				break;
			case exprjit::ExpressionNode::Type::Unop:
			{
				eval(node.unop.operand, arg);
				double oparg = m_st.popf();
				switch (node.unop.op) {
					case exprjit::ExpressionNode::Unop::Negate:
						m_st.push(-oparg);
						break;
					case exprjit::ExpressionNode::Unop::Abs:
						m_st.push(std::abs(oparg));
						break;
					case exprjit::ExpressionNode::Unop::Sin:
						m_st.push(std::sin(oparg));
						break;
					case exprjit::ExpressionNode::Unop::Cos:
						m_st.push(std::cos(oparg));
						break;
					case exprjit::ExpressionNode::Unop::Floor:
						m_st.push(std::floor(oparg));
						break;
				}
				break;
			}
			case exprjit::ExpressionNode::Type::Binop:
			{
				eval(node.binop.lhs, arg);
				eval(node.binop.rhs, arg);
				double rhs = m_st.popf();
				double lhs = m_st.popf();
				switch (node.binop.op) {
					case exprjit::ExpressionNode::Binop::Add:
						m_st.push(lhs + rhs);
						break;
					case exprjit::ExpressionNode::Binop::Subtract:
						m_st.push(lhs - rhs);
						break;
					case exprjit::ExpressionNode::Binop::Multiply:
						m_st.push(lhs * rhs);
						break;
					case exprjit::ExpressionNode::Binop::Divide:
						m_st.push(lhs / rhs);
						break;
					case exprjit::ExpressionNode::Binop::Modulo:
						m_st.push(std::fmod(lhs, rhs));
						break;
				}
				break;
			}
		}
	}

	double IRInterpreter::operator()(double arg) noexcept {
		m_st.clear();
		using exprjit::ir::Code;

		for (const auto i : m_instr) {
			switch (i.code) {
				case Code::ILoad:
				case Code::FLoad:
					m_st.push(i.operands->value);
					break;
				case Code::FArg:
					m_st.push(arg);
					break;
				case Code::IToF:
					m_st.push((double)m_st.popi());
					break;
				case Code::FToI:
					m_st.push((int64_t)m_st.popf());
					break;
				case Code::FAdd:
					m_st.push(m_st.popf() + m_st.popf());
					break;
				case Code::FSub:
					m_st.push(m_st.popf() - m_st.popf());
					break;
				case Code::FMul:
					m_st.push(m_st.popf() * m_st.popf());
					break;
				case Code::FDiv:
					m_st.push(m_st.popf() / m_st.popf());
					break;
				case Code::FAbs:
					m_st.push(abs(m_st.popf()));
					break;
				case Code::FSin:
					m_st.push(sin(m_st.popf()));
					break;
				case Code::FFloor:
					m_st.push(floor(m_st.popf()));
					break;
				case Code::FCos:
					m_st.push(cos(m_st.popf()));
					break;
				case Code::IAdd:
					m_st.push(m_st.popi() + m_st.popi());
					break;
				case Code::ISub:
					m_st.push(m_st.popi() - m_st.popi());
					break;
				case Code::IMul:
					m_st.push(m_st.popi() * m_st.popi());
					break;
				case Code::IDiv:
					m_st.push(m_st.popi() / m_st.popi());
					break;
				case Code::IAbs:
					m_st.push(abs(m_st.popi()));
					break;
				case Code::Ret:
					return m_st.popf();
			}
		}

		terminate();
	}
}