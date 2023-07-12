#include "main_graphics_view.hpp"

#include <QKeyEvent>

MainGraphicsView::MainGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSceneRect(-16000, -16000, 32000, 32000);
}

void MainGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        setDragMode(QGraphicsView::RubberBandDrag);
    QGraphicsView::keyPressEvent(event);
}

void MainGraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        setDragMode(QGraphicsView::ScrollHandDrag);
    QGraphicsView::keyPressEvent(event);
}
