#include "main_window.hpp"
#include "./ui_main_window.h"

#include "circle_node.hpp"
#include "double_circle_node.hpp"
#include "graph.hpp"
#include "invisible_node.hpp"
#include "node.hpp"
#include "transition_edge.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->main_view->setScene(scene);
    scene->addEllipse(0, 0, 10, 10);
    Graph *g = new Graph();
    Node *n_1 = new CircleNode(50, "0");
    Node *n_2 = new DoubleCircleNode(50, 42, "1");
    Edge *e_1_2 = new TransitionEdge("a");
    Node *n_invis = new InvisibleNode();
    Edge *s_edge = new TransitionEdge("");
    g->add_node(n_1);
    g->add_node(n_2);
    g->add_edge(e_1_2, n_1, n_2);
    g->add_node(n_invis);
    g->add_edge(s_edge, n_invis, n_1);
    g->compose_layout();
    scene->addItem(g);
}

MainWindow::~MainWindow() { delete ui; }
