#include "include/exprjit/ir_optimizer.h"

namespace exprjit::ir
{
	void Optimizer::operator()() {
		for (size_t o = 0; o < optimizations.size(); ++o) {
			size_t i = 0;
			while (i < m_ir.size()) {
				i = optimizations[o](m_ir, i);
			}
		}
	}

	std::vector<size_t(*)( std::vector<Instruction>& ir, size_t start)> Optimizer::optimizations {
		[](std::vector<Instruction>& ir, size_t i) -> size_t {
			size_t ni = i + 1;
			if (ni >= ir.size()) return ni;
			if(ir[i].operands[0].reg == ir[ni].operands[0].reg) { 
				if (
					ir[i].code == Code::IPush && ir[ni].code == Code::IPop || 
					ir[i].code == Code::FPush && ir[ni].code == Code::FPop
				) {
					ir.erase(ir.begin() + i, ir.begin() + i + 2);
					return i;
				}
			}
			return ni;
		}
	};
}