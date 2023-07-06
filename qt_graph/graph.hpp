#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <QGraphicsItem>
#include <graphviz/gvc.h>

class Node;

class Graph : public QGraphicsItem
{
    friend class Node;

  public:
    Graph();
    ~Graph();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

  private:
    class Context
    {
      public:
        Context();
        ~Context();

        GVC_t *m_gv_context;
    };

    static QPointF gv_to_qt_coords(const QPointF &gv_point);
    static qreal gv_to_qt_size(const qreal &gv_size);

    void update_layout();

    static Context m_context;

    QVector<Node *> m_nodes;

    Agraph_t *m_gv_graph;
};

#endif // GRAPH_HPP
