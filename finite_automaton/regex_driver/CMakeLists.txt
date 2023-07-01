find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

bison_target(
    regex_parser
    regex_parser.ypp
    ${CMAKE_CURRENT_BINARY_DIR}/regex_parser.tab.cpp
)

flex_target(
    regex_lexer
    regex_lexer.lex
    ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cpp
)

add_library(
    regex_driver
    regex_driver.cpp
    regex_ast.cpp
    ${BISON_regex_parser_OUTPUTS}
    ${FLEX_regex_lexer_OUTPUTS}
)

target_include_directories(
    regex_driver PUBLIC
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
