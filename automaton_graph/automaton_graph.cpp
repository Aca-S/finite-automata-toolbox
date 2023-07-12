#include "automaton_graph.hpp"

#include <QPainter>

#include "circle_node.hpp"
#include "double_circle_node.hpp"
#include "invisible_node.hpp"
#include "transition_edge.hpp"

AutomatonGraph::AutomatonGraph(const FiniteAutomaton &automaton) : Graph(), m_automaton(automaton)
{
    qreal node_size = 50;

    QMap<unsigned, Node *> node_map;
    for (const auto &state : m_automaton.get_states()) {
        Node *node;
        if (m_automaton.get_final_states().contains(state))
            node = new DoubleCircleNode(node_size, node_size - 8, QString::number(state));
        else
            node = new CircleNode(node_size, QString::number(state));
        add_node(node);
        node_map.insert(state, node);

        if (m_automaton.get_initial_states().contains(state)) {
            Node *invisible_node = new InvisibleNode();
            Edge *initial_edge = new TransitionEdge("");
            add_node(invisible_node);
            add_edge(initial_edge, invisible_node, node);
        }
    }

    for (const auto &[k, v] : m_automaton.get_transition_function()) {
        for (const auto &to_state : v) {
            QString label = k.second == FiniteAutomaton::epsilon_transition_value ? QString::fromUtf8("\u03B5")
                                                                                  : QString(QChar(k.second));
            Edge *edge = new TransitionEdge(label);
            add_edge(edge, node_map[k.first], node_map[to_state]);
        }
    }

    compose_layout();

    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
}

QRectF AutomatonGraph::boundingRect() const { return Graph::boundingRect(); }

void AutomatonGraph::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    Graph::paint(painter, option, widget);
    if (isSelected())
        painter->drawRect(boundingRect());
}
