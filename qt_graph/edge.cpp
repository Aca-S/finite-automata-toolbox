#include "edge.hpp"

#include <QPainter>

#include "utility.hpp"

Edge::Edge(const QString &label) : m_label(label) {}

QRectF Edge::boundingRect() const { return m_path.boundingRect(); }

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPath(m_path);
}

void Edge::setup() { Utility::set_gv_attribute(m_gv_edge, "label", m_label); }

void Edge::update_positions()
{
    bezier *curve_data = ED_spl(m_gv_edge)->list;

    const static auto convert_point = [](const pointf &gv_point) {
        return Utility::gv_to_qt_coords({gv_point.x, gv_point.y});
    };

    m_path.moveTo(convert_point(curve_data->list[0]));
    for (auto i = 1; i < curve_data->size; i += 3)
        m_path.cubicTo(
            convert_point(curve_data->list[i]), convert_point(curve_data->list[i + 1]),
            convert_point(curve_data->list[i + 2]));

    if (curve_data->eflag) {
        QPointF arrow_start = convert_point(curve_data->list[curve_data->size - 1]);
        QPointF arrow_end = convert_point(curve_data->ep);
        m_path.lineTo(arrow_end);

        QLineF normal = QLineF(arrow_start, arrow_end).normalVector();
        QPointF arrowhead_vector = QPointF(normal.dx() / 2.0, normal.dy() / 2.0);

        QPolygonF arrowhead({arrow_start - arrowhead_vector, arrow_end, arrow_start + arrowhead_vector});
        m_path.addPolygon(arrowhead);
    }
}
