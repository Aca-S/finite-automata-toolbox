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
};

#endif // MAIN_GRAPHICS_VIEW_HPP
