#pragma once
#include <vector>
#include <memory>
#include <EvoNDZ/app/scene.h>
#include <EvoNDZ/util/timer.h>
#include <exprjit/expression_node.h>
#include <exprjit/ir.h>
#include "../expression_compiler.h"
#include "../interpreter.h"

namespace ed
{
	class BenchmarkScene final : public evo::Scene {
	public:
		void initialize() override;
		void gui() override;

		void update() override { }
		void terminate() override { }
		void render() override { }

	private:
		std::vector<exprjit::ExpressionNode> m_expression;
		std::vector<exprjit::ir::Instruction> m_instructions;
		std::unique_ptr<exprjit::Function<double(double)>> m_function;
		std::unique_ptr<RecursiveInterpreter> m_interpreter_rec;
		std::unique_ptr<StackInterpreter> m_interpreter_stk;
		std::unique_ptr<IRInterpreter> m_interpreter_ir;
		bool m_hasResult = false;
		double m_timeAOT, m_timeJIT, m_timeIRC, m_timeIST, m_timeIIR;
		double m_timeParse, m_timeComp;
	};
}