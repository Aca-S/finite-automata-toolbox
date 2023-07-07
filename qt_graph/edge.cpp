#include "edge.hpp"

Edge::Edge(const QString &label, bool has_source_arrow, bool has_destination_arrow)
    : m_label(label), m_has_source_arrow(has_source_arrow), m_has_destination_arrow(has_destination_arrow)
{
}

QRectF Edge::boundingRect() const { return QRectF(); }

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {}

void Edge::setup() { agset(m_gv_edge, const_cast<char *>("label"), const_cast<char *>(m_label.toUtf8().constData())); }
