#ifndef UI_MENU_BAR_HPP
#define UI_MENU_BAR_HPP

#include <QGraphicsView>
#include <QMenuBar>

namespace Ui {
class MenuBar : public QMenuBar
{
    Q_OBJECT

  public:
    MenuBar(QGraphicsView *main_view, QWidget *parent = nullptr);

  private:
    void build_file_menu();
    void setup_file_menu();

    QMenu *m_file_menu;
    QAction *m_new_action;
    QAction *m_open_action;
    QAction *m_save_as_action;

    QGraphicsView *m_main_view;
};
} // namespace Ui

#endif // UI_MENU_BAR_HPP
