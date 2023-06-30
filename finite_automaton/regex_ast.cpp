#include "regex_ast.hpp"

ConcatenationAST::ConcatenationAST(std::unique_ptr<RegexAST> left, std::unique_ptr<RegexAST> right)
    : m_left(std::move(left)), m_right(std::move(right))
{
}

AlternationAST::AlternationAST(std::unique_ptr<RegexAST> left, std::unique_ptr<RegexAST> right)
    : m_left(std::move(left)), m_right(std::move(right))
{
}

ZeroOrOneAST::ZeroOrOneAST(std::unique_ptr<RegexAST> operand) : m_operand(std::move(operand)) {}

ZeroOrMoreAST::ZeroOrMoreAST(std::unique_ptr<RegexAST> operand) : m_operand(std::move(operand)) {}

OneOrMoreAST::OneOrMoreAST(std::unique_ptr<RegexAST> operand) : m_operand(std::move(operand)) {}

SymbolAST::SymbolAST(char symbol) : m_symbol(symbol) {}
