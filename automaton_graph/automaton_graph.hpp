#ifndef AUTOMATON_GRAPH_HPP
#define AUTOMATON_GRAPH_HPP

#include "automaton_node.hpp"
#include "finite_automaton.hpp"
#include "graph.hpp"

// Graphical representation of a finite automaton.
class AutomatonGraph : public Graph
{
    friend class MatchSimulator;

  public:
    AutomatonGraph(const FiniteAutomaton &automaton);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    enum
    {
        Type = UserType + 1
    };
    int type() const override;

    const FiniteAutomaton &get_automaton() const;

  private:
    FiniteAutomaton m_automaton;
    QMap<unsigned, AutomatonNode *> m_node_map;
};

#endif // AUTOMATON_GRAPH_HPP
