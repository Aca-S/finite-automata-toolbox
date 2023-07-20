#ifndef DOUBLE_CIRCLE_NODE_HPP
#define DOUBLE_CIRCLE_NODE_HPP

#include "automaton_node.hpp"

// Graphical representation of final state automaton nodes.
class DoubleCircleNode : public AutomatonNode
{
  public:
    DoubleCircleNode(
        qreal outer_size, qreal inner_size, const QString &label, const QFont &label_font = QFont("Times-Roman", 12));

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

  private:
    qreal m_inner_size;
};

#endif // DOUBLE_CIRCLE_NODE_HPP
