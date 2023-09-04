#ifndef VIEW_DOCK_HPP
#define VIEW_DOCK_HPP

#include <QDockWidget>
#include <QGraphicsView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class ViewDock : public QDockWidget
{
    Q_OBJECT

    class SideGraphicsView;

  public:
    ViewDock(QGraphicsView *main_view, QWidget *parent = nullptr);

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

    QGraphicsView *m_main_view;
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

#endif // VIEW_DOCK_HPP
