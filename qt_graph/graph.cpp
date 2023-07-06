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
    QPointF lower_left = gv_to_qt_coords({GD_bb(m_gv_graph).LL.x, GD_bb(m_gv_graph).LL.y});
    QPointF upper_right = gv_to_qt_coords({GD_bb(m_gv_graph).UR.x, GD_bb(m_gv_graph).UR.y});

    QPointF upper_left = {lower_left.x(), upper_right.y()};
    QPointF lower_right = {upper_right.x(), lower_left.y()};

    return QRectF(upper_left, lower_right);
}

void Graph::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawRect(boundingRect());
}

void Graph::update_layout() { gvLayout(m_context.m_gv_context, m_gv_graph, "dot"); }

QPointF Graph::gv_to_qt_coords(const QPointF &gv_point)
{
    static const qreal dpi_adjustment = 92.0 / 72.0;
    return {dpi_adjustment * gv_point.x(), -dpi_adjustment * gv_point.y()};
}

qreal Graph::gv_to_qt_size(const qreal &gv_size) { return 92.0 * gv_size; }
