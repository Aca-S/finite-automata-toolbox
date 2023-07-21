#include "match_simulator.hpp"

MatchSimulator::MatchSimulator(AutomatonGraph *graph, const QString &word)
    : m_graph(graph),
      m_match_steps(m_graph->get_automaton().generate_match_steps(std::string(word.toUtf8().constData()))),
      m_current_step(0)
{
    connect(this, &MatchSimulator::activate_state_nodes, m_graph, &AutomatonGraph::activate_state_nodes);
    connect(this, &MatchSimulator::deactivate_state_nodes, m_graph, &AutomatonGraph::deactivate_state_nodes);
    emit activate_state_nodes(m_match_steps[m_current_step]);
}

MatchSimulator::~MatchSimulator() { emit deactivate_state_nodes(m_match_steps[m_current_step]); }

void MatchSimulator::first_step()
{
    emit deactivate_state_nodes(m_match_steps[m_current_step]);
    m_current_step = 0;
    emit activate_state_nodes(m_match_steps[m_current_step]);
}

void MatchSimulator::previous_step()
{
    if (m_current_step > 0) {
        emit deactivate_state_nodes(m_match_steps[m_current_step]);
        emit activate_state_nodes(m_match_steps[--m_current_step]);
    }
}

void MatchSimulator::next_step()
{
    if (m_current_step < m_match_steps.size() - 1) {
        emit deactivate_state_nodes(m_match_steps[m_current_step]);
        emit activate_state_nodes(m_match_steps[++m_current_step]);
    }
}

void MatchSimulator::last_step()
{
    emit deactivate_state_nodes(m_match_steps[m_current_step]);
    m_current_step = m_match_steps.size() - 1;
    emit activate_state_nodes(m_match_steps[m_current_step]);
}
