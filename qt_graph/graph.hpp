#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <QGraphicsItem>
#include <graphviz/gvc.h>

#include <iostream>

class Graph : public QGraphicsItem
{
  public:
    QRectF boundingRect() const override { return QRectF(); }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {}

  private:
    class Context
    {
      public:
        Context();
        ~Context();

        GVC_t *m_gv_context;
    };

    static Context m_context;
};

#endif // GRAPH_HPP
