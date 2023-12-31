#ifndef REGEX_DRIVER_HPP
#define REGEX_DRIVER_HPP

#include "regex_ast.hpp"
#include "regex_parser.tab.hpp"

#include <memory>

class RegexDriver
{
    // Needed for accessing m_ast from the parser.
    friend class yy::parser;

  public:
    std::unique_ptr<RegexAST> parse(const std::string &regex);

  private:
    // Implemented in the lexer file - alternatively,
    // do extern declarations for the needed lexer functions.
    void string_scan_init(const std::string &regex);
    void string_scan_deinit();

    std::unique_ptr<RegexAST> m_ast;
};

// By default, yylex's signature is int yylex(void),
// so we have to redefine it.
#define YY_DECL yy::parser::symbol_type yylex(RegexDriver &driver)
// Declare yylex for use in the parser.
YY_DECL;

#endif // REGEX_DRIVER_HPP
