#ifndef NODE_HPP
#define NODE_HPP

#include <QGraphicsItem>
#include <graphviz/gvc.h>

class Node : public QGraphicsItem
{
    friend class Graph;

  public:
    Node(const QString &label);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

  private:
    void setup();

    QString m_label;
    Agnode_t *m_gv_node;
};

#endif // NODE_HPP
