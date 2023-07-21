#ifndef SIDE_GRAPHICS_VIEW_HPP
#define SIDE_GRAPHICS_VIEW_HPP

#include <QGraphicsView>

class SideGraphicsView : public QGraphicsView
{
    Q_OBJECT

  public:
    SideGraphicsView(QWidget *parent = nullptr);

  protected:
    void wheelEvent(QWheelEvent *event) override;

  private:
    qreal m_current_scale_factor = 1.0;
};

#endif // SIDE_GRAPHICS_VIEW_HPP
