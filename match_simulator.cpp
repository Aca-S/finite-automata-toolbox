#include "match_simulator.hpp"

MatchSimulator::MatchSimulator(AutomatonGraph *graph, const QString &word)
    : m_graph(graph),
      m_match_steps(m_graph->get_automaton().generate_match_steps(std::string(word.toUtf8().constData()))),
      m_current_step(0)
{
    for (const auto &state : m_match_steps[0])
        m_graph->m_node_map[state]->activate();
}

MatchSimulator::~MatchSimulator() { clear_active(); }

void MatchSimulator::first_step()
{
    clear_active();
    for (const auto &state : m_match_steps[0])
        m_graph->m_node_map[state]->activate();
}

void MatchSimulator::previous_step()
{
    if (m_current_step > 0) {
        clear_active();
        for (const auto &state : m_match_steps[--m_current_step])
            m_graph->m_node_map[state]->activate();
    }
}

void MatchSimulator::next_step()
{
    if (m_current_step < m_match_steps.size() - 1) {
        clear_active();
        for (const auto &state : m_match_steps[++m_current_step])
            m_graph->m_node_map[state]->activate();
    }
}

void MatchSimulator::last_step()
{
    clear_active();
    for (const auto &state : m_match_steps[m_match_steps.size() - 1])
        m_graph->m_node_map[state]->activate();
}

void MatchSimulator::clear_active()
{
    for (const auto &state : m_match_steps[m_current_step])
        m_graph->m_node_map[state]->deactivate();
}
