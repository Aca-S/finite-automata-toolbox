#ifndef UI_MENU_BAR_HPP
#define UI_MENU_BAR_HPP

#include <QMenuBar>

#include "automata_scene.hpp"
#include "scene_tab_bar.hpp"

namespace Ui {
class MenuBar : public QMenuBar
{
    Q_OBJECT

  public:
    MenuBar(QWidget *parent = nullptr);

  public slots:
    void set_scene(AutomataScene *scene);

  signals:
    void scene_opened(AutomataScene *scene);
    void scene_closed();
    void scene_saved_as();

  private:
    void build_file_menu();
    void setup_file_menu();
    void save_file(const QString &file_name);
    void save_with_dialog();
    void open_with_dialog();

    QMenu *m_file_menu;
    QAction *m_new_action;
    QAction *m_open_action;
    QAction *m_save_action;
    QAction *m_save_as_action;
    QAction *m_close_action;

    QMenu *m_edit_menu;
    QAction *m_undo_action;
    QAction *m_redo_action;

    AutomataScene *m_current_scene;
};
} // namespace Ui

#endif // UI_MENU_BAR_HPP
