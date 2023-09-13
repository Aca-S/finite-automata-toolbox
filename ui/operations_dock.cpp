#include "operations_dock.hpp"
#include "utility.hpp"

#include <QGroupBox>
#include <QLayout>

#include "automaton_graph.hpp"
#include "finite_automaton.hpp"

using namespace Ui;
using namespace Ui::Utility;

namespace {
QGroupBox *create_operation_group(const QString &title, QBoxLayout *layout, const QList<QWidget *> &widgets)
{
    auto group = new QGroupBox(title);
    for (QWidget *widget : widgets)
        layout->addWidget(widget);
    group->setLayout(layout);

    return group;
}

void execute_unary_operation(QGraphicsView *view, const auto &operation)
{
    for (auto *graph : get_selected<AutomatonGraph>(view->scene())) {
        auto *new_graph = new AutomatonGraph((graph->get_automaton().*operation)());
        add_item_at_pos(new_graph, view->scene(), get_center_pos(graph));
        new_graph->setSelected(true);
        view->scene()->removeItem(graph);
    }
}

void execute_binary_operation(QGraphicsView *view, const auto &operation)
{
    auto graphs = get_selected<AutomatonGraph>(view->scene());

    // TODO: Can be replaced with one call to std::ranges::fold_left_first
    // once it's more widely supported.
    if (graphs.size() > 1) {
        auto it = graphs.begin();
        auto acc = (*it)->get_automaton();
        view->scene()->removeItem(*it);
        for (std::advance(it, 1); it != graphs.end(); ++it) {
            acc = (acc.*operation)((*it)->get_automaton());
            view->scene()->removeItem(*it);
        }
        auto *new_graph = new AutomatonGraph(acc);
        add_item_at_pos(new_graph, view->scene(), get_viewport_center_pos(view));
        new_graph->setSelected(true);
    }
}

void execute_clone(QGraphicsView *view)
{
    for (auto *graph : get_selected<AutomatonGraph>(view->scene())) {
        auto *new_graph = new AutomatonGraph((graph->get_automaton()));
        add_item_at_pos(new_graph, view->scene(), get_center_pos(graph) + QPointF(20, 20));
        graph->setSelected(false);
        new_graph->setSelected(true);
    }
}

void execute_delete(QGraphicsView *view)
{
    for (auto *graph : get_selected<AutomatonGraph>(view->scene()))
        view->scene()->removeItem(graph);
}
} // namespace

OperationsDock::OperationsDock(QGraphicsView *main_view, QWidget *parent) : QDockWidget(parent), m_main_view(main_view)
{
    this->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    this->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    auto dock_container = new QWidget;
    dock_container->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    this->setWidget(dock_container);
    auto main_layout = new QVBoxLayout(dock_container);

    build_unary_group();
    setup_unary_group();

    build_binary_group();
    setup_binary_group();

    build_general_group();
    setup_general_group();

    main_layout->addStretch();
}

void OperationsDock::build_unary_group()
{
    m_determinize_btn = new QPushButton("Determinize");
    m_minimize_btn = new QPushButton("Minimize");
    m_complete_btn = new QPushButton("Complete");
    m_reverse_btn = new QPushButton("Reverse");
    m_complement_btn = new QPushButton("Complement");

    auto unary_group = create_operation_group(
        "Unary", new QVBoxLayout, {m_determinize_btn, m_minimize_btn, m_complete_btn, m_reverse_btn, m_complement_btn});
    this->widget()->layout()->addWidget(unary_group);
}

void OperationsDock::setup_unary_group()
{
    connect(m_determinize_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(m_main_view, &FiniteAutomaton::determinize);
    });

    connect(m_minimize_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(m_main_view, &FiniteAutomaton::minimize);
    });

    connect(m_complete_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(m_main_view, &FiniteAutomaton::complete);
    });

    connect(m_reverse_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(m_main_view, &FiniteAutomaton::reverse);
    });

    connect(m_complement_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(m_main_view, &FiniteAutomaton::complement);
    });
}

void OperationsDock::build_binary_group()
{
    m_union_btn = new QPushButton("Union");
    m_intersection_btn = new QPushButton("Intersection");
    m_difference_btn = new QPushButton("Difference");

    auto binary_group =
        create_operation_group("Binary", new QVBoxLayout, {m_union_btn, m_intersection_btn, m_difference_btn});
    this->widget()->layout()->addWidget(binary_group);
}

void OperationsDock::setup_binary_group()
{
    connect(m_union_btn, &QPushButton::clicked, this, [=]() {
        execute_binary_operation(m_main_view, &FiniteAutomaton::union_with);
    });

    connect(m_intersection_btn, &QPushButton::clicked, this, [=]() {
        execute_binary_operation(m_main_view, &FiniteAutomaton::intersection_with);
    });

    connect(m_difference_btn, &QPushButton::clicked, this, [=]() {
        execute_binary_operation(m_main_view, &FiniteAutomaton::difference_with);
    });
}

void OperationsDock::build_general_group()
{
    m_clone_btn = new QPushButton("Clone");
    m_delete_btn = new QPushButton("Delete");

    auto general_group = create_operation_group("General", new QVBoxLayout, {m_clone_btn, m_delete_btn});
    this->widget()->layout()->addWidget(general_group);
}

void OperationsDock::setup_general_group()
{
    connect(m_clone_btn, &QPushButton::clicked, this, [=]() { execute_clone(m_main_view); });

    connect(m_delete_btn, &QPushButton::clicked, this, [=]() { execute_delete(m_main_view); });
}
