#pragma once
#include <cstdint>
#include <vector>
#include <type_traits>
#include "ir.h"
#include "opcode.h"

namespace exprjit
{
	template<typename T>
	concept Emittable = std::unsigned_integral<T> || std::same_as<T, Opcode>;

	class BinaryEncoder {
	public:
		typedef unsigned char uchar_t;
		typedef std::vector<uchar_t>& binary_t;

		BinaryEncoder(binary_t bin, size_t intArgs, size_t floatArgs) 
			: m_binary(bin), m_integerArguments(intArgs), m_floatArguments(floatArgs) { }

		virtual void operator()(const ir::Instruction&) = 0;

	protected:
		template<Emittable... TValues>
		constexpr void emit(TValues... values) {
			( emits(values), ... );
		}

		size_t m_integerArguments;
		size_t m_floatArguments;

	private:
		binary_t m_binary;

		template<typename T>
		constexpr void emits(T value);

		template<std::unsigned_integral T>
		void emits(T value) {
			m_binary.push_back((unsigned char)value);
		}

		template<>
		void emits<unsigned char>(unsigned char value) {
			m_binary.push_back(value);
		}
		template<>
		constexpr void emits<Opcode>(Opcode value) {
			for(unsigned i = 0; i < value.size(); ++i) m_binary.push_back(value[i]);
		}
	};
}