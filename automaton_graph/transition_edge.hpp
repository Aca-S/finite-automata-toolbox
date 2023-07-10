#ifndef TRANSITION_EDGE_HPP
#define TRANSITION_EDGE_HPP

#include "edge.hpp"

// Graphical representation of an automaton transition.
class TransitionEdge : public Edge
{
  public:
    TransitionEdge(const QString &label, const QFont &label_font = QFont("Times-Roman", 12));

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // TRANSITION_EDGE_HPP
