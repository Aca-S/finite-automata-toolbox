#include "automata_scene.hpp"

#include "automaton_graph.hpp"
#include "finite_automaton.hpp"
#include "utility.hpp"

#include <QDataStream>
#include <QFile>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>

using namespace Ui;

AutomataScene::AutomataScene(QWidget *parent) : QGraphicsScene(parent) {}

QString AutomataScene::get_name() const { return m_name; }

void AutomataScene::undo_action() { m_undo_stack->undo(); }

void AutomataScene::redo_action() { m_undo_stack->redo(); }

namespace {
std::expected<FiniteAutomaton, QString> deserialize_automaton(QDataStream &in)
{
    QSet<char> q_alphabet;
    QSet<unsigned> q_states, q_initial_states, q_final_states;
    QMap<QPair<unsigned, char>, QSet<unsigned>> q_transition_function;

    in >> q_alphabet >> q_states >> q_initial_states >> q_final_states >> q_transition_function;
    if (in.status() != QDataStream::Ok)
        return std::unexpected("File format error");

    std::set<char> alphabet(q_alphabet.begin(), q_alphabet.end());
    std::set<unsigned> states(q_states.begin(), q_states.end());
    std::set<unsigned> initial_states(q_initial_states.begin(), q_initial_states.end());
    std::set<unsigned> final_states(q_final_states.begin(), q_final_states.end());
    std::map<std::pair<unsigned, char>, std::set<unsigned>> transition_function;
    QMapIterator<QPair<unsigned, char>, QSet<unsigned>> it(q_transition_function);
    while (it.hasNext()) {
        it.next();
        transition_function[{it.key().first, it.key().second}] =
            std::set<unsigned>(it.value().begin(), it.value().end());
    }

    auto automaton = FiniteAutomaton::construct(alphabet, states, initial_states, final_states, transition_function);
    if (!automaton)
        return std::unexpected(QString::fromUtf8(automaton.error().c_str()));

    return *automaton;
}

std::expected<AutomataScene *, QString> deserialize_scene(QDataStream &in)
{
    AutomataScene *scene = new AutomataScene;
    qsizetype num_of_automata;
    in >> num_of_automata;
    for (qsizetype i = 0; i < num_of_automata; ++i) {
        auto automaton = deserialize_automaton(in);
        QPointF pos;
        in >> pos;
        if (in.status() != QDataStream::Ok || !automaton) {
            delete scene;
            return std::unexpected("File format error");
        }
        Utility::add_item_at_pos(new AutomatonGraph(*automaton), scene, pos);
    }

    return scene;
}
} // namespace

std::expected<AutomataScene *, QString> AutomataScene::load_from_file(const QString &file_name, QWidget *parent)
{
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly))
        return std::unexpected(file.errorString());

    QDataStream in(&file);
    auto scene = deserialize_scene(in);
    file.close();

    if (scene) {
        scene.value()->m_name = file_name;
        scene.value()->setParent(parent);
    }

    return scene;
}

namespace {
void serialize_automaton(QDataStream &out, const FiniteAutomaton &automaton)
{
    QSet<char> alphabet(automaton.get_alphabet().begin(), automaton.get_alphabet().end());
    QSet<unsigned> states(automaton.get_states().begin(), automaton.get_states().end());
    QSet<unsigned> initial_states(automaton.get_initial_states().begin(), automaton.get_initial_states().end());
    QSet<unsigned> final_states(automaton.get_final_states().begin(), automaton.get_final_states().end());
    QMap<QPair<unsigned, char>, QSet<unsigned>> transition_function;
    for (const auto &[k, v] : automaton.get_transition_function())
        transition_function.insert(k, QSet<unsigned>(v.begin(), v.end()));

    out << alphabet << states << initial_states << final_states << transition_function;
}

void serialize_scene(QDataStream &out, AutomataScene *scene)
{
    auto graphs = Utility::get_items<AutomatonGraph>(scene);
    out << graphs.size();
    for (auto *graph : graphs) {
        serialize_automaton(out, graph->get_automaton());
        out << Utility::get_center_pos(graph);
    }
}
} // namespace

