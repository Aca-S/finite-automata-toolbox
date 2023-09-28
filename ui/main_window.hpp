#ifndef UI_MAIN_WINDOW_HPP
#define UI_MAIN_WINDOW_HPP

#include <QMainWindow>

#include "creation_dock.hpp"
#include "main_graphics_view.hpp"
#include "menu_bar.hpp"
#include "operations_dock.hpp"
#include "scene_tab_bar.hpp"
#include "utility.hpp"
#include "view_dock.hpp"

namespace Ui {
class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);

  private:
    void build();
    void setup();

    MainGraphicsView *m_main_view;
    SceneTabBar *m_tab_bar;
    MenuBar *m_menu_bar;
    ViewDock *m_view_dock;
    CreationDock *m_creation_dock;
    OperationsDock *m_operations_dock;
};
} // namespace Ui

#endif // UI_MAIN_WINDOW_HPP
