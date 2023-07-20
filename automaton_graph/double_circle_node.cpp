#include "double_circle_node.hpp"

#include <QPainter>

DoubleCircleNode::DoubleCircleNode(qreal outer_size, qreal inner_size, const QString &label, const QFont &label_font)
    : AutomatonNode(outer_size, label, label_font), m_inner_size(inner_size)
{
}

QRectF DoubleCircleNode::boundingRect() const { return Node::boundingRect(); }

void DoubleCircleNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    AutomatonNode::paint(painter, option, widget);
    painter->drawEllipse(boundingRect().center(), m_inner_size / 2.0, m_inner_size / 2.0);
}
