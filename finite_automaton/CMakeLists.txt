add_subdirectory(regex_driver)

add_library(
    finite_automaton
    finite_automaton.cpp
)

target_link_libraries(
    finite_automaton 
    PUBLIC regex_driver
)

target_include_directories(
    finite_automaton PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/regex_driver"
)

enable_testing()

add_executable(
    finite_automaton_test
    finite_automaton_test.cpp
)

target_link_libraries(
    finite_automaton_test
    PUBLIC finite_automaton
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(finite_automaton_test)
