#include "regex_ast.hpp"

ConcatenationAST::ConcatenationAST(std::unique_ptr<RegexAST> left, std::unique_ptr<RegexAST> right)
    : m_left(std::move(left)), m_right(std::move(right))
{
}

const RegexAST &ConcatenationAST::get_left() const { return *m_left; }

const RegexAST &ConcatenationAST::get_right() const { return *m_right; }

AlternationAST::AlternationAST(std::unique_ptr<RegexAST> left, std::unique_ptr<RegexAST> right)
    : m_left(std::move(left)), m_right(std::move(right))
{
}

const RegexAST &AlternationAST::get_left() const { return *m_left; }

const RegexAST &AlternationAST::get_right() const { return *m_right; }

ZeroOrOneAST::ZeroOrOneAST(std::unique_ptr<RegexAST> operand) : m_operand(std::move(operand)) {}

const RegexAST &ZeroOrOneAST::get_operand() const { return *m_operand; }

ZeroOrMoreAST::ZeroOrMoreAST(std::unique_ptr<RegexAST> operand) : m_operand(std::move(operand)) {}

const RegexAST &ZeroOrMoreAST::get_operand() const { return *m_operand; }

OneOrMoreAST::OneOrMoreAST(std::unique_ptr<RegexAST> operand) : m_operand(std::move(operand)) {}

const RegexAST &OneOrMoreAST::get_operand() const { return *m_operand; }

SymbolAST::SymbolAST(char symbol) : m_symbol(symbol) {}

char SymbolAST::get_symbol() const { return m_symbol; }
