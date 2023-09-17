#include "utility.hpp"

using namespace QtGraph::Utility;

QPointF QtGraph::Utility::gv_to_qt_coords(const pointf &gv_point)
{
    static const qreal dpi_adjustment = 92.0 / 72.0;
    return {dpi_adjustment * gv_point.x, -dpi_adjustment * gv_point.y};
}

qreal QtGraph::Utility::gv_to_qt_size(qreal gv_size) { return 92.0 * gv_size; }

void QtGraph::Utility::set_gv_attribute(void *gv_component, const QString &attribute, const QString &value)
{
    agsafeset(
        gv_component, const_cast<char *>(attribute.toUtf8().constData()),
        const_cast<char *>(value.toUtf8().constData()), const_cast<char *>(""));
}
