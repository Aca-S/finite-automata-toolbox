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
    SceneTabBar *tab_bar = new SceneTabBar(main_view, this);

    QWidget *central_widget = new QWidget(this);
    QVBoxLayout *central_layout = new QVBoxLayout(central_widget);
    central_layout->addWidget(tab_bar);
    central_layout->addWidget(main_view);
    central_layout->setSpacing(0);
    this->setCentralWidget(central_widget);

    this->setMenuBar(new MenuBar(tab_bar, this));

    auto view_dock = new ViewDock(main_view, this);
    auto creation_dock = new CreationDock(main_view, this);
    auto operation_dock = new OperationsDock(this);

    this->addDockWidget(Qt::LeftDockWidgetArea, view_dock);
    this->addDockWidget(Qt::LeftDockWidgetArea, creation_dock);
    this->addDockWidget(Qt::RightDockWidgetArea, operation_dock);

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
