#include "node.hpp"

#include <QPainter>

#include "utility.hpp"

Node::Node(const QString &label) : m_label(label) {}

QRectF Node::boundingRect() const
{
    qreal width = Utility::gv_to_qt_size(ND_width(m_gv_node));
    qreal height = Utility::gv_to_qt_size(ND_height(m_gv_node));
    QPointF center = Utility::gv_to_qt_coords({ND_coord(m_gv_node).x, ND_coord(m_gv_node).y});

    QPointF top_left = {center.x() - width / 2.0, center.y() - height / 2.0};
    QPointF bottom_right = {top_left.x() + width, top_left.y() + height};

    return QRectF(top_left, bottom_right);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawRect(boundingRect());
}

void Node::setup() { Utility::set_gv_attribute(m_gv_node, "label", m_label); }
