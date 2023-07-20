#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:
    void setup_construction_dock();
    void construct_by_element();
    void construct_by_regex();

    void setup_operations_dock();

    void setup_view_dock();
    void bring_selected_to_view();
    void generate_regex();
    void generate_acceptable();
    void generate_unacceptable();

    Ui::MainWindow *ui;
};

#endif // MAIN_WINDOW_HPP
