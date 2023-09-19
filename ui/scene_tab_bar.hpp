#ifndef UI_SCENE_TAB_BAR_HPP
#define UI_SCENE_TAB_BAR_HPP

#include <QGraphicsView>
#include <QTabBar>

#include "automata_scene.hpp"

namespace Ui {
class SceneTabBar : public QTabBar
{
  public:
    SceneTabBar(QGraphicsView *main_view, QWidget *parent = nullptr);

    int add_scene_tab(AutomataScene *scene = nullptr);
    void remove_scene_tab(int index);
    void remove_scene_tab();

    AutomataScene *get_current_scene();

  private:
    QList<AutomataScene *> m_automata_scenes;

    QGraphicsView *m_main_view;
};
} // namespace Ui

#endif // UI_SCENE_TAB_BAR_HPP
