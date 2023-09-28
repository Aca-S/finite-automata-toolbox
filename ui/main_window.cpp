#include "main_window.hpp"

#include <QLayout>

using namespace Ui;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    build();
    setup();
}

void MainWindow::build()
{
    this->setWindowTitle("Finite Automata Toolbox");

    m_main_view = new MainGraphicsView(this);
    m_tab_bar = new SceneTabBar(this);
    m_main_view->setScene(m_tab_bar->get_current_scene());

    auto *central_widget = new QWidget(this);
    auto *central_layout = new QVBoxLayout(central_widget);
    central_layout->addWidget(m_tab_bar);
    central_layout->addWidget(m_main_view);
    central_layout->setSpacing(0);
    this->setCentralWidget(central_widget);

    m_menu_bar = new MenuBar(this);
    m_menu_bar->set_scene(m_tab_bar->get_current_scene());
    this->setMenuBar(m_menu_bar);

    m_view_dock = new ViewDock(this);
    m_view_dock->set_scene(m_tab_bar->get_current_scene());
    m_creation_dock = new CreationDock(this);
    m_creation_dock->set_scene(m_tab_bar->get_current_scene());
    m_operations_dock = new OperationsDock(this);
    m_operations_dock->set_scene(m_tab_bar->get_current_scene());

    this->addDockWidget(Qt::LeftDockWidgetArea, m_view_dock);
    this->addDockWidget(Qt::LeftDockWidgetArea, m_creation_dock);
    this->addDockWidget(Qt::RightDockWidgetArea, m_operations_dock);
}

void MainWindow::setup()
{
    connect(m_menu_bar, &MenuBar::scene_opened, m_tab_bar, &SceneTabBar::add_scene);
    connect(m_menu_bar, &MenuBar::scene_closed, m_tab_bar, &SceneTabBar::remove_current_scene);
    connect(m_menu_bar, &MenuBar::scene_saved_as, m_tab_bar, &SceneTabBar::update_current_tab_label);

    connect(m_tab_bar, &SceneTabBar::scene_changed, m_main_view, &QGraphicsView::setScene);
    connect(m_tab_bar, &SceneTabBar::scene_changed, m_menu_bar, &MenuBar::set_scene);
    connect(m_tab_bar, &SceneTabBar::scene_changed, m_view_dock, &ViewDock::set_scene);
    connect(m_tab_bar, &SceneTabBar::scene_changed, m_creation_dock, &CreationDock::set_scene);
    connect(m_tab_bar, &SceneTabBar::scene_changed, m_operations_dock, &OperationsDock::set_scene);

    connect(
        m_main_view, &MainGraphicsView::viewport_center_changed, m_creation_dock, &CreationDock::set_viewport_center);
    connect(
        m_main_view, &MainGraphicsView::viewport_center_changed, m_operations_dock,
        &OperationsDock::set_viewport_center);
}
