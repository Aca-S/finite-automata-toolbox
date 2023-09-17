#ifndef QT_GRAPH_NODE_HPP
#define QT_GRAPH_NODE_HPP

#include <QFont>
#include <QGraphicsItem>
#include <graphviz/gvc.h>

namespace QtGraph {
class Node : public QGraphicsItem
{
    friend class Graph;

  public:
    Node(qreal width, qreal height, const QString &label, const QFont &label_font = QFont("Times-Roman", 12));

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

  private:
    void setup();
    void update_positions();

    qreal m_width;
    qreal m_height;
    QString m_label;
    QFont m_label_font;

    QPainterPath m_path;
    QPointF m_label_position;
    QRectF m_bounding_rectangle;

    Agnode_t *m_gv_node;
};
} // namespace QtGraph

#endif // QT_GRAPH_NODE_HPP
