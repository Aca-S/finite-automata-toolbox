#include "node.hpp"

#include <QPainter>
#include <iostream>

#include "utility.hpp"

Node::Node(qreal width, qreal height, const QString &label, const QFont &label_font)
    : m_width(width), m_height(height), m_label(label), m_label_font(label_font)
{
}

QRectF Node::boundingRect() const
{
    QRectF label_br =
        QFontMetrics(m_label_font).boundingRect(m_label).translated(m_label_position.x(), m_label_position.y());
    return m_path.boundingRect() | label_br;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPath(m_path);
    painter->setFont(m_label_font);
    painter->drawText(m_label_position, m_label);
}

void Node::setup()
{
    Utility::set_gv_attribute(m_gv_node, "fixedsize", "true");
    Utility::set_gv_attribute(m_gv_node, "width", QString::number(m_width / 92.0));
    Utility::set_gv_attribute(m_gv_node, "height", QString::number(m_height / 92.0));
    Utility::set_gv_attribute(m_gv_node, "label", m_label);
    Utility::set_gv_attribute(m_gv_node, "fontname", m_label_font.family());
    Utility::set_gv_attribute(m_gv_node, "fontsize", QString::number(m_label_font.pointSize()));
}

void Node::update_positions()
{
    using namespace Utility;

    m_path.clear();

    qreal width = gv_to_qt_size(ND_width(m_gv_node));
    qreal height = gv_to_qt_size(ND_height(m_gv_node));
    QPointF center = gv_to_qt_coords(ND_coord(m_gv_node));

    QPointF top_left = {center.x() - width / 2.0, center.y() - height / 2.0};
    QPointF bottom_right = {top_left.x() + width, top_left.y() + height};

    m_path.addEllipse(QRectF(top_left, bottom_right));

    if (m_label != "")
        m_label_position = m_path.boundingRect().center() - gv_to_qt_coords(ND_label(m_gv_node)->dimen) / 2.0;
}
