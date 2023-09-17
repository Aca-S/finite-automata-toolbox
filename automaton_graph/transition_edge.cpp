#include "transition_edge.hpp"

#include <QPainter>

TransitionEdge::TransitionEdge(const QString &label, const QFont &label_font) : QtGraph::Edge(label, label_font) {}

QRectF TransitionEdge::boundingRect() const { return QtGraph::Edge::boundingRect(); }

void TransitionEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    Edge::paint(painter, option, widget);
}
