find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

qt_add_library(
    qt_graph
    graph.cpp
    node.cpp
    edge.cpp
    utility.cpp
)

target_link_libraries(
    qt_graph
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    PRIVATE cgraph gvc
)

target_include_directories(
    qt_graph
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)
