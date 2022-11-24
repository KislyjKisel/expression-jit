#pragma once
#include <vector>
#include <bit>
#include <exprjit/expression_node.h>
#include <exprjit/ir.h>

namespace ed
{	
	class InterpreterStack {
	public:
		InterpreterStack(size_t capacity) : m_size(0), m_data(new uint64_t[capacity]) { }
		~InterpreterStack() { delete[] m_data; }

		template<typename T> void push(T value) noexcept { m_data[m_size++] = std::bit_cast<uint64_t>( value ); }
		double popf() noexcept { return std::bit_cast<double>(m_data[--m_size]); }
		int64_t popi() noexcept { return std::bit_cast<int64_t>(m_data[--m_size]); }
		void clear() { m_size = 0; }

	private:
		uint64_t* m_data;
		size_t m_size;
	};

	class ExpressionTreeInterpreter {
	public:
		ExpressionTreeInterpreter(const std::vector<exprjit::ExpressionNode>& exprtree, size_t root) 
			: m_root(root), m_tree(exprtree) { }


	protected:
		const std::vector<exprjit::ExpressionNode>& m_tree;
		size_t m_root;
	};

	class RecursiveInterpreter final : public ExpressionTreeInterpreter {
	public:
		RecursiveInterpreter(const std::vector<exprjit::ExpressionNode>& exprtree, size_t root)
			: ExpressionTreeInterpreter(exprtree, root) { }

		double operator()(double) const noexcept;

	private:
		double eval(size_t i, double arg) const noexcept;
	};

	class StackInterpreter final : public ExpressionTreeInterpreter {
	public:
		StackInterpreter(const std::vector<exprjit::ExpressionNode>& exprtree, size_t root)
			: ExpressionTreeInterpreter(exprtree, root), m_st(256) { }

		double operator()(double) noexcept;

	private:
		InterpreterStack m_st;

		void eval(size_t i, double arg) noexcept;
	};

	class IRInterpreter final {
	public:
		IRInterpreter(const std::vector<exprjit::ir::Instruction>& instr) : m_instr(instr), m_st(256) { }

		double operator()(double) noexcept;

	private:
		InterpreterStack m_st;
		const std::vector<exprjit::ir::Instruction>& m_instr;
	};
}