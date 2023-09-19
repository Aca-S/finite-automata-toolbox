#ifndef UI_SCENE_TAB_BAR_HPP
#define UI_SCENE_TAB_BAR_HPP

#include <QGraphicsView>
#include <QTabBar>

class SceneTabBar : public QTabBar
{
  public:
    SceneTabBar(QGraphicsView *main_view, QWidget *parent = nullptr);

  private:
    QGraphicsView *m_main_view;
};

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

#endif // UI_SCENE_TAB_BAR_HPP
