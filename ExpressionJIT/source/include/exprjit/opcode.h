#pragma once
#include <initializer_list>
#include <cstdint>

namespace exprjit
{
	struct Opcode {
		typedef unsigned char uchar_type;

		constexpr Opcode() : m_size(0), m_value(0) { }
		constexpr Opcode(uchar_type a) : m_data { a, 0, 0, 0 }, m_size(1) { }
		constexpr Opcode(std::initializer_list<uchar_type> l) : m_size(l.size()), m_data {} {
			auto lit = l.begin();
			for (unsigned i = 0; i < l.size(); ++i, ++lit) m_data[i] = *lit;
		}

		constexpr size_t size() const {
			return m_size;
		}

		constexpr unsigned char operator[](unsigned i) const { return m_data[i]; }
		constexpr unsigned char& operator[](unsigned i) { return m_data[i]; }

		Opcode operator|(const Opcode& other) const {
			Opcode res;
			res.m_size = std::min(m_size, other.m_size);
			res.m_value = other.m_value | m_value;
			return res;
		}
		Opcode operator&(const Opcode& other) const {
			Opcode res;
			res.m_size = std::min(m_size, other.m_size);
			res.m_value = other.m_value & m_value;
			return res;
		}
		Opcode operator^(const Opcode& other) const {
			Opcode res;
			res.m_size = std::min(m_size, other.m_size);
			res.m_value = other.m_value ^ m_value;
			return res;
		}
		
		Opcode operator|(uint32_t other) const {
			Opcode res;
			res.m_size = m_size;
			res.m_value = other | m_value;
			return res;
		}
		Opcode operator&(uint32_t other) const {
			Opcode res;
			res.m_size = m_size;
			res.m_value = other & m_value;
			return res;
		}
		Opcode operator^(uint32_t other) const {
			Opcode res;
			res.m_size = m_size;
			res.m_value = other ^ m_value;
			return res;
		}

	private:
		unsigned int m_size;
		union {
			unsigned char m_data[4];
			uint32_t m_value;
		};
	};
}