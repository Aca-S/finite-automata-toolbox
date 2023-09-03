#include "creation_dock.hpp"

#include <QApplication>
#include <QGroupBox>
#include <QLayout>
#include <QPushButton>

#include <ranges>
#include <string_view>

#include "automaton_graph.hpp"
#include "finite_automaton.hpp"

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

CreationDock::CreationDock(QGraphicsView *main_view, QWidget *parent) : QDockWidget(parent), m_main_view(main_view)
{
    this->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    auto dock_container = new QWidget;
    this->setWidget(dock_container);
    auto main_layout = new QVBoxLayout(dock_container);

    build_element_group();
    setup_element_group();

    build_regex_group();
    setup_regex_group();

    main_layout->addStretch();
}

const QString symbol_regex = "[!-~]"; // Printable ASCII characters without space.
const QString unsigned_regex = "(0|[1-9]\\d*)";

void CreationDock::build_element_group()
{
    m_alphabet_le = new QLineEdit;
    m_alphabet_le->setPlaceholderText("Alphabet");
    m_states_le = new QLineEdit;
    m_states_le->setPlaceholderText("States");
    m_initial_states_le = new QLineEdit;
    m_initial_states_le->setPlaceholderText("Initial states");
    m_final_states_le = new QLineEdit;
    m_final_states_le->setPlaceholderText("Final states");
    m_transition_le = new QLineEdit;
    m_transition_le->setPlaceholderText("Transition");

    m_add_transition_btn = new QPushButton("Add");
    m_remove_transition_btn = new QPushButton("Remove");

    m_transition_list = new QListWidget;
    m_transition_list->setSelectionMode(QAbstractItemView::MultiSelection);

    m_element_construct_btn = new QPushButton("Construct");
    m_element_construct_info = new QLabel("");
    m_element_construct_info->setWordWrap(true);

    auto element_group = new QGroupBox("By element");
    auto element_layout = new QGridLayout;
    element_layout->addWidget(m_alphabet_le, 0, 0, 1, 1);
    element_layout->addWidget(m_states_le, 1, 0, 1, 1);
    element_layout->addWidget(m_initial_states_le, 2, 0, 1, 1);
    element_layout->addWidget(m_final_states_le, 3, 0, 1, 1);
    element_layout->addWidget(m_transition_list, 0, 1, 3, 3);
    element_layout->addWidget(m_transition_le, 3, 1, 1, 1);
    element_layout->addWidget(m_add_transition_btn, 3, 2, 1, 1);
    element_layout->addWidget(m_remove_transition_btn, 3, 3, 1, 1);
    element_layout->addWidget(m_element_construct_btn, 4, 0, 1, 4);
    element_layout->addWidget(m_element_construct_info, 5, 0, 1, 4);
    element_group->setLayout(element_layout);
    this->widget()->layout()->addWidget(element_group);
}

void CreationDock::setup_element_group()
{
    QRegularExpression sym_list_regex("^(" + symbol_regex + "( " + symbol_regex + ")*)?$");
    QRegularExpression num_list_not_empty_regex("^(" + unsigned_regex + "( " + unsigned_regex + ")*)$");
    QRegularExpression num_list_regex("^(" + unsigned_regex + "( " + unsigned_regex + ")*)?$");
    QRegularExpression transition_regex("^" + unsigned_regex + " " + symbol_regex + " " + unsigned_regex + "$");

    auto alphabet_validator = new QRegularExpressionValidator(sym_list_regex, this);
    auto states_not_empty_validator = new QRegularExpressionValidator(num_list_not_empty_regex, this);
    auto states_validator = new QRegularExpressionValidator(num_list_regex, this);
    auto transition_validator = new QRegularExpressionValidator(transition_regex, this);

    set_validator(
        m_alphabet_le, alphabet_validator, m_element_construct_info,
        "The alphabet input must be a space separated list of printable ASCII characters.");
    set_validator(
        m_states_le, states_not_empty_validator, m_element_construct_info,
        "The states input must be a non-empty space separated list of numbers.");
    set_validator(
        m_initial_states_le, states_validator, m_element_construct_info,
        "The initial states input must be a space separated list of numbers.");
    set_validator(
        m_final_states_le, states_validator, m_element_construct_info,
        "The final states input must be a space separated list of numbers.");
    set_validator(
        m_transition_le, transition_validator, m_element_construct_info,
        "The transition input must be in the form of <state symbol state>.");

    connect(m_add_transition_btn, &QPushButton::clicked, m_transition_list, [=]() {
        if (m_transition_le->hasAcceptableInput()) {
            m_transition_list->addItem(m_transition_le->text());
            m_transition_le->clear();
        }
        m_transition_le->setFocus();
    });

    connect(m_remove_transition_btn, &QPushButton::clicked, m_transition_list, [=]() {
        for (auto *transition : m_transition_list->selectedItems())
            delete m_transition_list->takeItem(m_transition_list->row(transition));
        m_transition_le->setFocus();
    });

    connect(m_element_construct_btn, &QPushButton::clicked, this, [=]() { construct_by_element(); });
}

