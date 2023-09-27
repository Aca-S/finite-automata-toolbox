#ifndef UI_MAIN_WINDOW_HPP
#define UI_MAIN_WINDOW_HPP

#include <QMainWindow>

namespace Ui {
class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
};
} // namespace Ui

#endif // UI_MAIN_WINDOW_HPP
