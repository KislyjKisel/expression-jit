#include "benchmark_scene.h"
#include <imgui/imgui.h>
#include <exprjit/x86_64.h>
#include <exprjit/binary_encoder.h>
#include <exprjit/function.h>
#include <exprjit/parser.h>
#include <exprjit/jit.h>
#include <exprjit/data_type.h>
#include <exprjit/ir.h>
#include <exprjit/ir_generator.h>
#include <exprjit/ir_optimizer.h>

namespace ed
{
	double(* volatile bench_fun_aot )( double ) = [](double x) { return 18 - x * ( 3.14 - abs(x) + floor(x * abs(x - 5)) ); };
	const char* bench_fun_src = "18 - x * (3.14 - abs x + floor (x * abs(x - 5)))";

	void BenchmarkScene::initialize() {
		{
			std::vector<unsigned char> binary;
			std::unordered_map<char, std::pair<unsigned, exprjit::DataType>> argmap {
				{ 'x', { 0, exprjit::DataType::Float } }
			};

			evo::Timer timer;
			size_t ei = exprjit::Parser(bench_fun_src, m_expression, argmap)( );
			m_timeParse = timer.time<double>();
			timer.reset();
			exprjit::ir::Generator(m_expression, ei, m_instructions, exprjit::DataType::Float)();
			exprjit::ir::Optimizer opt(m_instructions);
			opt();
			exprjit::ir::jit(m_instructions, make_unique<exprjit::X86_64>(binary, 0, 1));
			m_function = std::make_unique<exprjit::Function<double(double)>>(binary);
			m_timeJIT = timer.time<double>();

			m_interpreter_rec = std::make_unique<RecursiveInterpreter>(m_expression, ei);
			m_interpreter_stk = std::make_unique<StackInterpreter>(m_expression, ei);
			m_interpreter_ir = std::make_unique<IRInterpreter>(m_instructions);
		}
	}

	template<typename F>
	double bench(int count, F&& f) {
		double arg = rand() * 0.00147;
		evo::Timer timer;
		for (int i = 0; i < count; ++i) volatile double res = f(arg);
		return timer.time<double>();
	}

	void BenchmarkScene::gui() {
		constexpr const char* flfrmt = "%9.7f";
		static int evaluations = 1000;
		static int repetitions = 4;

		ImGui::Begin("Expression JIT Demo Benchmark");
		ImGui::Text(bench_fun_src);
		ImGui::LabelText("Time parsing", flfrmt, m_timeParse);
		ImGui::LabelText("Time compiling", flfrmt, m_timeComp);
		ImGui::Separator();
		ImGui::InputInt("Evaluations", &evaluations);
		ImGui::InputInt("Repetitions", &repetitions);

		if (ImGui::Button("Run")) {
			const auto apt = [this]() {
				m_timeAOT += bench(evaluations, bench_fun_aot);
				m_timeJIT += bench(evaluations, *m_function);
				m_timeIRC += bench(evaluations, *m_interpreter_rec);
				m_timeIST += bench(evaluations, *m_interpreter_stk);
				m_timeIIR += bench(evaluations, *m_interpreter_ir);
			};

			m_timeAOT = m_timeIIR = m_timeIRC = m_timeIST = m_timeJIT = 0.0;

			for (int r = 0; r < repetitions; ++r) apt();
			double k = 1.0 / repetitions;
			m_timeAOT *= k;
			m_timeJIT *= k;
			m_timeIRC *= k;
			m_timeIST *= k;
			m_timeIIR *= k;

			m_hasResult = true;
		}
		ImGui::End();

		if (m_hasResult) {
			ImGui::Begin("Result");
			ImGui::LabelText("AOT", flfrmt, m_timeAOT);
			ImGui::LabelText("JIT", flfrmt, m_timeJIT);
			ImGui::LabelText("Interpreter: recursive", flfrmt, m_timeIRC);
			ImGui::LabelText("Interpreter: stack (tree)", flfrmt, m_timeIST);
			ImGui::LabelText("Interpreter: stack (ir)", flfrmt, m_timeIIR);
			ImGui::End();
		}
	}
}