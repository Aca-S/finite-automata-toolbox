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

    connect(ui->transition_btn, &QPushButton::clicked, ui->transition_list, [=]() {
        if (ui->transition_le->hasAcceptableInput())
            ui->transition_list->addItem(ui->transition_le->text());
    });

    connect(ui->construct_btn, &QPushButton::clicked, this, [=]() {
        if (ui->by_element_rb->isChecked())
            construct_by_element();
        else if (ui->by_regex_rb->isChecked())
            construct_by_regex();
    });
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
        ui->main_view->scene()->addItem(graph);
    } else
        ui->construction_by_element_info->setText(QString::fromUtf8(automaton.error().c_str()));
}

void MainWindow::construct_by_regex()
{
    std::string regex(ui->regex_construction_le->text().toUtf8().constData());
    auto automaton = FiniteAutomaton::construct(regex);
    if (automaton) {
        auto graph = new AutomatonGraph(*automaton);
        ui->main_view->scene()->addItem(graph);
    } else
        ui->construction_by_regex_info->setText(QString::fromUtf8(automaton.error().c_str()));
}

namespace {
void execute_unary_operation(QGraphicsScene *scene, const auto &operation)
{
    for (auto *selected : scene->selectedItems()) {
        auto graph = qgraphicsitem_cast<AutomatonGraph *>(selected);
        if (graph) {
            scene->addItem(new AutomatonGraph((graph->get_automaton().*operation)()));
            scene->removeItem(graph);
        }
    }
}
} // namespace

void MainWindow::setup_operations_dock()
{
    connect(ui->determinize_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(ui->main_view->scene(), &FiniteAutomaton::determinize);
    });

    connect(ui->minimize_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(ui->main_view->scene(), &FiniteAutomaton::minimize);
    });

    connect(ui->complete_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(ui->main_view->scene(), &FiniteAutomaton::complete);
    });

    connect(ui->reverse_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(ui->main_view->scene(), &FiniteAutomaton::reverse);
    });

    connect(ui->complement_btn, &QPushButton::clicked, this, [=]() {
        execute_unary_operation(ui->main_view->scene(), &FiniteAutomaton::complement);
    });
}
