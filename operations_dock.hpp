#ifndef OPERATIONS_DOCK_HPP
#define OPERATIONS_DOCK_HPP

#include <QDockWidget>
#include <QGraphicsView>

class OperationsDock : public QDockWidget
{
    Q_OBJECT

  public:
    OperationsDock(QGraphicsView *main_view, QWidget *parent = nullptr);

  private:
    QGraphicsView *m_main_view;
};

#endif // OPERATIONS_DOCK_HPP
