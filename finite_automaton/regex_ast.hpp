#ifndef REGEX_AST_HPP
#define REGEX_AST_HPP

#include <memory>
#include <variant>

class RegexAST;

class ConcatenationAST
{
  public:
    ConcatenationAST(std::unique_ptr<RegexAST> left, std::unique_ptr<RegexAST> right);

  private:
    std::unique_ptr<RegexAST> m_left, m_right;
};

class AlternationAST
{
  public:
    AlternationAST(std::unique_ptr<RegexAST> left, std::unique_ptr<RegexAST> right);

  private:
    std::unique_ptr<RegexAST> m_left, m_right;
};

class ZeroOrOneAST
{
  public:
    ZeroOrOneAST(std::unique_ptr<RegexAST> operand);

  private:
    std::unique_ptr<RegexAST> m_operand;
};

class ZeroOrMoreAST
{
  public:
    ZeroOrMoreAST(std::unique_ptr<RegexAST> operand);

  private:
    std::unique_ptr<RegexAST> m_operand;
};

class OneOrMoreAST
{
  public:
    OneOrMoreAST(std::unique_ptr<RegexAST> operand);

  private:
    std::unique_ptr<RegexAST> m_operand;
};

class SymbolAST
{
  public:
    SymbolAST(char symbol);

  private:
    char m_symbol;
};

class RegexAST
    : public std::variant<ConcatenationAST, AlternationAST, ZeroOrOneAST, ZeroOrMoreAST, OneOrMoreAST, SymbolAST>
{};

#endif // REGEX_AST_HPP
