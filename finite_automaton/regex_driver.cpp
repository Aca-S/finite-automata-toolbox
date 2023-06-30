#include "regex_driver.hpp"

std::unique_ptr<RegexAST> RegexDriver::parse(const std::string &regex)
{
    string_scan_init(regex);
    yy::parser parser(*this);
    int res = parser();
    string_scan_deinit();

    return res != 0 ? std::move(m_ast) : nullptr;
}
