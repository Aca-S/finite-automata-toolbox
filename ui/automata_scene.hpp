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

  public:
    AutomataScene(QWidget *parent = nullptr);

    static std::expected<AutomataScene *, QString> load_from_file(const QString &file_name, QWidget *parent = nullptr);
    std::optional<QString> save_to_file(const QString &file_name);

    void add_automata(const QList<QPair<QGraphicsItem *, QPointF>> &items);
    void remove_automata(const QList<QGraphicsItem *> &items);

    QString get_name() const;

  public slots:
    void undo_action();
    void redo_action();

  private:
    QString m_name;
    QUndoStack *m_undo_stack = new QUndoStack(this);
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
} // namespace Ui

#endif // UI_AUTOMATA_SCENE_HPP
