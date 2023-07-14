#include "main_window.hpp"
#include "./ui_main_window.h"

#include "automaton_graph.hpp"
#include "finite_automaton.hpp"

#include <iterator>
#include <ranges>
#include <string>
#include <string_view>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->main_view->setScene(scene);

    setup_construction_dock();
    setup_operations_dock();
}

MainWindow::~MainWindow() { delete ui; }

namespace {
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
} // namespace

namespace {
// Sets a line edit validator and optionally an info label which will display
// a specified message on invalid inputs or when focus has left the line edit
// while it contains a non-acceptable text.
void set_validator(
    QLineEdit *line_edit, QValidator *validator, QLabel *info_label = nullptr, const QString &info_message = "")
{
    line_edit->setValidator(validator);
    if (info_label) {
        QObject::connect(line_edit, &QLineEdit::textChanged, info_label, [=]() { info_label->setText(""); });
        QObject::connect(
            line_edit, &QLineEdit::inputRejected, info_label, [=]() { info_label->setText(info_message); });
        QObject::connect(qApp, &QApplication::focusChanged, info_label, [=](auto *old, auto *now) {
            if ((old == line_edit || now == line_edit) && !line_edit->hasAcceptableInput())
                info_label->setText(info_message);
            else if (old == line_edit)
                info_label->setText(""); // Last input was rejected, but without it, the text would be valid.
        });
    }
}
} // namespace

void MainWindow::setup_construction_dock()
{
    QString symbol_regex = "[ -~]"; // Printable ASCII characters.
    QString unsigned_regex = "(0|[1-9]\\d*)";

    QRegularExpression sym_list_regex("^(" + symbol_regex + "( " + symbol_regex + ")*)?$");
    QRegularExpression num_list_regex("^(" + unsigned_regex + "( " + unsigned_regex + ")*)?$");
    QRegularExpression transition_regex("^" + unsigned_regex + " " + symbol_regex + " " + unsigned_regex + "$");

    auto alphabet_validator = new QRegularExpressionValidator(sym_list_regex, this);
    auto states_validator = new QRegularExpressionValidator(num_list_regex, this);
    auto transition_validator = new QRegularExpressionValidator(transition_regex, this);

    set_validator(
        ui->alphabet_le, alphabet_validator, ui->construction_by_element_info,
        "The alphabet input must be a space separated list of printable ASCII characters.");
    set_validator(
        ui->states_le, states_validator, ui->construction_by_element_info,
        "The states input must be a space separated list of numbers.");
    set_validator(
        ui->initial_states_le, states_validator, ui->construction_by_element_info,
        "The initial states input must be a space separated list of numbers.");
    set_validator(
        ui->final_states_le, states_validator, ui->construction_by_element_info,
        "The final states input must be a space separated list of numbers.");
    set_validator(
        ui->transition_le, transition_validator, ui->construction_by_element_info,
        "The transition input must be in the form of <state symbol state>.");

    QRegularExpression symbols_only_regex("^" + symbol_regex + "*$");
    auto regex_validator = new QRegularExpressionValidator(symbols_only_regex, this);
    set_validator(
        ui->regex_construction_le, regex_validator, ui->construction_by_regex_info,
        "Only printable ASCII symbols are allowed in the RegEx.");

    connect(ui->add_transition_btn, &QPushButton::clicked, ui->transition_list, [=]() {
        if (ui->transition_le->hasAcceptableInput())
            ui->transition_list->addItem(ui->transition_le->text());
    });

    connect(ui->remove_transition_btn, &QPushButton::clicked, ui->transition_list, [=]() {
        for (auto *transition : ui->transition_list->selectedItems())
            delete ui->transition_list->takeItem(ui->transition_list->row(transition));
    });

    connect(ui->construction_by_element_btn, &QPushButton::clicked, this, [=]() { construct_by_element(); });

    connect(ui->construction_by_regex_btn, &QPushButton::clicked, this, [=]() { construct_by_regex(); });
}

