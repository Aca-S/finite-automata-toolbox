cmake_minimum_required(VERSION 3.23)
project(finite_automata_toolbox)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(finite_automaton)

add_executable(
    fat
    main.cpp
)

target_link_libraries(
    fat 
    PUBLIC finite_automaton
)

target_include_directories(
    fat PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/finite_automaton"
)

# Setting up GoogleTest

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.0
)

FetchContent_MakeAvailable(googletest)
