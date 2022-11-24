#include "include/exprjit/parser.h"
#include <unordered_map>
#include <sstream>
#include <string>
#include <cctype>

namespace exprjit
{
	std::unordered_map<char, signed char> precendenceMap {
		{ '+', 0 }, { '-', 0 }, { '*', 1 }, { '/', 1 }, { '%', 1 }
	};
	std::unordered_map<char, char> delimMap {
		{ '(', ')' }, { '[', ']' }, { '{', '}' }
	};
	std::unordered_map<char, ExpressionNode::Binop> binopMap {
		{ '+', ExpressionNode::Binop::Add		},
		{ '-', ExpressionNode::Binop::Subtract	},
		{ '*', ExpressionNode::Binop::Multiply	},
		{ '/', ExpressionNode::Binop::Divide		},
		{ '%', ExpressionNode::Binop::Modulo		}
	};
	std::unordered_map<char, ExpressionNode::Unop> unopMap {
		{ 'd', ExpressionNode::Unop::IToF	},
		{ 'i', ExpressionNode::Unop::FToI	},
		{ '-', ExpressionNode::Unop::Negate },
		{ 'a', ExpressionNode::Unop::Abs		},
		{ 's', ExpressionNode::Unop::Sin		},
		{ 'c', ExpressionNode::Unop::Cos		},
		{ 'f', ExpressionNode::Unop::Floor	},

	};
	std::unordered_map<std::string, char > funcMap {
		{ "abs",		'a' },
		{ "sin",		's' },
		{ "cos",		'c' },
		{ "int",		'i' },
		{ "flt",		'd' },
		{ "floor",	'f' },

	};

	void Parser::lexLiteral(Token& tok) {
		std::stringstream ss;
		bool integer = true;
		while (m_i < m_str.size() && (std::isdigit(m_str[m_i]) || m_str[m_i] == '.')) {
			if (m_str[m_i] == '.') integer = false;
			ss << m_str[m_i++];
		}
		tok.type = Token::Type::Literal;
		union {
			uint64_t litData;
			double litValueF;
			int64_t litValueI;
		};
		if (integer) {
			ss >> litValueI;
			tok.literal.type = DataType::Integer;
		}
		else {
			ss >> litValueF;
			tok.literal.type = DataType::Float;
		}
		tok.literal.value = litData;
	}

	void Parser::lexArgument(Token& tok) {
		auto it = m_argmap.find(m_str[m_i]);
		if (it == m_argmap.end()) {
			std::stringstream ss;
			while (m_i < m_str.size() && isalpha(m_str[m_i])) ss << m_str[m_i++];
			std::string func = ss.str();
			auto fit = funcMap.find(func);
			if(fit == funcMap.end()) throw ParserException("Unknown argument or function name.");
			tok.type = Token::Type::Operator;
			tok.oper.ch = fit->second;
			tok.oper.prec = 100;
		}
		else {
			tok.type = Token::Type::Argument;
			tok.argument.index = it->second.first;
			tok.argument.type = it->second.second;
			++m_i;
		}
	}

	void Parser::lexOperator(Token& tok) {
		tok.type = Token::Type::Operator;
		auto it = precendenceMap.find(m_str[m_i]);
		if (it == precendenceMap.end()) throw ParserException("Unexpected character.");
		tok.oper.ch = m_str[m_i];
		tok.oper.prec = it->second;
		++m_i;
	}

	char Parser::lex(Token& tok) {
		if (m_i >= m_str.size()) return '\0';
		while (m_i < m_str.size() && (m_str[m_i] == ' ' || m_str[m_i] == '\n')) ++m_i;
		if (m_i >= m_str.size()) return '\0';
		char cc = m_str[m_i];
		if (std::isdigit(cc)) {
			lexLiteral(tok);
		}
		else if (std::isalpha(cc)) {
			lexArgument(tok);
		}
		else if (delimMap.contains(cc)) {
			tok.type = Token::Type::Delimiter;
			tok.delimiter = cc;
			++m_i;
		}
		else if (cc == ')' || cc == ']' || cc == '}') {
			++m_i;
			return cc;
		}
		else lexOperator(tok);
		return '\1';
	}

	size_t Parser::parseUnary(const Token& tok) {
		auto it = unopMap.find(tok.oper.ch);
		if (it == unopMap.end()) throw ParserException("Unknown unary operator.");
		auto node = ExpressionNode::makeUnop(it->second, parsePrimary());
		size_t ni = m_expr.size();
		m_expr.push_back(node);
		return ni;
	}

	size_t Parser::parsePrimary() {
		Token tok;
		if (!lex(tok)) throw ParserException("Expected primary expression.");
		size_t ni = m_expr.size();
		switch (tok.type) {
			case Token::Type::Literal:
				m_expr.push_back(ExpressionNode::makeLiteral(tok.literal.value, tok.literal.type));
				return ni;
			case Token::Type::Argument:
				m_expr.push_back(ExpressionNode::makeArgument(tok.argument.index, tok.argument.type));
				return ni;
			case Token::Type::Delimiter:{ 
				auto r = parseExpression(delimMap.at(tok.delimiter));
				++m_i;
				return r;
			}
			case Token::Type::Operator:
				return parseUnary(tok);
			default:
				throw ParserException("Unexpected primary token.");
		}
	}

	std::pair<size_t, char> Parser::parseBinary(size_t lhs, char end, signed char prec) {
		while (true) {
			Token copt;
			size_t li = m_i;
			char clr = lex(copt);
			if(clr != '\1' || copt.type != Token::Type::Operator) {
				m_i = li;
				return { lhs, clr };
			}
			auto copp = precendenceMap.find(copt.oper.ch);
			if (copp == precendenceMap.end() || copp->second < prec) {
				m_i = li;
				return { lhs, '\2' }; //?
			}
			size_t rhs = parsePrimary();
			signed char nextprec = -1;
			Token nopt;
			li = m_i;
			char nlr = lex(nopt);
			m_i = li;
			if (nlr == '\1' && nopt.type == Token::Type::Operator) {
				nextprec = precendenceMap.at(nopt.oper.ch);
			}
			if (copp->second < nextprec) {
				rhs = parseBinary(rhs, end, prec + 1).first;
			}
			size_t bop = m_expr.size();
			m_expr.push_back(ExpressionNode::makeBinop(binopMap.at(copt.oper.ch), lhs, rhs));
			lhs = bop;
		}
	}

	size_t Parser::parseExpression(char end) {
		size_t lhs = parsePrimary();
		auto bpr = parseBinary(lhs, end);
		if (end != bpr.second && end != '\0') throw ParserException("Unexpected char.");
		return bpr.first;
	}

	size_t Parser::operator()() {
		size_t i = 0;
		return parseExpression('\0');
	}
}