#ifndef MAIN_GRAPHICS_VIEW_HPP
#define MAIN_GRAPHICS_VIEW_HPP

#include <QGraphicsView>

class MainGraphicsView : public QGraphicsView
{
    Q_OBJECT

  public:
    MainGraphicsView(QWidget *parent = nullptr);
};

#endif // MAIN_GRAPHICS_VIEW_HPP
