find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

qt_add_library(
    ui
    main_window.cpp
    view_dock.cpp
    creation_dock.cpp
    operations_dock.cpp
    menu_bar.cpp
    scene_tab_bar.cpp
    utility.cpp
)

target_link_libraries(
    ui
    PUBLIC finite_automaton
    PUBLIC automaton_graph
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
)
