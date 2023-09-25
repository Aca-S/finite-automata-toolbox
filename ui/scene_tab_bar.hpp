#ifndef UI_SCENE_TAB_BAR_HPP
#define UI_SCENE_TAB_BAR_HPP

#include <QGraphicsView>
#include <QTabBar>

#include "automata_scene.hpp"

namespace Ui {
class SceneTabBar : public QTabBar
{
    Q_OBJECT

  public:
    SceneTabBar(QWidget *parent = nullptr);
    AutomataScene *get_current_scene();

  public slots:
    void add_scene(AutomataScene *scene);
    void remove_current_scene();
    void update_current_tab_label();

  signals:
    void scene_changed(AutomataScene *scene);

  private:
    QList<AutomataScene *> m_automata_scenes;
};
} // namespace Ui

#endif // UI_SCENE_TAB_BAR_HPP
