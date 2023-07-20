#include "match_simulator.hpp"

MatchSimulator::MatchSimulator(AutomatonGraph *graph, const QString &word)
    : m_graph(graph),
      m_match_steps(m_graph->get_automaton().generate_match_steps(std::string(word.toUtf8().constData()))),
      m_current_step(0)
{
}

MatchSimulator::~MatchSimulator() {}

void MatchSimulator::first_step() {}

void MatchSimulator::previous_step() {}

void MatchSimulator::next_step() {}

void MatchSimulator::last_step() {}

void MatchSimulator::clear_active() {}
