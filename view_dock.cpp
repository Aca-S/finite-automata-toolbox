#include "view_dock.hpp"

#include <QApplication>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QWheelEvent>

#include <optional>
#include <ranges>

#include "automaton_graph.hpp"
#include "finite_automaton.hpp"
#include "match_simulator.hpp"

using namespace Ui;

namespace {
// TODO: Extract all of these to util namespace.

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

QFrame *create_h_line()
{
    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    return line;
}
} // namespace

ViewDock::ViewDock(QGraphicsView *main_view, QWidget *parent) : QDockWidget(parent), m_main_view(main_view)
{
    this->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    auto dock_container = new QWidget;
    this->setWidget(dock_container);
    auto main_layout = new QVBoxLayout(dock_container);

    build_view_section();
    main_layout->addWidget(create_h_line());
    build_match_section();
    main_layout->addWidget(create_h_line());
    build_regex_section();
    m_view_info = new QLabel("");
    m_view_info->setWordWrap(true);
    main_layout->addWidget(m_view_info);

    setup_view_section();
    setup_match_section();
    setup_regex_section();
}

void ViewDock::build_view_section()
{
    m_side_view = new SideGraphicsView;
    m_view_btn = new QPushButton("View selected");

    auto view_section = new QWidget;
    auto view_layout = new QVBoxLayout(view_section);
    view_layout->setContentsMargins(0, 0, 0, 0);
    view_layout->addWidget(m_side_view);
    view_layout->addWidget(m_view_btn);
    this->widget()->layout()->addWidget(view_section);
}

void ViewDock::setup_view_section()
{
    m_side_view->setScene(new QGraphicsScene(this));

    connect(m_view_btn, &QPushButton::clicked, this, [=]() {
        m_side_view->scene()->clear();
        m_regex_le->clear();
        m_view_info->setText("");

        auto graphs = get_selected<AutomatonGraph>(m_main_view->scene());

        if (graphs.size() > 0) {
            auto *new_graph = new AutomatonGraph(graphs.at(0)->get_automaton());
            new_graph->setFlag(QGraphicsItem::ItemIsSelectable, false);
            new_graph->setFlag(QGraphicsItem::ItemIsMovable, false);
            add_item_at_pos(new_graph, m_side_view->scene(), get_viewport_center_pos(m_side_view));
        } else
            m_view_info->setText("An automaton must be selected from the main view.");
    });
}

void ViewDock::build_match_section()
{
    m_acceptable_word_btn = new QPushButton("Generate acceptable");
    m_unacceptable_word_btn = new QPushButton("Generate unacceptable");
    m_word_le = new QLineEdit;
    m_word_le->setPlaceholderText("Word");
    m_match_reset_btn = new QPushButton("<<");
    m_match_back_btn = new QPushButton("<");
    m_match_next_btn = new QPushButton(">");
    m_match_end_btn = new QPushButton(">>");

    auto match_section = new QWidget;
    auto match_layout = new QGridLayout(match_section);
    match_layout->setContentsMargins(0, 0, 0, 0);
    match_layout->addWidget(m_acceptable_word_btn, 0, 0, 1, 2);
    match_layout->addWidget(m_unacceptable_word_btn, 0, 2, 1, 2);
    match_layout->addWidget(m_word_le, 1, 0, 1, 4);
    match_layout->addWidget(m_match_reset_btn, 2, 0, 1, 1);
    match_layout->addWidget(m_match_back_btn, 2, 1, 1, 1);
    match_layout->addWidget(m_match_next_btn, 2, 2, 1, 1);
    match_layout->addWidget(m_match_end_btn, 2, 3, 1, 1);
    this->widget()->layout()->addWidget(match_section);
}

void ViewDock::setup_match_section()
{
    QString symbol_regex = "[!-~]";
    QRegularExpression symbols_only_regex("^" + symbol_regex + "*$");
    auto word_validator = new QRegularExpressionValidator(symbols_only_regex, this);
    set_validator(
        m_word_le, word_validator, m_view_info, "Only printable ASCII symbols are allowed in the testing word.");

    connect(m_acceptable_word_btn, &QPushButton::clicked, this, [=]() {
        execute_generator_operation(
            m_side_view, m_word_le, m_view_info, "No acceptable words exist for the selected automaton.",
            &FiniteAutomaton::generate_valid_word);
    });

    connect(m_unacceptable_word_btn, &QPushButton::clicked, this, [=]() {
        execute_generator_operation(
            m_side_view, m_word_le, m_view_info,
            "No unacceptable words, under its alphabet, exist for the selected automaton.",
            &FiniteAutomaton::generate_invalid_word);
    });

    static std::optional<MatchSimulator> match_simulator;

    connect(m_view_btn, &QPushButton::clicked, this, [&]() { match_simulator.reset(); });

    connect(m_word_le, &QLineEdit::textChanged, this, [&]() { match_simulator.reset(); });

    connect(m_match_reset_btn, &QPushButton::clicked, this, [&]() {
        execute_matcher_operation(
            m_side_view, m_word_le, m_view_info, match_simulator, &MatchSimulator::first_step, false);
    });

    connect(m_match_back_btn, &QPushButton::clicked, this, [&]() {
        execute_matcher_operation(
            m_side_view, m_word_le, m_view_info, match_simulator, &MatchSimulator::previous_step, false);
    });

    connect(m_match_next_btn, &QPushButton::clicked, this, [&]() {
        execute_matcher_operation(
            m_side_view, m_word_le, m_view_info, match_simulator, &MatchSimulator::next_step, false);
    });

    connect(m_match_end_btn, &QPushButton::clicked, this, [&]() {
        execute_matcher_operation(
            m_side_view, m_word_le, m_view_info, match_simulator, &MatchSimulator::last_step, true);
    });
}

void ViewDock::build_regex_section()
{
    m_regex_btn = new QPushButton("Generate RegEx");
    m_regex_le = new QLineEdit;
    m_regex_le->setPlaceholderText("RegEx");
    m_regex_le->setReadOnly(true);

    auto regex_section = new QWidget;
    auto regex_layout = new QHBoxLayout(regex_section);
    regex_layout->setContentsMargins(0, 0, 0, 0);
    regex_layout->addWidget(m_regex_btn);
    regex_layout->addWidget(m_regex_le);
    this->widget()->layout()->addWidget(regex_section);
}

void ViewDock::setup_regex_section()
{
    connect(m_regex_btn, &QPushButton::clicked, this, [=]() {
        execute_generator_operation(
            m_side_view, m_regex_le, m_view_info, "No regular expression matches selected automaton",
            &FiniteAutomaton::generate_regex);
    });
}

ViewDock::SideGraphicsView::SideGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSceneRect(-16000, -16000, 32000, 32000);
    centerOn({0.0, 0.0});
}

void ViewDock::SideGraphicsView::wheelEvent(QWheelEvent *event)
{
    qreal factor_of_change = 1.1;
    qreal max_scale_factor = 1.5;
    qreal min_scale_factor = 0.35;

    if (event->angleDelta().y() > 0) {
        qreal new_scale_factor = m_current_scale_factor * factor_of_change;
        if (new_scale_factor < max_scale_factor) {
            scale(factor_of_change, factor_of_change);
            m_current_scale_factor = new_scale_factor;
        }
    } else if (event->angleDelta().y() < 0) {
        qreal new_scale_factor = m_current_scale_factor / factor_of_change;
        if (new_scale_factor > min_scale_factor) {
            scale(1 / factor_of_change, 1 / factor_of_change);
            m_current_scale_factor = new_scale_factor;
        }
    }
}
