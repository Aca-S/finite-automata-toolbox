add_library(
    finite_automaton
    finite_automaton.cpp
)

enable_testing()

add_executable(
    finite_automaton_test
    finite_automaton_test.cpp
)

target_link_libraries(
    finite_automaton_test
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(finite_automaton_test)
