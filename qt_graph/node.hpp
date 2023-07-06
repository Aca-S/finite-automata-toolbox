#ifndef NODE_HPP
#define NODE_HPP

#include <QGraphicsItem>
#include <graphviz/gvc.h>

#include "graph.hpp"

class Node : public QGraphicsItem
{
  public:
    Node(Graph *parent_graph, const QString &label);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

  private:
    QString m_label;

    Agnode_t *m_gv_node;
};

#endif // NODE_HPP
