#include "utility.hpp"

QPointF Utility::gv_to_qt_coords(const QPointF &gv_point)
{
    static const qreal dpi_adjustment = 92.0 / 72.0;
    return {dpi_adjustment * gv_point.x(), -dpi_adjustment * gv_point.y()};
}

qreal Utility::gv_to_qt_size(const qreal &gv_size) { return 92.0 * gv_size; }
