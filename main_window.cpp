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
    Node *n_1 = new Node(50, 50, "abc");
    Node *n_2 = new Node(50, 50, "qwe");
    Edge *e_1_2 = new Edge("a");
    g->add_node(n_1);
    g->add_node(n_2);
    g->add_edge(e_1_2, n_1, n_2);
    scene->addItem(g);
}

MainWindow::~MainWindow() { delete ui; }
