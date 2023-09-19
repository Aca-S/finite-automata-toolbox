#ifndef UI_MENU_BAR_HPP
#define UI_MENU_BAR_HPP

#include <QMenuBar>

#include "scene_tab_bar.hpp"

namespace Ui {
class MenuBar : public QMenuBar
{
    Q_OBJECT

  public:
    MenuBar(SceneTabBar *scene_tab_bar, QWidget *parent = nullptr);

  private:
    void build_file_menu();
    void setup_file_menu();

    QMenu *m_file_menu;
    QAction *m_new_action;
    QAction *m_open_action;
    QAction *m_save_as_action;
    QAction *m_close_action;

    SceneTabBar *m_scene_tab_bar;
};
} // namespace Ui

#endif // UI_MENU_BAR_HPP
