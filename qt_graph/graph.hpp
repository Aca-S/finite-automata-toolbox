#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <QGraphicsItem>
#include <graphviz/gvc.h>

#include "node.hpp"

class Graph : public QGraphicsItem
{
  public:
    Graph();
    ~Graph();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void add_node(Node *node);

  private:
    class Context
    {
      public:
        Context();
        ~Context();

        GVC_t *m_gv_context;
    };

    void update_layout();

    static Context m_context;
    QVector<Node *> m_nodes;
    Agraph_t *m_gv_graph;
};

#endif // GRAPH_HPP
