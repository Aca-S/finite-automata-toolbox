#ifndef MATCH_SIMULATOR_HPP
#define MATCH_SIMULATOR_HPP

#include "automaton_graph.hpp"

#include <QLineEdit>
#include <QObject>
#include <QString>

#include <set>
#include <vector>

class MatchSimulator : public QObject
{
    Q_OBJECT

  public:
    MatchSimulator(AutomatonGraph *graph, QLineEdit *word_le);
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
    QLineEdit *m_word_le;

    std::vector<std::set<unsigned>> m_match_steps;
    size_t m_current_step = 0;
    int m_word_position = 0;
};

#endif // MATCH_SIMULATOR_HPP
