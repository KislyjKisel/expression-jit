#pragma once
#include <vector>
#include <memory>
#include "binary_encoder.h"
#include "ir.h"

namespace exprjit::ir
{
	inline void jit(const std::vector<ir::Instruction>& ir, std::unique_ptr<BinaryEncoder>&& binaryEmitter) {
		for (size_t i = 0; i < ir.size(); ++i) {
			( *binaryEmitter )( ir[i] );
		}
	}
}