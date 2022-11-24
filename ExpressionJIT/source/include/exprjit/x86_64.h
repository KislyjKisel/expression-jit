#pragma once
#include <exception>
#include <unordered_map>
#include <numbers>
#include <functional>
#include "binary_encoder.h"

namespace exprjit
{
	//todo: unops and regextbit - unused? wrong? wtf

	template<typename T>
	concept Arithmetic = std::floating_point<T> || std::integral<T>;

	class X86_64 : public BinaryEncoder {
	public:
		X86_64(binary_t bin, size_t integerArgs, size_t floatArgs) : BinaryEncoder(bin, integerArgs, floatArgs) { }

	private:
#pragma region Registers
		constexpr static uint32_t RAX = 0b000;
		constexpr static uint32_t RCX = 0b001;
		constexpr static uint32_t RDX = 0b010;
		constexpr static uint32_t RBX = 0b011;
		constexpr static uint32_t RSP = 0b100;
		constexpr static uint32_t RBP = 0b101;
		constexpr static uint32_t RSI = 0b110;
		constexpr static uint32_t RDI = 0b111;

		constexpr static uint32_t R8 = 0b1000;
		constexpr static uint32_t R9 = 0b1001;
		constexpr static uint32_t R10 = 0b1010;
		constexpr static uint32_t R11 = 0b1011;
		constexpr static uint32_t R12 = 0b1100;
		constexpr static uint32_t R13 = 0b1101;
		constexpr static uint32_t R14 = 0b1110;
		constexpr static uint32_t R15 = 0b1111;

		constexpr static uint32_t XMM0 = 0b000;
		constexpr static uint32_t XMM1 = 0b001;
		constexpr static uint32_t XMM2 = 0b010;
		constexpr static uint32_t XMM3 = 0b011;
		constexpr static uint32_t XMM4 = 0b100;
		constexpr static uint32_t XMM5 = 0b101;
		constexpr static uint32_t XMM6 = 0b110;
		constexpr static uint32_t XMM7 = 0b111;

		constexpr static uint32_t reg_ext = 0b1000;
		constexpr static uint32_t reg_mask = 0b111;
		constexpr static uint32_t reg_argi[4] { RCX, RDX, R8, R9 };
		constexpr static uint32_t reg_argf[4] { XMM0, XMM1, XMM2, XMM3 };
#pragma endregion
#pragma region REX
		constexpr static uint32_t m_rex_base = 0b01000000;
		constexpr static uint32_t m_rex_w = 0b1000;
		constexpr static uint32_t m_rex_r = 0b0100; //ext REG
		constexpr static uint32_t m_rex_x = 0b0010;
		constexpr static uint32_t m_rex_b = 0b0001; //ext R/M

		constexpr static uint32_t rexw(uint32_t reg, uint32_t rm) {
			return m_rex_base | m_rex_w | ( reg & reg_ext ? m_rex_r : 0 ) | ( rm & reg_ext ? m_rex_b : 0 );
		}
#pragma endregion
#pragma region Opcodes
		class BadOpcodeException : public std::exception {
			const char* what() const noexcept override {
				return "Bad opcode.";
			}
		};

		enum class RegExtBit {
			B, R
		};

		class Prefix {
		public:
			constexpr static uint32_t None = 0;
			constexpr static uint32_t REX = 1 << 0;
			constexpr static uint32_t REXF = 1 << 1;
			constexpr static uint32_t x66 = 1 << 2;
			constexpr static uint32_t xF2 = 1 << 3;

			constexpr Prefix(uint32_t v) : m_value(v) { }

			constexpr bool has(uint32_t v) const {
				return m_value & v;
			}
			constexpr bool has(Prefix r) const {
				return m_value & r.m_value;
			}

		private:
			uint32_t m_value;
		};

		struct Instruction {
			enum class Type {
				Vop,		// No args
				Binop,	// /r
				Digop,	// /digit
				Unop		// +r*
			};

