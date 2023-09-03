#include "main_window.hpp"
#include "./ui_main_window.h"

#include "creation_dock.hpp"
#include "operations_dock.hpp"

#include "automaton_graph.hpp"
#include "finite_automaton.hpp"
#include "match_simulator.hpp"

#include <iterator>
#include <optional>
#include <ranges>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->main_view->setScene(scene);

    this->addDockWidget(Qt::RightDockWidgetArea, new OperationsDock(ui->main_view, this));
    this->addDockWidget(Qt::LeftDockWidgetArea, new CreationDock(ui->main_view, this));

    setup_view_dock();
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

namespace {
template <typename T> QList<T *> get_selected(QGraphicsScene *scene)
{
    using namespace std::views;

    auto selected = scene->selectedItems() | transform([](auto *item) { return qgraphicsitem_cast<T *>(item); })
                    | filter([](auto *item) { return item != nullptr; });
    return QList<T *>(selected.begin(), selected.end());
}

template <typename T> QList<T *> get_items(QGraphicsScene *scene)
{
    using namespace std::views;

    auto selected = scene->items() | transform([](auto *item) { return qgraphicsitem_cast<T *>(item); })
                    | filter([](auto *item) { return item != nullptr; });
    return QList<T *>(selected.begin(), selected.end());
}
} // namespace

namespace {
void execute_generator_operation(
    QGraphicsView *view, QLineEdit *result_le, QLabel *info_label, const QString &impossible_message,
    const auto &operation)
{
    info_label->setText("");
    auto graphs = get_items<AutomatonGraph>(view->scene());
    if (graphs.size() > 0) {
        auto generated = (graphs.at(0)->get_automaton().*operation)();
        if (generated)
            result_le->setText(QString::fromStdString(*generated));
        else
            info_label->setText(impossible_message);
    } else
        info_label->setText("An automaton must be selected.");
}

void execute_matcher_operation(
    QGraphicsView *view, QLineEdit *word_le, QLabel *info_label, std::optional<MatchSimulator> &match_simulator,
    const auto &operation, bool execute_initial_operation)
{
    info_label->setText("");
    auto graphs = get_items<AutomatonGraph>(view->scene());
    if (graphs.size() > 0) {
        auto graph = graphs.at(0);
        if (!match_simulator) {
            match_simulator.emplace(graph, word_le);
            if (execute_initial_operation)
                (match_simulator.value().*operation)();
        } else
            (match_simulator.value().*operation)();
    } else
        info_label->setText("An automaton must be selected.");
}
} // namespace

void MainWindow::setup_view_dock()
{
    ui->select_view->setScene(new QGraphicsScene(this));

    QString symbol_regex = "[!-~]";
    QRegularExpression symbols_only_regex("^" + symbol_regex + "*$");
    auto word_validator = new QRegularExpressionValidator(symbols_only_regex, this);
    set_validator(
        ui->test_word_le, word_validator, ui->view_dock_info,
        "Only printable ASCII symbols are allowed in the testing word.");

    connect(ui->view_selected_btn, &QPushButton::clicked, this, [=]() { bring_selected_to_view(); });

    connect(ui->generate_regex_btn, &QPushButton::clicked, this, [=]() {
        execute_generator_operation(
            ui->select_view, ui->generate_regex_le, ui->view_dock_info,
            "No regular expression matches selected automaton", &FiniteAutomaton::generate_regex);
    });

    connect(ui->generate_acceptable_btn, &QPushButton::clicked, this, [=]() {
        execute_generator_operation(
            ui->select_view, ui->test_word_le, ui->view_dock_info,
            "No acceptable words exist for the selected automaton.", &FiniteAutomaton::generate_valid_word);
    });

    connect(ui->generate_unacceptable_btn, &QPushButton::clicked, this, [=]() {
        execute_generator_operation(
            ui->select_view, ui->test_word_le, ui->view_dock_info,
            "No unacceptable words, under its alphabet, exist for the selected automaton.",
            &FiniteAutomaton::generate_invalid_word);
    });

    static std::optional<MatchSimulator> match_simulator;

    connect(ui->view_selected_btn, &QPushButton::clicked, this, [&]() { match_simulator.reset(); });

    connect(ui->test_word_le, &QLineEdit::textChanged, this, [&]() { match_simulator.reset(); });

    connect(ui->test_reset_btn, &QPushButton::clicked, this, [&]() {
        execute_matcher_operation(
            ui->select_view, ui->test_word_le, ui->view_dock_info, match_simulator, &MatchSimulator::first_step, false);
    });

    connect(ui->test_back_btn, &QPushButton::clicked, this, [&]() {
        execute_matcher_operation(
            ui->select_view, ui->test_word_le, ui->view_dock_info, match_simulator, &MatchSimulator::previous_step,
            false);
    });

    connect(ui->test_forward_btn, &QPushButton::clicked, this, [&]() {
        execute_matcher_operation(
            ui->select_view, ui->test_word_le, ui->view_dock_info, match_simulator, &MatchSimulator::next_step, false);
    });

    connect(ui->test_end_btn, &QPushButton::clicked, this, [&]() {
        execute_matcher_operation(
            ui->select_view, ui->test_word_le, ui->view_dock_info, match_simulator, &MatchSimulator::last_step, true);
    });
}

void MainWindow::bring_selected_to_view()
{
    ui->select_view->scene()->clear();
    ui->generate_regex_le->clear();
    ui->view_dock_info->setText("");

    auto graphs = get_selected<AutomatonGraph>(ui->main_view->scene());

    if (graphs.size() > 0) {
        auto *new_graph = new AutomatonGraph(graphs.at(0)->get_automaton());
        new_graph->setFlag(QGraphicsItem::ItemIsSelectable, false);
        new_graph->setFlag(QGraphicsItem::ItemIsMovable, false);
        add_item_at_pos(new_graph, ui->select_view->scene(), get_viewport_center_pos(ui->select_view));
    } else
        ui->view_dock_info->setText("An automaton must be selected from the main view.");
}
