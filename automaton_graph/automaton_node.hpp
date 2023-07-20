#ifndef AUTOMATON_NODE_HPP
#define AUTOMATON_NODE_HPP

#include "node.hpp"

class AutomatonNode : public Node
{
  public:
    AutomatonNode(qreal size, const QString &label, const QFont &label_font = QFont("Times-Roman", 12));

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void activate();
    void deactivate();

  private:
    bool m_is_active = false;
};

#endif // AUTOMATON_NODE_HPP
