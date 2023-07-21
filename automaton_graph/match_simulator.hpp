#ifndef MATCH_SIMULATOR_HPP
#define MATCH_SIMULATOR_HPP

#include "automaton_graph.hpp"

#include <QObject>
#include <QString>

#include <set>
#include <vector>

class MatchSimulator : public QObject
{
    Q_OBJECT

  public:
    MatchSimulator(AutomatonGraph *graph, const QString &word);
    ~MatchSimulator();

    void first_step();
    void previous_step();
    void next_step();
    void last_step();

  signals:
    void activate_state_nodes(const std::set<unsigned> &states);
    void deactivate_state_nodes(const std::set<unsigned> &states);

  private:
    AutomatonGraph *m_graph;
    std::vector<std::set<unsigned>> m_match_steps;
    size_t m_current_step;
};

#endif // MATCH_SIMULATOR_HPP
