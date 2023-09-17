#ifndef QT_GRAPH_UTILITY_HPP
#define QT_GRAPH_UTILITY_HPP

#include <QPointF>
#include <QString>
#include <graphviz/gvc.h>

namespace QtGraph::Utility {
QPointF gv_to_qt_coords(const pointf &gv_point);
qreal gv_to_qt_size(double gv_size);
void set_gv_attribute(void *gv_component, const QString &attribute, const QString &value);
} // namespace QtGraph::Utility

#endif // QT_GRAPH_UTILITY_HPP
