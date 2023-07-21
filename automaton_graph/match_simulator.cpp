#include "match_simulator.hpp"

#include <QApplication>
#include <QInputMethodEvent>

namespace {
void mark_line_edit(QLineEdit *le, int position, int length)
{
    QTextCharFormat fmt;
    fmt.setBackground(Qt::darkGreen);
    fmt.setForeground(Qt::white);

    position -= le->cursorPosition();
    QInputMethodEvent decorate_event(
        "", {QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat, position, length, fmt)});
    QApplication::sendEvent(le, &decorate_event);
}

void unmark_line_edit(QLineEdit *le) { mark_line_edit(le, 0, 0); }
} // namespace

MatchSimulator::MatchSimulator(AutomatonGraph *graph, QLineEdit *word_le)
    : m_graph(graph), m_word_le(word_le),
      m_match_steps(m_graph->get_automaton().generate_match_steps(std::string(m_word_le->text().toUtf8().constData())))
{
    connect(this, &MatchSimulator::activate_state_nodes, m_graph, &AutomatonGraph::activate_state_nodes);
    connect(this, &MatchSimulator::deactivate_state_nodes, m_graph, &AutomatonGraph::deactivate_state_nodes);
    emit activate_state_nodes(m_match_steps[m_current_step]);
    mark_line_edit(m_word_le, m_word_position, 1);
}

MatchSimulator::~MatchSimulator()
{
    emit deactivate_state_nodes(m_match_steps[m_current_step]);
    unmark_line_edit(m_word_le);
}

void MatchSimulator::first_step()
{
    emit deactivate_state_nodes(m_match_steps[m_current_step]);
    m_current_step = 0;
    emit activate_state_nodes(m_match_steps[m_current_step]);
    m_word_position = 0;
    mark_line_edit(m_word_le, m_word_position, 1);
}

void MatchSimulator::previous_step()
{
    if (m_current_step > 0) {
        emit deactivate_state_nodes(m_match_steps[m_current_step]);
        emit activate_state_nodes(m_match_steps[--m_current_step]);
    }
    if (m_word_position > 0)
        mark_line_edit(m_word_le, --m_word_position, 1);
}

void MatchSimulator::next_step()
{
    if (m_current_step < m_match_steps.size() - 1) {
        emit deactivate_state_nodes(m_match_steps[m_current_step]);
        emit activate_state_nodes(m_match_steps[++m_current_step]);
    }
    if (m_word_position < m_word_le->text().length())
        mark_line_edit(m_word_le, ++m_word_position, 1);
}

void MatchSimulator::last_step()
{
    emit deactivate_state_nodes(m_match_steps[m_current_step]);
    m_current_step = m_match_steps.size() - 1;
    emit activate_state_nodes(m_match_steps[m_current_step]);
    m_word_position = m_word_le->text().length();
    mark_line_edit(m_word_le, m_word_position, 1);
}
