#pragma once
#include <unordered_map>
#include <vector>
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
	class ExpressionCompiler {
	public:
		template<typename T> requires std::integral<T> || std::floating_point<T>
		inline static constexpr exprjit::DataType ReturnDataType = 
			std::integral<T> ? exprjit::DataType::Integer : exprjit::DataType::Float;

		void clearArgs() {
			argmap.clear();
		}

		void arg(char name, unsigned index, exprjit::DataType type) {
			argmap.insert({ name, { index, type } });
		}


		template<typename ReturnType, typename... ArgumentTypes>
		auto* compile(std::string_view src) {
			expr.clear();
			ir.clear();
			binary.clear();

			size_t ei = exprjit::Parser(src, expr, argmap)();
			exprjit::ir::Generator(expr, ei, ir, ReturnDataType<ReturnType>)();
			exprjit::ir::Optimizer opt(ir);
			opt();
			exprjit::ir::jit(ir, make_unique<exprjit::X86_64>(binary, 0, sizeof...(ArgumentTypes)));
			return new exprjit::Function<ReturnType(ArgumentTypes...)>(binary);
		}

	private:
		std::vector<exprjit::ExpressionNode> expr;
		std::vector<exprjit::ir::Instruction> ir;
		std::vector<unsigned char> binary;

		std::unordered_map<char, std::pair<unsigned, exprjit::DataType>> argmap;
	};
}