#pragma once
#include <vector>
#include "ir.h"

namespace exprjit::ir
{
	class Optimizer {
	public:
		Optimizer(std::vector<ir::Instruction>& ir) : m_ir(ir) { }
	
		void operator()();

	private:
		std::vector<ir::Instruction>& m_ir;

		static std::vector<size_t(*)( std::vector<Instruction>& ir, size_t start )> optimizations;
	};
}