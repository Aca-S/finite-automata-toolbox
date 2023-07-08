#ifndef NODE_HPP
#define NODE_HPP

#include <QFont>
#include <QGraphicsItem>
#include <graphviz/gvc.h>

class Node : public QGraphicsItem
{
    friend class Graph;

  public:
    Node(qreal width, qreal height, const QString &label, const QFont &label_font = QFont("Times-Roman", 12));

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

  private:
    void setup();

    qreal m_width;
    qreal m_height;
    QString m_label;
    QFont m_label_font;

    Agnode_t *m_gv_node;
};

#endif // NODE_HPP
