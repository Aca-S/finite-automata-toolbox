#include "operations_dock.hpp"

#include <QGroupBox>
#include <QLayout>
#include <QPushButton>

#include <ranges>

#include "automaton_graph.hpp"
#include "finite_automaton.hpp"

namespace {
QGroupBox *create_operation_group(const QString &title, QBoxLayout *layout, const QList<QWidget *> &widgets)
{
    auto group = new QGroupBox(title);
    for (QWidget *widget : widgets)
        layout->addWidget(widget);
    group->setLayout(layout);

    return group;
}

QPointF get_center_pos(QGraphicsItem *item)
{
    auto br = item->boundingRect();
    return item->pos() + QPointF(br.width(), br.height()) / 2.0;
}

QPointF get_viewport_center_pos(QGraphicsView *view) { return view->mapToScene(view->rect().center()); }

void add_item_at_pos(QGraphicsItem *item, QGraphicsScene *scene, const QPointF &center_pos)
{
    auto br = item->boundingRect();
    item->setPos(center_pos - QPointF(br.width(), br.height()) / 2.0);
    scene->addItem(item);
}

template <typename T> QList<T *> get_selected(QGraphicsScene *scene)
{
    using namespace std::views;

    auto selected = scene->selectedItems() | transform([](auto *item) { return qgraphicsitem_cast<T *>(item); })
                    | filter([](auto *item) { return item != nullptr; });
    return QList<T *>(selected.begin(), selected.end());
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

    auto determinize_btn = new QPushButton("Determinize");
    auto minimize_btn = new QPushButton("Minimize");
    auto complete_btn = new QPushButton("Complete");
    auto reverse_btn = new QPushButton("Reverse");
    auto complement_btn = new QPushButton("Complement");
    auto union_btn = new QPushButton("Union");
    auto intersection_btn = new QPushButton("Intersection");
    auto difference_btn = new QPushButton("Difference");
    auto clone_btn = new QPushButton("Clone");
    auto delete_btn = new QPushButton("Delete");

    auto dock_container = new QWidget;
    this->setWidget(dock_container);
    auto main_layout = new QVBoxLayout(dock_container);

    auto unary_group = create_operation_group(
        "Unary", new QVBoxLayout, {determinize_btn, minimize_btn, complete_btn, reverse_btn, complement_btn});
    main_layout->addWidget(unary_group);

    auto binary_group =
        create_operation_group("Binary", new QVBoxLayout, {union_btn, intersection_btn, difference_btn});
    main_layout->addWidget(binary_group);

    auto general_group = create_operation_group("General", new QVBoxLayout, {clone_btn, delete_btn});
    main_layout->addWidget(general_group);

    main_layout->addStretch();

    connect(determinize_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(m_main_view, &FiniteAutomaton::determinize);
    });

    connect(minimize_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(m_main_view, &FiniteAutomaton::minimize);
    });

    connect(complete_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(m_main_view, &FiniteAutomaton::complete);
    });

    connect(reverse_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(m_main_view, &FiniteAutomaton::reverse);
    });

    connect(complement_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(m_main_view, &FiniteAutomaton::complement);
    });

    connect(union_btn, &QPushButton::clicked, this, [=]() {
        execute_binary_operation(m_main_view, &FiniteAutomaton::union_with);
    });

    connect(intersection_btn, &QPushButton::clicked, this, [=]() {
        execute_binary_operation(m_main_view, &FiniteAutomaton::intersection_with);
    });

    connect(difference_btn, &QPushButton::clicked, this, [=]() {
        execute_binary_operation(m_main_view, &FiniteAutomaton::difference_with);
    });

    connect(clone_btn, &QPushButton::clicked, this, [=]() { execute_clone(m_main_view); });

    connect(delete_btn, &QPushButton::clicked, this, [=]() { execute_delete(m_main_view); });
}
