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

#endif // UI_SCENE_TAB_BAR_HPP
