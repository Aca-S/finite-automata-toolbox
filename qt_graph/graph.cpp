#include "graph.hpp"

#include <QPainter>

Graph::Context::Context() : m_gv_context(gvContext()) {}

Graph::Context::~Context() { gvFreeContext(m_gv_context); }

Graph::Context Graph::m_context;

Graph::Graph() : QGraphicsItem(), m_gv_graph(agopen(const_cast<char *>(""), Agdirected, 0)) { update_layout(); }

Graph::~Graph()
{
    gvFreeLayout(m_context.m_gv_context, m_gv_graph);
    agclose(m_gv_graph);
}

QRectF Graph::boundingRect() const
{
    QPointF bottom_left = gv_to_qt_coords({GD_bb(m_gv_graph).LL.x, GD_bb(m_gv_graph).LL.y});
    QPointF top_right = gv_to_qt_coords({GD_bb(m_gv_graph).UR.x, GD_bb(m_gv_graph).UR.y});

    QPointF top_left = {bottom_left.x(), top_right.y()};
    QPointF bottom_right = {top_right.x(), bottom_left.y()};

    return QRectF(top_left, bottom_right);
}

void Graph::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawRect(boundingRect());
}

void Graph::add_node(Node *node)
{
    node->setParentItem(this);
    node->m_gv_node = agnode(m_gv_graph, const_cast<char *>(std::to_string(m_nodes.size()).c_str()), 1);
    node->setup();
    m_nodes.append(node);
    update_layout();
}

void Graph::update_layout() { gvLayout(m_context.m_gv_context, m_gv_graph, "dot"); }

QPointF Graph::gv_to_qt_coords(const QPointF &gv_point)
{
    static const qreal dpi_adjustment = 92.0 / 72.0;
    return {dpi_adjustment * gv_point.x(), -dpi_adjustment * gv_point.y()};
}

qreal Graph::gv_to_qt_size(const qreal &gv_size) { return 92.0 * gv_size; }
