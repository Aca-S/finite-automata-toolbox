#include "main_graphics_view.hpp"

#include <QKeyEvent>
#include <QWheelEvent>

MainGraphicsView::MainGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
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

void MainGraphicsView::wheelEvent(QWheelEvent *event)
{
    qreal factor_of_change = 1.1;
    qreal max_scale_factor = 1.5;
    qreal min_scale_factor = 0.35;

    if (event->angleDelta().y() > 0) {
        qreal new_scale_factor = m_current_scale_factor * factor_of_change;
        if (new_scale_factor < max_scale_factor) {
            scale(factor_of_change, factor_of_change);
            m_current_scale_factor = new_scale_factor;
        }
    } else if (event->angleDelta().y() < 0) {
        qreal new_scale_factor = m_current_scale_factor / factor_of_change;
        if (new_scale_factor > min_scale_factor) {
            scale(1 / factor_of_change, 1 / factor_of_change);
            m_current_scale_factor = new_scale_factor;
        }
    }
}
