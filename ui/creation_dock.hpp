#ifndef UI_CREATION_DOCK_HPP
#define UI_CREATION_DOCK_HPP

#include <QDockWidget>
#include <QGraphicsView>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>

namespace Ui {
class CreationDock : public QDockWidget
{
    Q_OBJECT

  public:
    CreationDock(QWidget *parent = nullptr);

  private:
    void build_element_group();
    void setup_element_group();
    void construct_by_element();

    void build_regex_group();
    void setup_regex_group();
    void construct_by_regex();

    QLineEdit *m_alphabet_le, *m_states_le, *m_initial_states_le, *m_final_states_le, *m_transition_le;
    QListWidget *m_transition_list;
    QPushButton *m_add_transition_btn, *m_remove_transition_btn, *m_element_construct_btn;
    QLabel *m_element_construct_info;

    QLineEdit *m_regex_le;
    QPushButton *m_regex_construct_btn;
    QLabel *m_regex_construct_info;

  signals:
    void operation_triggered(const std::function<void(QGraphicsView *view)> &op);
};
} // namespace Ui

#endif // UI_CREATION_DOCK_HPP
