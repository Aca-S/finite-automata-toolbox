#include "main_window.hpp"
#include "./ui_main_window.h"

#include "automaton_graph.hpp"
#include "finite_automaton.hpp"

#include <QLabel>
#include <QRegularExpression>
#include <QValidator>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setup_validators();

    connect(ui->transition_btn, &QPushButton::clicked, ui->transition_list, [=]() {
        if (ui->transition_le->hasAcceptableInput())
            ui->transition_list->addItem(ui->transition_le->text());
    });

    /*QGraphicsScene *scene = new QGraphicsScene(this);
    ui->main_view->setScene(scene);
    AutomatonGraph *g = new AutomatonGraph(*FiniteAutomaton::construct("(a|b)*"));
    scene->addItem(g);*/
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
        QObject::connect(qApp, &QApplication::focusChanged, info_label, [=](auto *old) {
            if (old == line_edit && !line_edit->hasAcceptableInput())
                info_label->setText(info_message);
            else if (old == line_edit)
                info_label->setText(""); // Last input was rejected, but without it, the text would be valid.
        });
    }
}
} // namespace

void MainWindow::setup_validators()
{
    QString unsigned_regex = "(0|[1-9]\\d*)";
    QRegularExpression sym_list_regex("^(\\w( \\w)*)?$");
    QRegularExpression num_list_regex("^(" + unsigned_regex + "( " + unsigned_regex + ")*)?$");
    QRegularExpression transition_regex("^" + unsigned_regex + " \\w " + unsigned_regex + "$");

    set_validator(
        ui->alphabet_le, new QRegularExpressionValidator(sym_list_regex, this), ui->construction_by_element_info,
        "The alphabet input must be a space separated list of ASCII characters.");
    set_validator(
        ui->states_le, new QRegularExpressionValidator(num_list_regex, this), ui->construction_by_element_info,
        "The states input must be a space separated list of numbers.");
    set_validator(
        ui->initial_states_le, new QRegularExpressionValidator(num_list_regex, this), ui->construction_by_element_info,
        "The initial states input must be a space separated list of numbers.");
    set_validator(
        ui->final_states_le, new QRegularExpressionValidator(num_list_regex, this), ui->construction_by_element_info,
        "The final states input must be a space separated list of numbers.");
    set_validator(
        ui->transition_le, new QRegularExpressionValidator(transition_regex, this), ui->construction_by_element_info,
        "The transition input must be in the form of <state symbol state>.");
}