			static Instruction vop(X86_64& emitter, Opcode code) {
				return Instruction(emitter, code, Type::Vop, Prefix::None);
			}
			static Instruction unop(X86_64& emitter, Opcode code, Prefix prefix, RegExtBit rext) {
				return Instruction(emitter, code, Type::Unop, prefix, rext);
			}
			static Instruction binop(X86_64& emitter, Opcode code, Prefix prefix) {
				return Instruction(emitter, code, Type::Binop, prefix);
			}
			static Instruction digop(X86_64& emitter, Opcode code, Prefix prefix, uint32_t digit) {
				auto op = Instruction(emitter, code, Type::Digop, prefix);
				op.m_digit = digit;
				return op;
			}

			void operator()() {
#ifndef NDEBUG
				if (m_type != Type::Vop) [[unlikely]] {
					throw BadOpcodeException();
				}
#endif 
				m_emitter.emit(m_code);
			}
			void operator()(uint32_t reg, uint32_t rm) {
#ifndef NDEBUG
				if (m_type != Type::Binop) [[unlikely]] {
					throw BadOpcodeException();
				}
#endif
				if (m_prefix.has(Prefix::x66)) m_emitter.emit(0x66ui8);
				else if (m_prefix.has(Prefix::xF2)) m_emitter.emit(0xF2ui8);

				if (m_prefix.has(Prefix::REXF) || (m_prefix.has(Prefix::REX) && ( reg & reg_ext || rm & reg_ext ) ))
					m_emitter.emit(rexw(reg, rm));

				m_emitter.emit(
					m_code,
					modrm_rr(reg, rm)
				);
			}
			void operator()(uint32_t r) {
				switch (m_type) {
					case Type::Unop:
						if (m_prefix.has(Prefix::x66)) m_emitter.emit(0x66ui8);
						else if (m_prefix.has(Prefix::xF2)) m_emitter.emit(0xF2ui8);

						if (m_prefix.has(Prefix::REXF) || ( m_prefix.has(Prefix::REX) && r & reg_ext )) {
							m_emitter.emit(
								m_rex_base | m_rex_w | ( r & reg_ext ? (m_rext == RegExtBit::B ? m_rex_b : m_rex_r) : 0 )
							);
						}
						m_emitter.emit(m_code | ( r & reg_mask ));
						break;
					case Type::Digop:
						if (m_prefix.has(Prefix::x66)) m_emitter.emit(0x66ui8);
						else if (m_prefix.has(Prefix::xF2)) m_emitter.emit(0xF2ui8);

						if (m_prefix.has(Prefix::REXF) || ( m_prefix.has(Prefix::REX) && r & reg_ext )) {
							m_emitter.emit(m_rex_base | m_rex_w | ( r & reg_ext ? ( m_rext == RegExtBit::B ? m_rex_b : m_rex_r ) : 0 ));
						}
						m_emitter.emit(
							m_code,
							modrm_rr(m_digit, r)
						);
						break;
#ifndef NDEBUG
					[[unlikely]] default:
						throw BadOpcodeException();
#endif
				}
			}

		private:
			static unsigned char modrm_rr(uint32_t reg, uint32_t rm) noexcept {
				return (unsigned char)(
					( 0b11u << 6u ) |
					( ( reg & reg_mask ) << 3 ) |
					( ( rm & reg_mask ) )
					);
			}

			X86_64& m_emitter;
			Type m_type;
			uint32_t m_digit;
			Opcode m_code;
			Prefix m_prefix;
			RegExtBit m_rext;

			Instruction(X86_64& emitter, const Opcode& code, Type type, Prefix prefix, RegExtBit rext = RegExtBit::B) noexcept
				: m_type(type), m_emitter(emitter), m_code(code), m_prefix(prefix), m_digit(), m_rext(rext) { }
		};

		Instruction op_ret		= Instruction::vop(*this, { 0xC3 });

