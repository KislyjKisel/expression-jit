#include "include/exprjit/ir_generator.h"
#include <unordered_map>

namespace exprjit::ir
{
	std::unordered_map<ExpressionNode::Binop, std::pair<Code, Code>> binopMap {
		{ ExpressionNode::Binop::Add,		{ Code::IAdd, Code::FAdd } },
		{ ExpressionNode::Binop::Subtract,	{ Code::ISub, Code::FSub } },
		{ ExpressionNode::Binop::Multiply,	{ Code::IMul, Code::FMul } },
		{ ExpressionNode::Binop::Divide,		{ Code::IDiv, Code::FDiv } },
		{ ExpressionNode::Binop::Modulo,		{ Code::IMod, Code::FMod } }
	};
	std::unordered_map<ExpressionNode::Unop, std::pair<Code, Code>> unopMap {
		{ ExpressionNode::Unop::Negate, { Code::INeg, Code::FNeg		} },
		{ ExpressionNode::Unop::Abs,		{ Code::IAbs, Code::FAbs		} },
		{ ExpressionNode::Unop::Sin,		{ Code::None, Code::FSin		} },
		{ ExpressionNode::Unop::Cos,		{ Code::None, Code::FCos		} },
		{ ExpressionNode::Unop::Floor,	{ Code::None, Code::FFloor	} },
	};
	VirtualRegister vri[2] { VirtualRegister::I0, VirtualRegister::I1 };
	VirtualRegister vrf[2] { VirtualRegister::F0, VirtualRegister::F1 };

	bool isInt(VirtualRegister vr) {
		return vr == VirtualRegister::I0 || vr == VirtualRegister::I1;
	}

	void Generator::itof(VirtualRegister d, VirtualRegister i) noexcept {
		Instruction instr(Code::IToF);
		instr.operands[0] = d;
		instr.operands[1] = i;
		m_ir.push_back(instr);
	}
	void Generator::ftoi(VirtualRegister i, VirtualRegister d) noexcept {
		Instruction instr(Code::FToI);
		instr.operands[0] = i;
		instr.operands[1] = d;
		m_ir.push_back(instr);
	}
	void Generator::pop(VirtualRegister vr) noexcept {
		Instruction popa(isInt(vr) ? Code::IPop : Code::FPop);
		popa.operands[0] = vr;
		m_ir.push_back(popa);
	}
	void Generator::push(VirtualRegister vr) noexcept {
		Instruction pusha(isInt(vr) ? Code::IPush : Code::FPush);
		pusha.operands[0] = vr;
		m_ir.push_back(pusha);
	}

	VirtualRegister Generator::popa(DataType type, DataType resT, int reg) noexcept {
		VirtualRegister V = type == DataType::Integer ? vri[reg] : vrf[reg];
		pop(V);
		if (type != resT) {
			VirtualRegister Vn;
			if(resT == DataType::Float) { 
				Vn = vrf[reg];
				itof(Vn, V);
			}
			else {
				Vn = vri[reg];
				ftoi(Vn, V);
			}
			V = Vn;
		}
		return V;
	}

	DataType Generator::gen(size_t i) noexcept {
		auto& node = m_expression[i];

		switch (node.type) {
			case ExpressionNode::Type::Binop:
			{
				auto code = binopMap.at(node.binop.op);
				DataType rhsT = gen(node.binop.rhs);
				DataType lhsT = gen(node.binop.lhs);
				DataType resT = ( lhsT == DataType::Float || rhsT == DataType::Float ) ? DataType::Float : DataType::Integer;
				VirtualRegister lhsV = popa(lhsT, resT, 0);
				VirtualRegister rhsV = popa(rhsT, resT, 1);
				Instruction instr(resT == DataType::Integer ? code.first : code.second);
				instr.operands[0] = lhsV;
				instr.operands[1] = rhsV;
				m_ir.push_back(instr);
				push(lhsV);
				return resT;
			}
			case ExpressionNode::Type::Unop:
			{
				DataType opT = gen(node.unop.operand);
				if (node.unop.op == ExpressionNode::Unop::FToI) {
					if (opT != DataType::Integer) {
						pop(VirtualRegister::F0);
						ftoi(VirtualRegister::I0, VirtualRegister::F0);
						push(VirtualRegister::I0);
					}
					return DataType::Integer;
				}
				else if (node.unop.op == ExpressionNode::Unop::IToF) {
					if (opT != DataType::Float) {
						pop(VirtualRegister::I0);
						itof(VirtualRegister::F0, VirtualRegister::I0);
						push(VirtualRegister::F0);
					}
					return DataType::Float;
				}
				const auto& code = unopMap.at(node.unop.op);

				DataType iT;
				Code iC;
				if (opT == DataType::Integer) {
					if (code.first == Code::None) {
						iT = DataType::Float;
						iC = code.second;
					}
					else {
						iT = DataType::Integer;
						iC = code.first;
					}
				}
				else {
					if (code.second == Code::None) {
						iT = DataType::Integer;
						iC = code.first;
					}
					else {
						iT = DataType::Float;
						iC = code.second;
					}
				}

				Instruction instr(iC);
				VirtualRegister opV = popa(opT, iT, 0);
				instr.operands[0] = opV;
				m_ir.push_back(instr);
				push(opV);
				return iT;
			}
			case ExpressionNode::Type::Argument:
			{
				Instruction instr(node.argument.type == DataType::Integer ? Code::IArg : Code::FArg);
				instr.operands[0] = node.argument.index;
				m_ir.push_back(instr);
				return node.argument.type;
			}
			case ExpressionNode::Type::Literal:
			{
				Instruction instr(node.literal.type == DataType::Integer ? Code::ILoad : Code::FLoad);
				instr.operands[0] = node.literal.value;
				m_ir.push_back(instr);
				return node.literal.type;
			}
			default:
				throw std::exception("Unknown expression node.");
		}
	}

	void Generator::operator()() {
		DataType returnType = gen(m_exprRoot);
		if (returnType == m_resultType) {
			pop(returnType == DataType::Integer ? VirtualRegister::IR : VirtualRegister::FR);
		}
		else if (returnType == DataType::Float && m_resultType == DataType::Integer) {
			pop(VirtualRegister::F0);
			ftoi(VirtualRegister::IR, VirtualRegister::F0);
		}
		else if (returnType == DataType::Integer && m_resultType == DataType::Float) {
			pop(VirtualRegister::I0);
			itof(VirtualRegister::FR, VirtualRegister::I0);
		}
		m_ir.push_back(Code::Ret);
	}
}