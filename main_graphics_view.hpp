#ifndef MAIN_GRAPHICS_VIEW_HPP
#define MAIN_GRAPHICS_VIEW_HPP

#include <QGraphicsView>

class MainGraphicsView : public QGraphicsView
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

#endif // MAIN_GRAPHICS_VIEW_HPP
