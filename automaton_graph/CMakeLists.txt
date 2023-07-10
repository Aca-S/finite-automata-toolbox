find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

qt_add_library(
    automaton_graph
    circle_node.cpp
)

target_link_libraries(
    automaton_graph
    PUBLIC finite_automaton
    PUBLIC qt_graph
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
)
