find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

qt_add_library(
    qt_graph
    graph.cpp
    node.cpp
    utility.cpp
)

target_link_libraries(
    qt_graph
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    PUBLIC cgraph gvc
)
