#ifndef MATCH_SIMULATOR_HPP
#define MATCH_SIMULATOR_HPP

#include "automaton_graph.hpp"

#include <QString>

#include <set>
#include <vector>

class MatchSimulator
{
  public:
    MatchSimulator(AutomatonGraph *graph, const QString &word);

    void first_step();
    void previous_step();
    void next_step();
    void last_step();
    void clear_active();

  private:
    AutomatonGraph *m_graph;
    std::vector<std::set<unsigned>> m_match_steps;
    size_t m_current_step;
};

#endif // MATCH_SIMULATOR_HPP
