#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <QPointF>

namespace Utility {
QPointF gv_to_qt_coords(const QPointF &gv_point);
qreal gv_to_qt_size(const qreal &gv_size);
} // namespace Utility

#endif // UTILITY_HPP
