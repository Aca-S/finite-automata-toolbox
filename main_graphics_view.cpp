#include "main_graphics_view.hpp"

MainGraphicsView::MainGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSceneRect(-16000, -16000, 32000, 32000);
}
