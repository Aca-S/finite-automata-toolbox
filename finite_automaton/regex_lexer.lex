%option noyywrap
%option nounput
%option noinput

%{
#include "regex_driver.hpp"
#include "regex_parser.tab.hpp"
%}

%%

[|?*+()] {
    return *yytext;
}

. {
    return yy::parser::make_SYM_T(*yytext);
}

%%

void RegexDriver::string_scan_init(const std::string &regex)
{
    yy_scan_string(regex.c_str());
}

void RegexDriver::string_scan_deinit()
{
    yy_delete_buffer(YY_CURRENT_BUFFER);
}
