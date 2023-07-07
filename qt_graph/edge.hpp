#ifndef EDGE_HPP
#define EDGE_HPP

#include <QGraphicsItem>
#include <graphviz/gvc.h>

class Edge : public QGraphicsItem
{
    friend class Graph;

  public:
    Edge(const QString &label);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

  private:
    void setup();
    void update_positions();

    QString m_label;
    QPainterPath m_path;
    Agedge_t *m_gv_edge;
};

#endif // EDGE_HPP
