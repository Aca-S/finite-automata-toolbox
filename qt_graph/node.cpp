#include "node.hpp"

#include <QPainter>

Node::Node(Graph *parent_graph, const QString &label)
    : QGraphicsItem(parent_graph),
      m_gv_node(agnode(
          parent_graph->m_gv_graph, const_cast<char *>(std::to_string(parent_graph->m_nodes.size()).c_str()), 1)),
      m_label(label)
{
    agset(m_gv_node, const_cast<char *>("label"), const_cast<char *>(m_label.toUtf8().constData()));
    parent_graph->m_nodes.append(this);
    parent_graph->update_layout();
}

QRectF Node::boundingRect() const
{
    qreal width = Graph::gv_to_qt_size(ND_width(m_gv_node));
    qreal height = Graph::gv_to_qt_size(ND_height(m_gv_node));
    QPointF center = Graph::gv_to_qt_coords({ND_coord(m_gv_node).x, ND_coord(m_gv_node).y});

    QPointF top_left = {center.x() - width / 2.0, center.y() - height / 2.0};
    QPointF bottom_right = {top_left.x() + width, top_left.y() + height};

    return QRectF(top_left, bottom_right);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawRect(boundingRect());
}
