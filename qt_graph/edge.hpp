#ifndef EDGE_HPP
#define EDGE_HPP

#include <QFont>
#include <QGraphicsItem>
#include <graphviz/gvc.h>

class Edge : public QGraphicsItem
{
    friend class Graph;

  public:
    Edge(const QString &label, const QFont &label_font = QFont("Times-Roman", 12));

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

  private:
    void setup();
    void update_positions();

    QString m_label;
    QFont m_label_font;

    QPainterPath m_path;
    QPointF m_label_position;
    QRectF m_bounding_rectangle;

    Agedge_t *m_gv_edge;
};

#endif // EDGE_HPP
