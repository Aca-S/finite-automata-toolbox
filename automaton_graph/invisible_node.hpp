#ifndef INVISIBLE_NODE_HPP
#define INVISIBLE_NODE_HPP

#include "node.hpp"

// Used in representing automaton initial nodes by connecting an invisible
// node to the initial node with a transition edge.
class InvisibleNode : public QtGraph::Node
{
  public:
    InvisibleNode();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // INVISIBLE_NODE_HPP