// In case the save operation fails, an error is returned.
std::optional<QString> AutomataScene::save_to_file(const QString &file_name)
{
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly))
        return file.errorString();

    QDataStream out(&file);
    serialize_scene(out, this);
    file.close();

    m_name = file_name;

    return std::nullopt;
}

void AutomataScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Must handle the base class event before grabbing the selected items.
    QGraphicsScene::mousePressEvent(event);

    m_moving_items.clear();
    for (auto *item : this->selectedItems())
        m_moving_items.append({item, item->pos()});
}

void AutomataScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);

    if (event->button() == Qt::LeftButton) {
        for (auto &[item, old_pos] : m_moving_items) {
            // If any item at all changed its position, trigger the move action.
            if (old_pos != item->pos()) {
                move_automata(m_moving_items);
                break;
            }
        }
    }
}

void AutomataScene::add_automata(const QList<QPair<QGraphicsItem *, QPointF>> &items)
{
    m_undo_stack->push(new AddCommand(this, items));
}

void AutomataScene::remove_automata(const QList<QGraphicsItem *> &items)
{
    m_undo_stack->push(new RemoveCommand(this, items));
}

void AutomataScene::replace_automata(
    const QList<QGraphicsItem *> &old_items, const QList<QPair<QGraphicsItem *, QPointF>> &new_items)
{
    m_undo_stack->push(new ReplaceCommand(this, old_items, new_items));
}

void AutomataScene::move_automata(const QList<QPair<QGraphicsItem *, QPointF>> &old_positions)
{
    m_undo_stack->push(new MoveCommand(this, old_positions));
}

AutomataScene::AddCommand::AddCommand(
    QGraphicsScene *scene, const QList<QPair<QGraphicsItem *, QPointF>> &items, QUndoCommand *parent)
    : m_scene(scene), m_items(items), QUndoCommand(parent)
{
}

void AutomataScene::AddCommand::undo()
{
    for (auto &[item, _] : m_items)
        m_scene->removeItem(item);
}

void AutomataScene::AddCommand::redo()
{
    for (auto &[item, pos] : m_items)
        Utility::add_item_at_pos(item, m_scene, pos);
}

AutomataScene::RemoveCommand::RemoveCommand(
    QGraphicsScene *scene, const QList<QGraphicsItem *> &items, QUndoCommand *parent)
    : m_scene(scene), m_items(items), QUndoCommand(parent)
{
}

void AutomataScene::RemoveCommand::undo()
{
    for (auto *item : m_items)
        m_scene->addItem(item);
}

void AutomataScene::RemoveCommand::redo()
{
    for (auto *item : m_items)
        m_scene->removeItem(item);
}

AutomataScene::ReplaceCommand::ReplaceCommand(
    QGraphicsScene *scene, const QList<QGraphicsItem *> &old_items,
    const QList<QPair<QGraphicsItem *, QPointF>> &new_items, QUndoCommand *parent)
    : m_scene(scene), m_old_items(old_items), m_new_items(new_items), QUndoCommand(parent)
{
}

void AutomataScene::ReplaceCommand::undo()
{
    for (auto &[item, _] : m_new_items)
        m_scene->removeItem(item);

    for (auto *item : m_old_items)
        m_scene->addItem(item);
}

void AutomataScene::ReplaceCommand::redo()
{
    for (auto *item : m_old_items)
        m_scene->removeItem(item);

    for (auto &[item, pos] : m_new_items)
        Utility::add_item_at_pos(item, m_scene, pos);
}

AutomataScene::MoveCommand::MoveCommand(
    QGraphicsScene *scene, const QList<QPair<QGraphicsItem *, QPointF>> &old_positions, QUndoCommand *parent)
    : m_scene(scene), m_old_positions(old_positions), QUndoCommand(parent)
{
    for (auto &[item, _] : m_old_positions)
        m_new_positions.append(item->pos());
}

void AutomataScene::MoveCommand::undo()
{
    for (auto &[item, old_pos] : m_old_positions)
        item->setPos(old_pos);
}

void AutomataScene::MoveCommand::redo()
{
    int i = 0;
    for (auto &[item, _] : m_old_positions)
        item->setPos(m_new_positions[i++]);
}
