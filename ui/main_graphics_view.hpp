#ifndef UI_MAIN_GRAPHICS_VIEW_HPP
#define UI_MAIN_GRAPHICS_VIEW_HPP

#include <QGraphicsView>

namespace Ui {
class MainGraphicsView : public QGraphicsView
{
    Q_OBJECT

  public:
    MainGraphicsView(QWidget *parent = nullptr);

  signals:
    void viewport_center_changed(QPointF new_center);

  protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

  private:
    qreal m_current_scale_factor = 1.0;
};
} // namespace Ui

#endif // UI_MAIN_GRAPHICS_VIEW_HPP
