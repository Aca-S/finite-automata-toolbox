#include "main_window.hpp"
#include "./ui_main_window.h"

#include "graph.hpp"
#include "node.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->main_view->setScene(scene);
    scene->addEllipse(0, 0, 10, 10);
    Graph *g = new Graph();
    Node *n_1 = new Node(g, "abc");
    Node *n_2 = new Node(g, "qwe");
    scene->addItem(g);
}

MainWindow::~MainWindow() { delete ui; }
