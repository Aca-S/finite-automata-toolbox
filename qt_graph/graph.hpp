#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <QGraphicsItem>
#include <graphviz/gvc.h>

#include "edge.hpp"
#include "node.hpp"

class Graph : public QGraphicsItem
{
  public:
    Graph();
    ~Graph();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool add_node(Node *node);
    bool add_edge(Edge *edge, Node *source, Node *destination);
    void compose_layout();

  private:
    class Context
    {
      public:
        Context();
        ~Context();

        GVC_t *m_gv_context;
    };

    QRectF m_bounding_rectangle;

    static Context m_context;
    QVector<Node *> m_nodes;
    QVector<Edge *> m_edges;
    Agraph_t *m_gv_graph;
};

#endif // GRAPH_HPP
