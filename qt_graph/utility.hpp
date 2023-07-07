#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <QPointF>
#include <QString>

namespace Utility {
QPointF gv_to_qt_coords(const QPointF &gv_point);
qreal gv_to_qt_size(const qreal &gv_size);
void set_gv_attribute(void *gv_component, const QString &attribute, const QString &value);
} // namespace Utility

#endif // UTILITY_HPP