		Instruction op_movvi		= Instruction::unop(*this,	{ 0xB8			}, Prefix::REXF,		RegExtBit::B	);//[REG = IMM		]
		Instruction op_popi		= Instruction::unop(*this,	{ 0x58			}, Prefix::REX,		RegExtBit::B	);
		Instruction op_pushi		= Instruction::unop(*this,	{ 0x50			}, Prefix::REX,		RegExtBit::B	);
		Instruction op_movri		= Instruction::binop(*this,	{ 0x8B			}, Prefix::REXF					);//[REG = RM		]
		Instruction op_addri		= Instruction::binop(*this,	{ 0x03			}, Prefix::REXF					);//[REG = REG + R/M	]
		Instruction op_subri		= Instruction::binop(*this,	{ 0x2B			}, Prefix::REXF					);//[REG = REG - R/M]
		Instruction op_subvi 	= Instruction::digop(*this,	{ 0x81			}, Prefix::REXF,		5			);//[R/M = R/M - V32]
		Instruction op_mulri		= Instruction::binop(*this,	{ 0x0F, 0xAF		}, Prefix::REXF					);//[REG = REG * R/M	]
		Instruction op_xorri		= Instruction::binop(*this,	{ 0x33			}, Prefix::REXF					);//[REG = REG ^ R/M	]
		Instruction op_divri		= Instruction::digop(*this,	{ 0xF7			}, Prefix::REXF,		7			);
		Instruction op_negri		= Instruction::digop(*this,	{ 0xF7			}, Prefix::REXF,		3			);//[RM = -RM		]
		Instruction op_sarvi		= Instruction::digop(*this,	{ 0xC1			}, Prefix::REXF,		7			);//[RM = RM >>>	 i8 ]
		

		Instruction op_loadf		= Instruction::binop(*this, { 0x0F, 0x6E			}, Prefix::x66 | Prefix::REXF);	// [XMM = R/M]
		Instruction op_storef	= Instruction::binop(*this, { 0x0F, 0x7E			}, Prefix::x66 | Prefix::REXF); // [R/M = XMM]
		Instruction op_movf		= Instruction::binop(*this, { 0x0F, 0x10			}, Prefix::xF2);
		Instruction op_addf		= Instruction::binop(*this, { 0x0F, 0x58			}, Prefix::xF2);
		Instruction op_subf		= Instruction::binop(*this, { 0x0F, 0x5C			}, Prefix::xF2);
		Instruction op_mulf		= Instruction::binop(*this, { 0x0F, 0x59			}, Prefix::xF2);
		Instruction op_divf		= Instruction::binop(*this, { 0x0F, 0x5E			}, Prefix::xF2);
		Instruction op_xorf		= Instruction::binop(*this, { 0x0F, 0x57			}, Prefix::x66); // [REG = REG ^ R/M]
		Instruction op_andf		= Instruction::binop(*this, { 0x0F, 0x54			}, Prefix::x66); // [REG = REG & R/M]
		Instruction op_roundf	= Instruction::binop(*this, { 0x0F, 0x3A, 0x0B	}, Prefix::x66); // [REG = round R/M] [i8]

		Instruction op_pcmpeqw	= Instruction::binop(*this, { 0x0F, 0x75			}, Prefix::x66);
		Instruction op_psllqv	= Instruction::digop(*this, { 0x0F, 0x73			}, Prefix::x66,		6); // ... [i8]
		Instruction op_psrlqv	= Instruction::digop(*this, { 0x0F, 0x73			}, Prefix::x66,		2); // ... [i8]

		Instruction op_ftoi		= Instruction::binop(*this, { 0x0F, 0x2D }, Prefix::xF2 | Prefix::REXF); // [REG = (I) R/M]
		Instruction op_itof		= Instruction::binop(*this, { 0x0F, 0x2A }, Prefix::xF2 | Prefix::REXF); // [R/M = (D) REG]

#pragma endregion
		
		//uint64_t getBin(double v) {
		//	union {
		//		double vd;
		//		uint64_t vi;
		//	};
		//	vd = v;
		//	return vi;
		//}

		template<Arithmetic T >
		void value(T value) {
			union {
				T vi;
				unsigned char vc[sizeof(T)];
			};
			vi = value;
			for (size_t i = 0; i < sizeof(T); ++i) {
				emit(vc[i]);
			}
		}