void MainWindow::construct_by_element()
{
    for (QLineEdit *le : {ui->alphabet_le, ui->states_le, ui->initial_states_le, ui->final_states_le}) {
        if (!le->hasAcceptableInput()) {
            le->setFocus();
            return;
        }
    }

    const static auto input_to_container = [](const QString &str, auto &container, const auto &transform_fn) {
        using std::operator""sv;
        using namespace std::views;

        std::ranges::copy(
            split(std::string_view(str.toUtf8().constData()), " "sv)
                | transform([](const auto &sv) { return std::string(sv.begin(), sv.end()); }) | transform(transform_fn),
            std::inserter(container, container.end()));
    };

    std::set<char> alphabet;
    input_to_container(ui->alphabet_le->text(), alphabet, [](const auto &s) { return s[0]; });

    std::set<unsigned> states;
    input_to_container(ui->states_le->text(), states, [](const auto &s) { return std::stoul(s); });

    std::set<unsigned> initial_states;
    input_to_container(ui->initial_states_le->text(), initial_states, [](const auto &s) { return std::stoul(s); });

    std::set<unsigned> final_states;
    input_to_container(ui->final_states_le->text(), final_states, [](const auto &s) { return std::stoul(s); });

    std::map<std::pair<unsigned, char>, std::set<unsigned>> transition_function;
    for (auto i = 0; i < ui->transition_list->count(); ++i) {
        QString transition = ui->transition_list->item(i)->text();
        auto spl = transition.split(" ");
        unsigned from_state = std::stoul(std::string(spl[0].toUtf8().constData()));
        char transition_symbol = std::string(spl[1].toUtf8().constData())[0];
        unsigned to_state = std::stoul(std::string(spl[2].toUtf8().constData()));
        transition_function[{from_state, transition_symbol}].insert(to_state);
    }

    auto automaton = FiniteAutomaton::construct(alphabet, states, initial_states, final_states, transition_function);
    if (automaton) {
        auto graph = new AutomatonGraph(*automaton);
        add_item_at_pos(graph, ui->main_view->scene(), get_viewport_center_pos(ui->main_view));
    } else
        ui->construction_by_element_info->setText(QString::fromUtf8(automaton.error().c_str()));
}

void MainWindow::construct_by_regex()
{
    std::string regex(ui->regex_construction_le->text().toUtf8().constData());
    auto automaton = FiniteAutomaton::construct(regex);
    if (automaton) {
        auto graph = new AutomatonGraph(*automaton);
        add_item_at_pos(graph, ui->main_view->scene(), get_viewport_center_pos(ui->main_view));
    } else
        ui->construction_by_regex_info->setText(QString::fromUtf8(automaton.error().c_str()));
}

namespace {
void execute_unary_operation(QGraphicsView *view, const auto &operation)
{
    for (auto *selected : view->scene()->selectedItems()) {
        auto *graph = qgraphicsitem_cast<AutomatonGraph *>(selected);
        if (graph) {
            auto *new_graph = new AutomatonGraph((graph->get_automaton().*operation)());
            add_item_at_pos(new_graph, view->scene(), get_center_pos(graph));
            new_graph->setSelected(true);
            view->scene()->removeItem(graph);
        }
    }
}

void execute_binary_operation(QGraphicsView *view, const auto &operation)
{
    using namespace std::views;

    auto graphs = view->scene()->selectedItems()
                  | transform([](auto *item) { return qgraphicsitem_cast<AutomatonGraph *>(item); })
                  | filter([](auto *graph) { return graph != nullptr; });

    // TODO: Can be replaced with one call to std::ranges::fold_left_first
    // once it's more widely supported.
    if (std::ranges::distance(graphs.begin(), graphs.end()) > 1) {
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
    for (auto *selected : view->scene()->selectedItems()) {
        auto *graph = qgraphicsitem_cast<AutomatonGraph *>(selected);
        if (graph) {
            auto *new_graph = new AutomatonGraph((graph->get_automaton()));
            add_item_at_pos(new_graph, view->scene(), get_center_pos(graph) + QPointF(20, 20));
            graph->setSelected(false);
            new_graph->setSelected(true);
        }
    }
}

void execute_delete(QGraphicsView *view)
{
    for (auto *selected : view->scene()->selectedItems()) {
        auto *graph = qgraphicsitem_cast<AutomatonGraph *>(selected);
        if (graph)
            view->scene()->removeItem(graph);
    }
}
} // namespace

void MainWindow::setup_operations_dock()
{
    connect(ui->determinize_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(ui->main_view, &FiniteAutomaton::determinize);
    });

    connect(ui->minimize_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(ui->main_view, &FiniteAutomaton::minimize);
    });

    connect(ui->complete_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(ui->main_view, &FiniteAutomaton::complete);
    });

    connect(ui->reverse_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(ui->main_view, &FiniteAutomaton::reverse);
    });

    connect(ui->complement_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(ui->main_view, &FiniteAutomaton::complement);
    });

    connect(ui->union_btn, &QPushButton::clicked, this, [=]() {
        execute_binary_operation(ui->main_view, &FiniteAutomaton::union_with);
    });

    connect(ui->intersection_btn, &QPushButton::clicked, this, [=]() {
        execute_binary_operation(ui->main_view, &FiniteAutomaton::intersection_with);
    });

    connect(ui->difference_btn, &QPushButton::clicked, this, [=]() {
        execute_binary_operation(ui->main_view, &FiniteAutomaton::difference_with);
    });

    connect(ui->clone_btn, &QPushButton::clicked, this, [=]() { execute_clone(ui->main_view); });

    connect(ui->delete_btn, &QPushButton::clicked, this, [=]() { execute_delete(ui->main_view); });
}
