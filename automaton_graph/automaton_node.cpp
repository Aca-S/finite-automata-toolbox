#include "automaton_node.hpp"

#include <QPainter>

AutomatonNode::AutomatonNode(qreal size, const QString &label, const QFont &label_font)
    : QtGraph::Node(size, size, label, label_font)
{
}

QRectF AutomatonNode::boundingRect() const { return QtGraph::Node::boundingRect(); }

void AutomatonNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (m_is_active) {
        painter->setBrush(QColor(0, 255, 255, 120));
        painter->drawEllipse(boundingRect());
        painter->setBrush(Qt::NoBrush);
    }
    Node::paint(painter, option, widget);
}

void AutomatonNode::activate()
{
    m_is_active = true;
    update();
}

void AutomatonNode::deactivate()
{
    m_is_active = false;
    update();
}
