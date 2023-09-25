#include "main_window.hpp"

#include <QKeyEvent>
#include <QLayout>
#include <QWheelEvent>

#include "creation_dock.hpp"
#include "menu_bar.hpp"
#include "operations_dock.hpp"
#include "scene_tab_bar.hpp"
#include "view_dock.hpp"

using namespace Ui;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setWindowTitle("Finite Automata Toolbox");

    MainGraphicsView *main_view = new MainGraphicsView(this);
    SceneTabBar *tab_bar = new SceneTabBar(this);
    main_view->setScene(tab_bar->get_current_scene());

    QWidget *central_widget = new QWidget(this);
    QVBoxLayout *central_layout = new QVBoxLayout(central_widget);
    central_layout->addWidget(tab_bar);
    central_layout->addWidget(main_view);
    central_layout->setSpacing(0);
    this->setCentralWidget(central_widget);

    auto menu_bar = new MenuBar(this);
    menu_bar->set_scene(tab_bar->get_current_scene());
    this->setMenuBar(menu_bar);

    auto view_dock = new ViewDock(this);
    auto creation_dock = new CreationDock(this);
    auto operation_dock = new OperationsDock(this);

    this->addDockWidget(Qt::LeftDockWidgetArea, view_dock);
    this->addDockWidget(Qt::LeftDockWidgetArea, creation_dock);
    this->addDockWidget(Qt::RightDockWidgetArea, operation_dock);

    connect(menu_bar, &MenuBar::scene_opened, tab_bar, &SceneTabBar::add_scene);
    connect(menu_bar, &MenuBar::scene_closed, tab_bar, &SceneTabBar::remove_current_scene);
    connect(menu_bar, &MenuBar::scene_saved_as, tab_bar, &SceneTabBar::update_current_tab_label);

    connect(tab_bar, &SceneTabBar::scene_changed, main_view, &QGraphicsView::setScene);
    connect(tab_bar, &SceneTabBar::scene_changed, menu_bar, &MenuBar::set_scene);

    connect(view_dock, &ViewDock::operation_triggered, main_view, &MainGraphicsView::execute_operation);
    connect(creation_dock, &CreationDock::operation_triggered, main_view, &MainGraphicsView::execute_operation);
    connect(operation_dock, &OperationsDock::operation_triggered, main_view, &MainGraphicsView::execute_operation);
}

void MainWindow::MainGraphicsView::execute_operation(const std::function<void(QGraphicsView *view)> &op) { op(this); }

MainWindow::MainGraphicsView::MainGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSceneRect(-16000, -16000, 32000, 32000);
}

void MainWindow::MainGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        setDragMode(QGraphicsView::RubberBandDrag);
    QGraphicsView::keyPressEvent(event);
}

void MainWindow::MainGraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        setDragMode(QGraphicsView::ScrollHandDrag);
    QGraphicsView::keyPressEvent(event);
}

void MainWindow::MainGraphicsView::wheelEvent(QWheelEvent *event)
{
    qreal factor_of_change = 1.1;
    qreal max_scale_factor = 1.5;
    qreal min_scale_factor = 0.35;

    if (event->angleDelta().y() > 0) {
        qreal new_scale_factor = m_current_scale_factor * factor_of_change;
        if (new_scale_factor < max_scale_factor) {
            scale(factor_of_change, factor_of_change);
            m_current_scale_factor = new_scale_factor;
        }
    } else if (event->angleDelta().y() < 0) {
        qreal new_scale_factor = m_current_scale_factor / factor_of_change;
        if (new_scale_factor > min_scale_factor) {
            scale(1 / factor_of_change, 1 / factor_of_change);
            m_current_scale_factor = new_scale_factor;
        }
    }
}
