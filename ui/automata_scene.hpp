#ifndef UI_AUTOMATA_SCENE_HPP
#define UI_AUTOMATA_SCENE_HPP

#include <QGraphicsScene>
#include <QStack>
#include <QUndoCommand>

#include <expected>
#include <optional>

namespace Ui {
class AutomataScene : public QGraphicsScene
{
    class AddCommand;
    class RemoveCommand;
    class ReplaceCommand;
    class MoveCommand;

  public:
    AutomataScene(QWidget *parent = nullptr);

    static std::expected<AutomataScene *, QString> load_from_file(const QString &file_name, QWidget *parent = nullptr);
    std::optional<QString> save_to_file(const QString &file_name);

    void add_automata(const QList<QPair<QGraphicsItem *, QPointF>> &items);
    void remove_automata(const QList<QGraphicsItem *> &items);
    void
    replace_automata(const QList<QGraphicsItem *> &old_items, const QList<QPair<QGraphicsItem *, QPointF>> &new_items);
    void move_automata(const QList<QPair<QGraphicsItem *, QPointF>> &old_positions);

    QString get_name() const;

  public slots:
    void undo_action();
    void redo_action();

  protected:
    // Override the mouse press and release events to detect scene item movements.
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

  private:
    QString m_name;
    QUndoStack *m_undo_stack = new QUndoStack(this);
    QList<QPair<QGraphicsItem *, QPointF>> m_moving_items;
};

class AutomataScene::AddCommand : public QUndoCommand
{
  public:
    AddCommand(
        QGraphicsScene *scene, const QList<QPair<QGraphicsItem *, QPointF>> &items, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

  private:
    QGraphicsScene *m_scene;
    QList<QPair<QGraphicsItem *, QPointF>> m_items;
};

class AutomataScene::RemoveCommand : public QUndoCommand
{
  public:
    RemoveCommand(QGraphicsScene *scene, const QList<QGraphicsItem *> &items, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

  private:
    QGraphicsScene *m_scene;
    QList<QGraphicsItem *> m_items;
};

class AutomataScene::ReplaceCommand : public QUndoCommand
{
  public:
    ReplaceCommand(
        QGraphicsScene *scene, const QList<QGraphicsItem *> &old_items,
        const QList<QPair<QGraphicsItem *, QPointF>> &new_items, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

  private:
    QGraphicsScene *m_scene;
    QList<QGraphicsItem *> m_old_items;
    QList<QPair<QGraphicsItem *, QPointF>> m_new_items;
};

class AutomataScene::MoveCommand : public QUndoCommand
{
  public:
    MoveCommand(
        QGraphicsScene *scene, const QList<QPair<QGraphicsItem *, QPointF>> &old_positions,
        QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

  private:
    QGraphicsScene *m_scene;
    QList<QPair<QGraphicsItem *, QPointF>> m_old_positions;
    QList<QPointF> m_new_positions;
};
} // namespace Ui

#endif // UI_AUTOMATA_SCENE_HPP
