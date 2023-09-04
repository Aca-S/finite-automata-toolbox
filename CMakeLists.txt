cmake_minimum_required(VERSION 3.23)
project(finite_automata_toolbox)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_subdirectory(finite_automaton)
add_subdirectory(qt_graph)
add_subdirectory(automaton_graph)

qt_add_executable(
    fat
    main.cpp
    main_window.cpp
    operations_dock.cpp
    creation_dock.cpp
    view_dock.cpp
)

target_link_libraries(
    fat 
    PUBLIC finite_automaton
    PUBLIC qt_graph
    PUBLIC automaton_graph
    PRIVATE Qt6::Widgets
)

set_target_properties(
    fat PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

target_include_directories(
    fat PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/finite_automaton"
    "${PROJECT_SOURCE_DIR}/qt_graph"
    "${PROJECT_SOURCE_DIR}/automaton_graph"
)

# Setting up GoogleTest

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.0
)

FetchContent_MakeAvailable(googletest)

enable_testing()
