#ifndef UI_OPERATIONS_DOCK_HPP
#define UI_OPERATIONS_DOCK_HPP

#include <QDockWidget>
#include <QGraphicsView>
#include <QPushButton>

#include <functional>

#include "automata_scene.hpp"

namespace Ui {
class OperationsDock : public QDockWidget
{
    Q_OBJECT

  public:
    OperationsDock(QWidget *parent = nullptr);

  public slots:
    void set_scene(AutomataScene *scene);
    void set_viewport_center(QPointF center);

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

    AutomataScene *m_current_scene;
    QPointF m_viewport_center;
};
} // namespace Ui

#endif // UI_OPERATIONS_DOCK_HPP
