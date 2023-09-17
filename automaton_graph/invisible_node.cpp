#include "invisible_node.hpp"

InvisibleNode::InvisibleNode() : QtGraph::Node(0, 0, "") {}

QRectF InvisibleNode::boundingRect() const { return QtGraph::Node::boundingRect(); }

void InvisibleNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {}
