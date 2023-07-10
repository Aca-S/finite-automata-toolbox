#include "graph.hpp"

#include <QPainter>

#include "utility.hpp"

Graph::Context::Context() : m_gv_context(gvContext()) {}

Graph::Context::~Context() { gvFreeContext(m_gv_context); }

Graph::Context Graph::m_context;

Graph::Graph() : QGraphicsItem(), m_gv_graph(agopen(const_cast<char *>(""), Agdirected, 0))
{
    Utility::set_gv_attribute(m_gv_graph, "splines", "true");
}

Graph::~Graph()
{
    gvFreeLayout(m_context.m_gv_context, m_gv_graph);
    agclose(m_gv_graph);
}

QRectF Graph::boundingRect() const
{
    // Due to the possible difference in Qt's and Graphviz's labels,
    // we won't use Graphviz's bounding box data, and instead we'll
    // calculate it ourselves in the layout composition phase.
    return m_bounding_rectangle;
}

void Graph::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {}

bool Graph::add_node(Node *node)
{
    // A node cannot be reassigned to a different graph.
    if (node->parentItem())
        return false;

    node->setParentItem(this);
    node->m_gv_node = agnode(m_gv_graph, const_cast<char *>(std::to_string(m_nodes.size()).c_str()), 1);
    node->setup();
    m_nodes.append(node);

    return true;
}

bool Graph::add_edge(Edge *edge, Node *source, Node *destination)
{
    if (source->parentItem() != this || destination->parentItem() != this)
        return false;

    edge->setParentItem(this);
    edge->m_gv_edge = agedge(
        m_gv_graph, source->m_gv_node, destination->m_gv_node,
        const_cast<char *>(std::to_string(m_edges.size()).c_str()), 1);
    edge->setup();
    m_edges.append(edge);

    return true;
}

// Must be called exactly once - after adding all of the nodes and edges
// and before placing on a scene.
void Graph::compose_layout()
{
    gvFreeLayout(m_context.m_gv_context, m_gv_graph);
    gvLayout(m_context.m_gv_context, m_gv_graph, "dot");

    m_bounding_rectangle = QRectF();

    for (Node *n : m_nodes) {
        n->update_positions();
        m_bounding_rectangle |= n->boundingRect();
    }

    for (Edge *e : m_edges) {
        e->update_positions();
        m_bounding_rectangle |= e->boundingRect();
    }
}
