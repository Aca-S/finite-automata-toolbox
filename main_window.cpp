#include "main_window.hpp"
#include "./ui_main_window.h"

#include "creation_dock.hpp"
#include "operations_dock.hpp"
#include "view_dock.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->main_view->setScene(scene);

    this->addDockWidget(Qt::RightDockWidgetArea, new OperationsDock(ui->main_view, this));
    this->addDockWidget(Qt::LeftDockWidgetArea, new ViewDock(ui->main_view, this));
    this->addDockWidget(Qt::LeftDockWidgetArea, new CreationDock(ui->main_view, this));
}

MainWindow::~MainWindow() { delete ui; }
