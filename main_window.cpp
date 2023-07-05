#include "main_window.hpp"
#include "./ui_main_window.h"

#include "graph.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Graph g;
}

MainWindow::~MainWindow() { delete ui; }
