#ifndef UI_OPERATIONS_DOCK_HPP
#define UI_OPERATIONS_DOCK_HPP

#include <QDockWidget>
#include <QGraphicsView>

namespace Ui {
class OperationsDock : public QDockWidget
{
    Q_OBJECT

  public:
    OperationsDock(QGraphicsView *main_view, QWidget *parent = nullptr);

  private:
    QGraphicsView *m_main_view;
};
} // namespace Ui

#endif // UI_OPERATIONS_DOCK_HPP
