#pragma once
#include <string_view>
#include <vector>
#include <exception>
#include <unordered_map>
#include "expression_node.h"

namespace exprjit
{
	class ParserException : public std::exception {
	public:
		ParserException(const char* str) : m_str(str) { }

		const char* what() const noexcept override {
			return m_str;
		}
	private:
		const char* m_str;
	};

	class Parser {
	public:
		typedef std::unordered_map<char, std::pair<unsigned, DataType>> argsmap_t;

		Parser(std::string_view s, std::vector<ExpressionNode>& expr, const argsmap_t& args)
			: m_str(s), m_expr(expr), m_argmap(args), m_i(0) { }

		size_t operator()();

	private:
		std::string_view m_str;
		size_t m_i;
		std::vector<ExpressionNode>& m_expr;
		const argsmap_t& m_argmap;

		struct Token {
			enum class Type {
				Literal,
				Argument,
				Delimiter,
				Operator
			};

			Type type;

			union {
				struct {
					char ch;
					signed char prec;
				} oper;
				char delimiter;
				struct {
					unsigned index;
					DataType type;
				} argument;
				struct {
					uint64_t value;
					DataType type;
				} literal;
			};
		};

		void lexLiteral(Token& tok);
		void lexArgument(Token& tok);
		void lexOperator(Token& tok);
		char lex(Token&);

		size_t parseExpression(char);
		size_t parsePrimary();
		std::pair<size_t, char> parseBinary(size_t lhs, char end, signed char prec = 0);
		size_t parseUnary(const Token& tok);
	};
}