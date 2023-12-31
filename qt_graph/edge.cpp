#include "edge.hpp"

#include <QPainter>

#include "utility.hpp"

using namespace QtGraph;

Edge::Edge(const QString &label, const QFont &label_font) : m_label(label), m_label_font(label_font) {}

QRectF Edge::boundingRect() const { return m_bounding_rectangle; }

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPath(m_path);
    painter->setFont(m_label_font);
    painter->drawText(m_label_position, m_label);
}

void Edge::setup()
{
    Utility::set_gv_attribute(m_gv_edge, "label", m_label);
    Utility::set_gv_attribute(m_gv_edge, "fontname", m_label_font.family());
    Utility::set_gv_attribute(m_gv_edge, "fontsize", QString::number(m_label_font.pointSize()));
}

void Edge::update_positions()
{
    using namespace QtGraph::Utility;

    m_path.clear();

    bezier *curve_data = ED_spl(m_gv_edge)->list;

    m_path.moveTo(gv_to_qt_coords(curve_data->list[0]));
    for (auto i = 1; i < curve_data->size; i += 3)
        m_path.cubicTo(
            gv_to_qt_coords(curve_data->list[i]), gv_to_qt_coords(curve_data->list[i + 1]),
            gv_to_qt_coords(curve_data->list[i + 2]));

    if (curve_data->eflag) {
        QPointF arrow_start = gv_to_qt_coords(curve_data->list[curve_data->size - 1]);
        QPointF arrow_end = gv_to_qt_coords(curve_data->ep);
        m_path.lineTo(arrow_end);

        QLineF normal = QLineF(arrow_start, arrow_end).normalVector();
        QPointF arrowhead_vector = QPointF(normal.dx() / 2.0, normal.dy() / 2.0);

        QPolygonF arrowhead({arrow_start - arrowhead_vector, arrow_end, arrow_start + arrowhead_vector});
        m_path.addPolygon(arrowhead);
    }

    // Long edge labels as rendered by Qt don't correspond perfectly to Graphviz's labels,
    // so some overlap on longer labels is possible.
    if (m_label != "") {
        QPointF center_position = gv_to_qt_coords(ED_label(m_gv_edge)->pos);
        QPointF label_dim = gv_to_qt_coords(ED_label(m_gv_edge)->dimen);
        m_label_position = {center_position.x() - label_dim.x() / 2.0, center_position.y()};
    }

    QRectF label_br =
        QFontMetrics(m_label_font).boundingRect(m_label).translated(m_label_position.x(), m_label_position.y());
    m_bounding_rectangle = m_path.boundingRect() | label_br;
}
