#ifndef REGEX_AST_HPP
#define REGEX_AST_HPP

#include <memory>
#include <variant>

class RegexAST;

class ConcatenationAST
{
  public:
    ConcatenationAST(std::unique_ptr<RegexAST> left, std::unique_ptr<RegexAST> right);
    const RegexAST &get_left() const;
    const RegexAST &get_right() const;

  private:
    std::unique_ptr<RegexAST> m_left, m_right;
};

class AlternationAST
{
  public:
    AlternationAST(std::unique_ptr<RegexAST> left, std::unique_ptr<RegexAST> right);
    const RegexAST &get_left() const;
    const RegexAST &get_right() const;

  private:
    std::unique_ptr<RegexAST> m_left, m_right;
};

class ZeroOrOneAST
{
  public:
    ZeroOrOneAST(std::unique_ptr<RegexAST> operand);
    const RegexAST &get_operand() const;

  private:
    std::unique_ptr<RegexAST> m_operand;
};

class ZeroOrMoreAST
{
  public:
    ZeroOrMoreAST(std::unique_ptr<RegexAST> operand);
    const RegexAST &get_operand() const;

  private:
    std::unique_ptr<RegexAST> m_operand;
};

class OneOrMoreAST
{
  public:
    OneOrMoreAST(std::unique_ptr<RegexAST> operand);
    const RegexAST &get_operand() const;

  private:
    std::unique_ptr<RegexAST> m_operand;
};

class SymbolAST
{
  public:
    SymbolAST(char symbol);
    char get_symbol() const;

  private:
    char m_symbol;
};

class RegexAST
    : public std::variant<ConcatenationAST, AlternationAST, ZeroOrOneAST, ZeroOrMoreAST, OneOrMoreAST, SymbolAST>
{};

// For overloaded lambdas...
template <class... Ts> struct overloaded : Ts...
{
    using Ts::operator()...;
};

template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

#endif // REGEX_AST_HPP
