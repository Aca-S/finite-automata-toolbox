#ifndef UI_OPERATIONS_DOCK_HPP
#define UI_OPERATIONS_DOCK_HPP

#include <QDockWidget>
#include <QGraphicsView>
#include <QPushButton>

namespace Ui {
class OperationsDock : public QDockWidget
{
    Q_OBJECT

  public:
    OperationsDock(QGraphicsView *main_view, QWidget *parent = nullptr);

  private:
    void build_unary_group();
    void setup_unary_group();

    void build_binary_group();
    void setup_binary_group();

    void build_general_group();
    void setup_general_group();

    QPushButton *m_determinize_btn;
    QPushButton *m_minimize_btn;
    QPushButton *m_complete_btn;
    QPushButton *m_reverse_btn;
    QPushButton *m_complement_btn;

    QPushButton *m_union_btn;
    QPushButton *m_intersection_btn;
    QPushButton *m_difference_btn;

    QPushButton *m_clone_btn;
    QPushButton *m_delete_btn;

    QGraphicsView *m_main_view;
};
} // namespace Ui

#endif // UI_OPERATIONS_DOCK_HPP
