#include "operations_dock.hpp"
#include "utility.hpp"

#include <QGroupBox>
#include <QLayout>
#include <QShortcut>

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

void execute_unary_operation(AutomataScene *scene, const auto &operation)
{
    auto selected_graphs = get_selected<AutomatonGraph>(scene);

    if (selected_graphs.empty())
        return;

    QList<QPair<QGraphicsItem *, QPointF>> new_graphs;
    for (auto *graph : selected_graphs) {
        auto *new_graph = new AutomatonGraph((graph->get_automaton().*operation)());
        new_graphs.append({new_graph, get_center_pos(graph)});
        new_graph->setSelected(true);
    }

    QList<QGraphicsItem *> old_graphs(selected_graphs.begin(), selected_graphs.end());
    scene->replace_automata(old_graphs, new_graphs);
}

void execute_binary_operation(AutomataScene *scene, QPointF viewport_center, const auto &operation)
{
    auto selected_graphs = get_selected<AutomatonGraph>(scene);

    if (selected_graphs.size() < 2)
        return;

    // TODO: Can be replaced with one call to std::ranges::fold_left_first
    // once it's more widely supported.
    auto it = selected_graphs.begin();
    auto acc = (*it)->get_automaton();
    for (std::advance(it, 1); it != selected_graphs.end(); ++it)
        acc = (acc.*operation)((*it)->get_automaton());
    auto *new_graph = new AutomatonGraph(acc);
    new_graph->setSelected(true);

    QList<QGraphicsItem *> old_graphs(selected_graphs.begin(), selected_graphs.end());
    scene->replace_automata(old_graphs, {{new_graph, viewport_center}});
}

void execute_clone(AutomataScene *scene)
{
    auto selected_graphs = get_selected<AutomatonGraph>(scene);

    if (selected_graphs.empty())
        return;

    QList<QPair<QGraphicsItem *, QPointF>> new_graphs;
    for (auto *graph : selected_graphs) {
        auto *new_graph = new AutomatonGraph((graph->get_automaton()));
        QPointF new_pos = get_center_pos(graph) + QPointF(20, 20);
        new_graphs.append({new_graph, new_pos});
        graph->setSelected(false);
        new_graph->setSelected(true);
    }
    scene->add_automata(new_graphs);
}

void execute_delete(AutomataScene *scene)
{
    if (scene->selectedItems().empty())
        return;

    scene->remove_automata(scene->selectedItems());
}
} // namespace

OperationsDock::OperationsDock(QWidget *parent) : QDockWidget(parent)
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

void OperationsDock::set_scene(AutomataScene *scene) { m_current_scene = scene; }

void OperationsDock::set_viewport_center(QPointF center) { m_viewport_center = center; }

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
        execute_unary_operation(m_current_scene, &FiniteAutomaton::determinize);
    });

    connect(m_minimize_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(m_current_scene, &FiniteAutomaton::minimize);
    });

    connect(m_complete_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(m_current_scene, &FiniteAutomaton::complete);
    });

    connect(m_reverse_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(m_current_scene, &FiniteAutomaton::reverse);
    });

    connect(m_complement_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(m_current_scene, &FiniteAutomaton::complement);
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
        execute_binary_operation(m_current_scene, m_viewport_center, &FiniteAutomaton::union_with);
    });

    connect(m_intersection_btn, &QPushButton::clicked, this, [=]() {
        execute_binary_operation(m_current_scene, m_viewport_center, &FiniteAutomaton::intersection_with);
    });

    connect(m_difference_btn, &QPushButton::clicked, this, [=]() {
        execute_binary_operation(m_current_scene, m_viewport_center, &FiniteAutomaton::difference_with);
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
    connect(m_clone_btn, &QPushButton::clicked, this, [=]() { execute_clone(m_current_scene); });

    connect(m_delete_btn, &QPushButton::clicked, this, [=]() { execute_delete(m_current_scene); });
    auto *delete_sc = new QShortcut(QKeySequence("Del"), this);
    connect(delete_sc, &QShortcut::activated, m_delete_btn, &QPushButton::click);
}