void CreationDock::construct_by_element()
{
    for (QLineEdit *le : {m_alphabet_le, m_states_le, m_initial_states_le, m_final_states_le}) {
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
    input_to_container(m_alphabet_le->text(), alphabet, [](const auto &s) { return s[0]; });

    std::set<unsigned> states;
    input_to_container(m_states_le->text(), states, [](const auto &s) { return std::stoul(s); });

    std::set<unsigned> initial_states;
    input_to_container(m_initial_states_le->text(), initial_states, [](const auto &s) { return std::stoul(s); });

    std::set<unsigned> final_states;
    input_to_container(m_final_states_le->text(), final_states, [](const auto &s) { return std::stoul(s); });

    std::map<std::pair<unsigned, char>, std::set<unsigned>> transition_function;
    for (auto i = 0; i < m_transition_list->count(); ++i) {
        QString transition = m_transition_list->item(i)->text();
        auto spl = transition.split(" ");
        unsigned from_state = std::stoul(std::string(spl[0].toUtf8().constData()));
        char transition_symbol = std::string(spl[1].toUtf8().constData())[0];
        unsigned to_state = std::stoul(std::string(spl[2].toUtf8().constData()));
        transition_function[{from_state, transition_symbol}].insert(to_state);
    }

    auto automaton = FiniteAutomaton::construct(alphabet, states, initial_states, final_states, transition_function);
    if (automaton) {
        auto graph = new AutomatonGraph(*automaton);
        add_item_at_pos(graph, m_main_view->scene(), get_viewport_center_pos(m_main_view));
    } else
        m_element_construct_info->setText(QString::fromUtf8(automaton.error().c_str()));
}

void CreationDock::build_regex_group()
{
    m_regex_le = new QLineEdit;
    m_regex_le->setPlaceholderText("RegEx");
    m_regex_construct_btn = new QPushButton("Construct");
    m_regex_construct_info = new QLabel("");
    m_regex_construct_info->setWordWrap(true);

    auto regex_group = new QGroupBox("By RegEx");
    auto regex_layout = new QGridLayout;
    regex_layout->addWidget(m_regex_le, 0, 0, 1, 1);
    regex_layout->addWidget(m_regex_construct_btn, 0, 1, 1, 1);
    regex_layout->addWidget(m_regex_construct_info, 1, 0, 1, 2);
    regex_group->setLayout(regex_layout);
    this->widget()->layout()->addWidget(regex_group);
}

void CreationDock::setup_regex_group()
{
    QRegularExpression symbols_only_regex("^" + symbol_regex + "*$");
    auto regex_validator = new QRegularExpressionValidator(symbols_only_regex, this);
    set_validator(
        m_regex_le, regex_validator, m_regex_construct_info, "Only printable ASCII symbols are allowed in the RegEx.");

    connect(m_regex_construct_btn, &QPushButton::clicked, this, [=]() { construct_by_regex(); });
}

void CreationDock::construct_by_regex()
{
    std::string regex(m_regex_le->text().toUtf8().constData());
    auto automaton = FiniteAutomaton::construct(regex);
    if (automaton) {
        auto graph = new AutomatonGraph(*automaton);
        add_item_at_pos(graph, m_main_view->scene(), get_viewport_center_pos(m_main_view));
    } else
        m_regex_construct_info->setText(QString::fromUtf8(automaton.error().c_str()));
}
