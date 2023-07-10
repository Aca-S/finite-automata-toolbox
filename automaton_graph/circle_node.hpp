#ifndef CIRCLE_NODE_HPP
#define CIRCLE_NODE_HPP

#include "node.hpp"

// Graphical representation of non-final state automaton nodes.
class CircleNode : public Node
{
  public:
    CircleNode(qreal size, const QString &label, const QFont &label_font = QFont("Times-Roman", 12));

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // CIRCLE_NODE_HPP
