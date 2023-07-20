#include "circle_node.hpp"

#include <QPainter>

CircleNode::CircleNode(qreal size, const QString &label, const QFont &label_font)
    : AutomatonNode(size, label, label_font)
{
}

QRectF CircleNode::boundingRect() const { return Node::boundingRect(); }

void CircleNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    AutomatonNode::paint(painter, option, widget);
}
