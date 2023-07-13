#ifndef AUTOMATON_GRAPH_HPP
#define AUTOMATON_GRAPH_HPP

#include "finite_automaton.hpp"
#include "graph.hpp"

// Graphical representation of a finite automaton.
class AutomatonGraph : public Graph
{
  public:
    AutomatonGraph(const FiniteAutomaton &automaton);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    const FiniteAutomaton &get_automaton() const;

  private:
    FiniteAutomaton m_automaton;
};

#endif // AUTOMATON_GRAPH_HPP
