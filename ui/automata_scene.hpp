#ifndef UI_AUTOMATA_SCENE_HPP
#define UI_AUTOMATA_SCENE_HPP

#include <QGraphicsScene>

namespace Ui {
class AutomataScene : public QGraphicsScene
{
  public:
    AutomataScene(QWidget *parent = nullptr);

    void set_scene_name(const QString &scene_name);
    QString get_scene_name();

    bool has_unsaved_changes();

  private:
    QString m_scene_name;
    bool m_has_unsaved_changes = false;
};
} // namespace Ui

#endif // UI_AUTOMATA_SCENE_HPP