		void pushf(uint32_t reg) {
			op_storef(reg, R11);
			op_pushi(R11);
		}
		void popf(uint32_t reg) {
			op_popi(R11);
			op_loadf(reg, R11);
		}
		void negf(uint32_t reg, uint32_t tmp) {
			op_movvi(R11);
			value<uint64_t>(0x8000000000000000);
			op_loadf(tmp, R11);
			op_xorf(reg, tmp);
		}
		void genf1(uint32_t reg) {
			op_pcmpeqw(reg, reg);
			op_psllqv(reg);
			value<uint8_t>(54ui8);
			op_psrlqv(reg);
			value<uint8_t>(2ui8);
		}
		void loadfv(uint32_t reg, double v) {
			op_movvi(R11);
			value<double>(v);
			op_loadf(reg, R11);
		}

		inline static std::unordered_map<ir::VirtualRegister, uint32_t> regMap {
			{ ir::VirtualRegister::I0, RAX	},
			{ ir::VirtualRegister::I1, R10	},
			{ ir::VirtualRegister::IR, RAX  },
			{ ir::VirtualRegister::F0, XMM4	},
			{ ir::VirtualRegister::F1, XMM5	},
			{ ir::VirtualRegister::FR, XMM0 }
		};

		std::unordered_map<ir::Code, std::function<void(const ir::Instruction&)>> emitterMap {
			{
				ir::Code::Ret,
				[this](const ir::Instruction&){
					op_ret(); 
				}
			},
			{
				ir::Code::ILoadR,
				[this](const ir::Instruction& i) {
					auto r = regMap.at(i.operands[0].reg);
					op_movvi(r);
					value<int64_t>(i.operands[1].value);
					op_pushi(r);
				}
			},
			{
				ir::Code::ILoad,
				[this](const ir::Instruction& i) {
					op_movvi(RAX);
					value<int64_t>(i.operands[0].value);
					op_pushi(RAX);
				}
			},
			{
				ir::Code::IArg,
				[this](const ir::Instruction& i) {
					op_pushi(reg_argi[i.operands[0].value]);
				}
			},
			{
				ir::Code::IPush,
				[this](const ir::Instruction& i) {
					op_pushi(regMap.at(i.operands[0].reg));
				}
			},
			{
				ir::Code::IPop,
				[this](const ir::Instruction& i) {
					op_popi(regMap.at(i.operands[0].reg));
				}
			},
			{
				ir::Code::IMov,
				[this](const ir::Instruction& i) {
					op_movri(regMap.at(i.operands[0].reg), regMap.at(i.operands[1].reg));
				}
			},
			{
				ir::Code::IAdd,
				[this](const ir::Instruction& i) {
					op_addri(regMap.at(i.operands[0].reg), regMap.at(i.operands[1].reg));
				}
			},
			{
				ir::Code::ISub,
				[this](const ir::Instruction& i) {
					op_subri(regMap.at(i.operands[0].reg), regMap.at(i.operands[1].reg));
				}
			},
			{
				ir::Code::IMul,
				[this](const ir::Instruction& i) {
					op_mulri(regMap.at(i.operands[0].reg), regMap.at(i.operands[1].reg));
				}
			},
			{
				ir::Code::IDiv,
				[this](const ir::Instruction& i) {
					uint32_t r0 = regMap.at(i.operands[0].reg), r1 = regMap.at(i.operands[1].reg);
					op_movri(RAX, r0);
					op_movri(R11, RDX);
					op_xorri(RDX, RDX);
					op_divri(r1);
					op_movri(r0, RAX);
					op_movri(RDX, R11);
				}
			},
			{
				ir::Code::IMod,
				[this](const ir::Instruction& i) {
					uint32_t r0 = regMap.at(i.operands[0].reg), r1 = regMap.at(i.operands[1].reg);
					op_movri(RAX, r0);
					op_movri(R11, RDX);
					op_xorri(RDX, RDX);
					op_divri(r1);
					op_movri(r0, RDX);
					op_movri(RDX, R11);
				}
			},
			{
				ir::Code::INeg,
				[this](const ir::Instruction& i) {
					op_negri(regMap.at(i.operands[0].reg));
				}
			},
			{
				ir::Code::IAbs,
				[this](const ir::Instruction& i) {
					uint32_t r0 = regMap.at(i.operands[0].reg);
					uint32_t r1 = r0 == RAX ? R10 : RAX;
					op_movri(r1, r0);
					op_sarvi(r1);
					value<uint8_t>(63);
					op_xorri(r0, r1);
					op_subri(r0, r1);
				}
			},

			{
				ir::Code::FLoad,
				[this](const ir::Instruction& i) {
					op_movvi(RAX);
					value<uint64_t>(i.operands[0].value);
					op_pushi(RAX);
				}
			},
			{
				ir::Code::FArg,
				[this](const ir::Instruction& i) {
					pushf(reg_argf[i.operands[0].value]);
				}
			},
			{
				ir::Code::FPush,
				[this](const ir::Instruction& i) {
					pushf(regMap.at(i.operands[0].reg));
				}
			},
			{
				ir::Code::FPop,
				[this](const ir::Instruction& i) {
					popf(regMap.at(i.operands[0].reg));
				}
			},
			{
				ir::Code::FMov,
				[this](const ir::Instruction& i) {
					op_movf(regMap.at(i.operands[0].reg), regMap.at(i.operands[1].reg));
				}
			},
			{
				ir::Code::FAdd,
				[this](const ir::Instruction& i) {
					op_addf(regMap.at(i.operands[0].reg), regMap.at(i.operands[1].reg));
				}
			},
			{
				ir::Code::FSub,
				[this](const ir::Instruction& i) {
					op_subf(regMap.at(i.operands[0].reg), regMap.at(i.operands[1].reg));
				}
			},
			{
				ir::Code::FMul,
				[this](const ir::Instruction& i) {
					op_mulf(regMap.at(i.operands[0].reg), regMap.at(i.operands[1].reg));
				}
			},
			{
				ir::Code::FDiv,
				[this](const ir::Instruction& i) {
					op_divf(regMap.at(i.operands[0].reg), regMap.at(i.operands[1].reg));
				}
			},
			{
				ir::Code::FNeg,
				[this](const ir::Instruction& i) {


					uint32_t xra = regMap.at(i.operands[0].reg);
					uint32_t xrt = xra == XMM4 ? XMM5 : XMM4;
					negf(xra, xrt);
				}
			},
			{
				ir::Code::FAbs,
				[this](const ir::Instruction& i) {
					op_movvi(RAX);
					value<uint64_t>(0x7fffffffffffffff);
					uint32_t xra = regMap.at(i.operands[0].reg);
					uint32_t xrt = xra == XMM4 ? XMM5 : XMM4;
					op_loadf(xrt, RAX);
					op_andf(xra, xrt);
				}
			},
			{
				ir::Code::FFloor,
				[this](const ir::Instruction& i) {
					uint32_t xra = regMap.at(i.operands[0].reg);
					op_roundf(xra, xra);
					value<uint8_t>(9ui8);
				}
			},
			{
				ir::Code::FSin,
				[this](const ir::Instruction& i) {
					//Taylor series, n = 10
					//XRA - sum, result
					//XRT = x^(i * 2 + 1), computed by multiplying by XMM2
					//XMM1 - addend, initially = x^(i * 2 + 1), divided by XMM3 and added to/subtracted from XRA 
					//XMM2 - x^2
					//XMM3 - divider, = (i * 2 + 1)!
					//XMM0 - used in argument reduction st2

					
					uint32_t xra = regMap.at(i.operands[0].reg);
					uint32_t xrt = xra == XMM4 ? XMM5 : XMM4;
					if (m_floatArguments > 0) {
						pushf(XMM0);
						if (m_floatArguments > 1) {
							pushf(XMM1);
							if (m_floatArguments > 2) {
								pushf(XMM2);
								if (m_floatArguments > 3) {
									pushf(XMM3);
								}
							}
						}
					}

					//argument reduction to [0; 2pi]				x' = x - 2pi * floor(x / 2pi);
					op_movf(xrt, xra);							// xra = x
					op_movvi(RAX);								// load 2pi to XMM2
					value<double>(2.0 * std::numbers::pi);		// ^
					op_loadf(XMM2, RAX);							// ^
					op_divf(xrt, XMM2);							// xrt = x / 2pi
					op_roundf(xrt, xrt);							// xrt = floor(xrt)
					value<uint8_t>(9ui8);						// ^ (rounding mode)
					op_mulf(xrt, XMM2);							// xrt = xrt (floor( x / 2pi)) * XMM2 (2pi)
					op_subf(xra, xrt);							// xra = xra - xrt <=> x' = x - floor(x / 2pi) 

					//argument reduction to [0; pi]			
					// [ sin (pi + x) = -sin x ]
					// XMM0 = -floor(x' / pi), x'' = x' + XMM0 * pi
					// result = (XMM0 * 2 + 1) * result  (XMM0 * 2 + 1 maps {-1, 0} to {-1, 1})
					// eq: x' > pi ? -f(x' - pi) : f(x') 
					op_movf(XMM0, xra);							// XMM0 = x'
					op_movvi(RAX);								// load pi to xrt
					value<double>(std::numbers::pi);				// ^
					op_loadf(xrt, RAX);							// ^
					op_divf(XMM0, xrt);							// XMM0 = = XMM0 / pi (= x' / pi)
					op_roundf(XMM0, XMM0);						// XMM0 = floor(XMM0)
					value<uint8_t>(9ui8);						// ^ (rounding mode)
					negf(XMM0, XMM1);							// XMM0 = -XMM0	(XMM1 is tmp reg to store sign mask)
					op_movf(XMM1, XMM0);							// XMM1 = XMM0
					op_mulf(XMM1, xrt);							// XMM1 = XMM1 * pi ( = XMM0 * pi)
					op_addf(xra, XMM1);							// xra = xra + XMM1 (x'' = x' + XMM0 * pi)

					//1, 2
					op_movf(xrt, xra);
					op_mulf(xrt, xrt);
					op_movf(XMM2, xrt);
					op_mulf(xrt, xra);
					op_movf(XMM1, xrt);
					op_movvi(RAX);
					value<double>(6.0);	// 3!
					//value<uint64_t>(getBin(6.0));
					op_loadf(XMM3, RAX);
					op_divf(XMM1, XMM3);
					op_subf(xra, XMM1);

					//3
					op_mulf(xrt, XMM2);
					op_movf(XMM1, xrt);
					op_movvi(RAX);
					value<double>(120.0); // 5!
					//value<uint64_t>(getBin(120.0));
					op_loadf(XMM3, RAX);
					op_divf(XMM1, XMM3);
					op_addf(xra, XMM1);

					//4
					op_mulf(xrt, XMM2);
					op_movf(XMM1, xrt);
					op_movvi(RAX);
					value<double>(5040.0); // 7!
					//value<uint64_t>(getBin(5040.0));
					op_loadf(XMM3, RAX);
					op_divf(XMM1, XMM3);
					op_subf(xra, XMM1);

					//5
					op_mulf(xrt, XMM2);
					op_movf(XMM1, xrt);
					op_movvi(RAX);
					value<double>(362880.0); // 9!
					//value<uint64_t>(getBin(362880.0));
					op_loadf(XMM3, RAX);
					op_divf(XMM1, XMM3);
					op_addf(xra, XMM1);

					//6
					op_mulf(xrt, XMM2);
					op_movf(XMM1, xrt);
					op_movvi(RAX);
					value<double>(39916800.0); // 11!
					//value<uint64_t>(getBin(39916800.0));
					op_loadf(XMM3, RAX);
					op_divf(XMM1, XMM3);
					op_subf(xra, XMM1);

					//7
					op_mulf(xrt, XMM2);
					op_movf(XMM1, xrt);
					op_movvi(RAX);
					value<double>(6227020800.0); // 13!
					//value<uint64_t>(getBin(6227020800.0));
					op_loadf(XMM3, RAX);
					op_divf(XMM1, XMM3);
					op_addf(xra, XMM1);

					//8
					op_mulf(xrt, XMM2);
					op_movf(XMM1, xrt);
					op_movvi(RAX);
					value<double>(1307674368000.0); // 15!
					//value<uint64_t>(getBin(1307674368000.0));
					op_loadf(XMM3, RAX);
					op_divf(XMM1, XMM3);
					op_subf(xra, XMM1);

					//9
					op_mulf(xrt, XMM2);
					op_movf(XMM1, xrt);
					op_movvi(RAX);
					value<double>(355687428096000.0); // 17!
					//value<uint64_t>(getBin(1307674368000.0));
					op_loadf(XMM3, RAX);
					op_divf(XMM1, XMM3);
					op_addf(xra, XMM1);

					//10
					op_mulf(xrt, XMM2);
					op_movf(XMM1, xrt);
					op_movvi(RAX);
					value<double>(121645100408832000.0); // 19!
					//value<uint64_t>(getBin(1307674368000.0));
					op_loadf(XMM3, RAX);
					op_divf(XMM1, XMM3);
					op_subf(xra, XMM1);

					op_addf(XMM0, XMM0);		//XMM0 *= 2
					genf1(XMM1);				//XMM1 = 1
					op_addf(XMM0, XMM1);		//XMM0 += 1
					op_mulf(xra, XMM0);

					if(m_floatArguments > 0) { 
						if (m_floatArguments > 1) {
							if (m_floatArguments > 2) {
								if (m_floatArguments > 3) {
									popf(XMM3);
								}
								popf(XMM2);
							}
							popf(XMM1);
						}
						popf(XMM0);
					}
				}
			},
			{
				ir::Code::FCos,
				[this](const ir::Instruction& i) {
					//Taylor series, n = 10
					//XRA - sum, result
					//XRT = x^(i * 2), computed by multiplying by XMM2
					//XMM1 - addend, initially = x^(i * 2), divided by XMM3 and added to/subtracted from XRA 
					//XMM2 - x^2
					//XMM3 - divider, = (i * 2)!
					//XMM0 - used in argument reduction st2


					uint32_t xra = regMap.at(i.operands[0].reg);
					uint32_t xrt = xra == XMM4 ? XMM5 : XMM4;
					if (m_floatArguments > 0) {
						pushf(XMM0);
						if (m_floatArguments > 1) {
							pushf(XMM1);
							if (m_floatArguments > 2) {
								pushf(XMM2);
								if (m_floatArguments > 3) {
									pushf(XMM3);
								}
							}
						}
					}

					//argument reduction to [0; 2pi]				x' = x - 2pi * floor(x / 2pi);
					op_movf(xrt, xra);							// xra = x
					op_movvi(RAX);								// load 2pi to XMM2
					value<double>(2.0 * std::numbers::pi);		// ^
					op_loadf(XMM2, RAX);							// ^
					op_divf(xrt, XMM2);							// xrt = x / 2pi
					op_roundf(xrt, xrt);							// xrt = floor(xrt)
					value<uint8_t>(9ui8);						// ^ (rounding mode)
					op_mulf(xrt, XMM2);							// xrt = xrt (floor( x / 2pi)) * XMM2 (2pi)
					op_subf(xra, xrt);							// xra = xra - xrt <=> x' = x - floor(x / 2pi) 

					//argument reduction to [0; pi]			
					// [ cos (pi + x) = -cos x ]
					// XMM0 = -floor(x' / pi), x'' = x' + XMM0 * pi
					// result = (XMM0 * 2 + 1) * result  (XMM0 * 2 + 1 maps {-1, 0} to {-1, 1})
					// eq: x' > pi ? -f(x' - pi) : f(x') 
					op_movf(XMM0, xra);							// XMM0 = x'
					op_movvi(RAX);								// load pi to xrt
					value<double>(std::numbers::pi);				// ^
					op_loadf(xrt, RAX);							// ^
					op_divf(XMM0, xrt);							// XMM0 = = XMM0 / pi (= x' / pi)
					op_roundf(XMM0, XMM0);						// XMM0 = floor(XMM0)
					value<uint8_t>(9ui8);						// ^ (rounding mode)
					negf(XMM0, XMM1);							// XMM0 = -XMM0	(XMM1 is tmp reg to store sign mask)
					op_movf(XMM1, XMM0);							// XMM1 = XMM0
					op_mulf(XMM1, xrt);							// XMM1 = XMM1 * pi ( = XMM0 * pi)
					op_addf(xra, XMM1);							// xra = xra + XMM1 (x'' = x' + XMM0 * pi)

					//1, 2
					op_movf(xrt, xra);		// xrt = x''
					genf1(xra);				// xra = 1
					op_mulf(xrt, xrt);		// xrt = x''^2
					op_movf(XMM2, xrt);		// xmm2 = x''^2
					op_movf(XMM1, xrt);		// xmm1 = x''^2
					op_movvi(RAX);
					value<double>(2.0);
					op_loadf(XMM3, RAX);
					op_divf(XMM1, XMM3);		// xmm1 = x''^2 / 2!
					op_subf(xra, XMM1);		// xra = 1 - x''^2 / 2!

					//3
					op_mulf(xrt, XMM2);		//xrt = x''^2 * x''^2 = x''^4
					op_movf(XMM1, xrt);		//xmm1 = xrt
					loadfv(XMM3, 24.0);		//xmm3 = 4!
					op_divf(XMM1, XMM3);		//xmm1 = xmm1 / xmm3 = xrt / 4! = x''^4 / 4!
					op_addf(xra, XMM1);

					//4
					op_mulf(xrt, XMM2);		
					op_movf(XMM1, xrt);	
					loadfv(XMM3, 720.0);	
					op_divf(XMM1, XMM3);		
					op_subf(xra, XMM1);

					//5
					op_mulf(xrt, XMM2);		
					op_movf(XMM1, xrt);		
					loadfv(XMM3, 40320.0);		
					op_divf(XMM1, XMM3);	
					op_addf(xra, XMM1);

					//6
					op_mulf(xrt, XMM2);
					op_movf(XMM1, xrt);
					loadfv(XMM3, 3628800.0);
					op_divf(XMM1, XMM3);		
					op_subf(xra, XMM1);

					//7
					op_mulf(xrt, XMM2);
					op_movf(XMM1, xrt);
					loadfv(XMM3, 479001600.0);
					op_divf(XMM1, XMM3);	
					op_addf(xra, XMM1);

					//8
					op_mulf(xrt, XMM2);
					op_movf(XMM1, xrt);
					loadfv(XMM3, 87178291200.0); // 14!
					op_divf(XMM1, XMM3);
					op_subf(xra, XMM1);

					//9
					op_mulf(xrt, XMM2);
					op_movf(XMM1, xrt);
					loadfv(XMM3, 20922789888000.0);  // 16!
					op_divf(XMM1, XMM3);
					op_addf(xra, XMM1);

					//10
					op_mulf(xrt, XMM2);
					op_movf(XMM1, xrt);
					loadfv(XMM3, 6402373705728000.0); // 18!
					op_divf(XMM1, XMM3);
					op_subf(xra, XMM1);

					op_addf(XMM0, XMM0);		//XMM0 *= 2
					genf1(XMM1);				//XMM1 = 1
					op_addf(XMM0, XMM1);		//XMM0 += 1
					op_mulf(xra, XMM0);

					if (m_floatArguments > 0) {
						if (m_floatArguments > 1) {
							if (m_floatArguments > 2) {
								if (m_floatArguments > 3) {
									popf(XMM3);
								}
								popf(XMM2);
							}
							popf(XMM1);
						}
						popf(XMM0);
					}
				}
			},
			{
				ir::Code::FToI,
				[this](const ir::Instruction& i) {
					op_ftoi(regMap.at(i.operands[1].reg), regMap.at(i.operands[0].reg));
				}
			},
			{
				ir::Code::IToF,
				[this](const ir::Instruction& i) {
					op_itof(regMap.at(i.operands[0].reg), regMap.at(i.operands[1].reg));
				}
			}
		};

	public:
		void operator()(const ir::Instruction& i) override {
			emitterMap.at(i.code)( i );
		}
	};
}