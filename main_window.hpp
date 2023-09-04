#ifndef UI_MAIN_WINDOW_HPP
#define UI_MAIN_WINDOW_HPP

#include <QGraphicsView>
#include <QMainWindow>

namespace Ui {
class MainWindow : public QMainWindow
{
    Q_OBJECT

    class MainGraphicsView;

  public:
    MainWindow(QWidget *parent = nullptr);
};

class MainWindow::MainGraphicsView : public QGraphicsView
{
    Q_OBJECT

  public:
    MainGraphicsView(QWidget *parent = nullptr);

  protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

  private:
    qreal m_current_scale_factor = 1.0;
};
} // namespace Ui

#endif // UI_MAIN_WINDOW_HPP
