#include "automaton_graph.hpp"

#include <QPainter>

#include "circle_node.hpp"
#include "double_circle_node.hpp"
#include "invisible_node.hpp"
#include "transition_edge.hpp"

AutomatonGraph::AutomatonGraph(const FiniteAutomaton &automaton) : QtGraph::Graph(), m_automaton(automaton)
{
    using namespace QtGraph;

    qreal node_size = 50;

    for (const auto &state : m_automaton.get_states()) {
        AutomatonNode *node;
        if (m_automaton.get_final_states().contains(state))
            node = new DoubleCircleNode(node_size, node_size - 8, QString::number(state));
        else
            node = new CircleNode(node_size, QString::number(state));
        add_node(node);
        m_node_map.insert(state, node);

        if (m_automaton.get_initial_states().contains(state)) {
            Node *invisible_node = new InvisibleNode();
            Edge *initial_edge = new TransitionEdge("");
            add_node(invisible_node);
            add_edge(initial_edge, invisible_node, node);
        }
    }

    // Compress the edges to form a strict graph.
    QMap<QPair<unsigned, unsigned>, QString> label_map;
    for (const auto &[k, v] : m_automaton.get_transition_function()) {
        for (const auto &to_state : v) {
            QString symbol = k.second == FiniteAutomaton::epsilon_transition_value ? QString::fromUtf8("\u03B5")
                                                                                   : QString(QChar(k.second));
            auto it = label_map.find({k.first, to_state});
            if (it == label_map.end())
                label_map.insert({k.first, to_state}, symbol);
            else
                it.value() = it.value() + ", " + symbol;
        }
    }

    for (const auto &[k, label] : label_map.asKeyValueRange()) {
        Edge *edge = new TransitionEdge(label);
        add_edge(edge, m_node_map[k.first], m_node_map[k.second]);
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

int AutomatonGraph::type() const { return Type; }

const FiniteAutomaton &AutomatonGraph::get_automaton() const { return m_automaton; }

void AutomatonGraph::activate_state_nodes(const std::set<unsigned> &states)
{
    for (const auto &state : states)
        m_node_map[state]->activate();
}

void AutomatonGraph::deactivate_state_nodes(const std::set<unsigned> &states)
{
    for (const auto &state : states)
        m_node_map[state]->deactivate();
}
