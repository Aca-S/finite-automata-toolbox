#ifndef UI_VIEW_DOCK_HPP
#define UI_VIEW_DOCK_HPP

#include <QDockWidget>
#include <QGraphicsView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "automata_scene.hpp"

namespace Ui {
class ViewDock : public QDockWidget
{
    Q_OBJECT

    class SideGraphicsView;

  public:
    ViewDock(QWidget *parent = nullptr);
    void set_scene(AutomataScene *scene);

  private:
    void build_view_section();
    void setup_view_section();

    void build_match_section();
    void setup_match_section();

    void build_regex_section();
    void setup_regex_section();

    SideGraphicsView *m_side_view;
    QPushButton *m_view_btn;

    QPushButton *m_acceptable_word_btn, *m_unacceptable_word_btn;
    QLineEdit *m_word_le;
    QPushButton *m_match_reset_btn, *m_match_back_btn, *m_match_next_btn, *m_match_end_btn;

    QPushButton *m_regex_btn;
    QLineEdit *m_regex_le;

    QLabel *m_view_info;

    AutomataScene *m_current_scene;
};

class ViewDock::SideGraphicsView : public QGraphicsView
{
    Q_OBJECT

  public:
    SideGraphicsView(QWidget *parent = nullptr);

  protected:
    void wheelEvent(QWheelEvent *event) override;

  private:
    qreal m_current_scale_factor = 1.0;
};
} // namespace Ui

#endif // UI_VIEW_DOCK_HPP
