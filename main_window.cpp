#include "main_window.hpp"
#include "./ui_main_window.h"

#include "automaton_graph.hpp"
#include "finite_automaton.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->main_view->setScene(scene);
    AutomatonGraph *g = new AutomatonGraph(*FiniteAutomaton::construct("(a|b)*"));
    scene->addItem(g);
}

MainWindow::~MainWindow() { delete ui; }
