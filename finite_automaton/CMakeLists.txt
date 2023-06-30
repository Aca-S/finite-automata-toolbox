find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

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
    finite_automaton
    finite_automaton.cpp
    regex_ast.cpp
    regex_driver.cpp
    ${BISON_regex_parser_OUTPUTS}
    ${FLEX_regex_parser_OUTPUTS}
)

enable_testing()

add_executable(
    finite_automaton_test
    finite_automaton_test.cpp
    finite_automaton.cpp
)

target_link_libraries(
    finite_automaton_test
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(finite_automaton_test)
