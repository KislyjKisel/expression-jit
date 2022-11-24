#pragma once
#include <cstdint>

namespace exprjit::ir
{
	enum class VirtualRegister {
		I0, I1, IR, F0, F1, FR,
		IA0, IA1, IA2, IA3,
		FA0, FA1, FA2, FA3
	};

	struct Operand {
		bool immediate;
		union {
			uint64_t value;
			VirtualRegister  reg;
		};

		Operand(uint64_t value) : immediate(true), value(value) { }
		Operand(VirtualRegister vr) : immediate(false), reg(vr) { }

		friend struct Instruction;

	private:
		Operand() = default;
	};

	enum class Code {
		None,
		Ret,

		ILoadR,//VR  : Src       IMM : Value		Load literal to reg.
		ILoad, //IMM : Value						Push literal on stack.
		IArg,  //IMM : Index						Push argument on stack.
		IPush, //VR  : Src						Push virt reg on stack.
		IPop,  //VR  : Dst						Pop virt reg from stack.
		IMov,  //VR  : Src		 VR  : Dst		Assign VR[Src] value of VR[Dst].
		IAdd,
		ISub,
		IMul,
		IDiv,
		IMod,
		INeg,

		IAbs,

		FLoad,
		FArg,
		FPush,
		FPop,
		FMov,
		FAdd,
		FSub,
		FMul,
		FDiv,
		FMod,
		FNeg,

		FAbs,
		FSin,
		FCos,
		FTan,
		FFloor,

		IToF, //VRf : Dst		VRi : SRC			Move i-val from VRi to VRf.
		FToI, //VRi : Dst		VRf	: SRC			Move f-val from VRf to VRi.
	};

	struct Instruction {
		Code code;
		Operand operands[2];

		Instruction(Code code) : code(code) {}
	};
}