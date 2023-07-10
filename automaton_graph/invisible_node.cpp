#include "invisible_node.hpp"

InvisibleNode::InvisibleNode() : Node(0, 0, "") {}

QRectF InvisibleNode::boundingRect() const { return Node::boundingRect(); }

void InvisibleNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {}
