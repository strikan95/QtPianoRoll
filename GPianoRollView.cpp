#include "GPianoRollView.h"
#include "GPianoRollScene.h"

#include <QKeyEvent>
#include <QScrollBar>
#include <QMenu>

GPianoRollView::GPianoRollView(QWidget *parent) :
    QGraphicsView(parent)
{
    setDragMode(QGraphicsView::NoDrag);
    setRenderHint(QPainter::Antialiasing);

    setBackgroundBrush(Qt::NoBrush);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    setCacheMode(QGraphicsView::CacheNone);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
}

GPianoRollView::GPianoRollView(GPianoRollScene *scene, QWidget *parent)
    : GPianoRollView(parent)
{
    setScene(scene);

    // Sets the scene rect to its maximum possible ranges to avoid autu scene range
    // re-calculation when expanding the all QGraphicsItems common rect.
    int maxSize = 32767;
    setSceneRect(0, 0, maxSize * 2, maxSize * 2);

    setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void GPianoRollView::mousePressEvent(QMouseEvent *e)
{
    switch (e->button()) {
    case Qt::MiddleButton:
    {
        setCursor(Qt::ClosedHandCursor);
        mLastMousePosition = e->position().toPoint();
        return;
    }
    case Qt::LeftButton:
    {
        if(e->modifiers() & Qt::ControlModifier)
        {
            setDragMode(QGraphicsView::RubberBandDrag);
            qDebug() << "Setting rubber band drag";
        }
    }
    default:
        break;
    }

    QGraphicsView::mousePressEvent(e);
}

void GPianoRollView::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::MiddleButton)
    {
        QScrollBar *hBar = horizontalScrollBar();
        QScrollBar *vBar = verticalScrollBar();
        QPoint delta = e->position().toPoint() - mLastMousePosition;
        qDebug() << delta;
        hBar->setValue(hBar->value() + (isRightToLeft() ? delta.x() : -delta.x()));
        vBar->setValue(vBar->value() - delta.y());

        mLastMousePosition = e->position().toPoint();
        return;
    }

    QGraphicsView::mouseMoveEvent(e);
}

void GPianoRollView::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() & Qt::MiddleButton)
    {
        unsetCursor();
        return;
    }

    if(e->button() & Qt::LeftButton && e->modifiers() & Qt::ControlModifier)
    {
        setDragMode(QGraphicsView::NoDrag);
    }

    QGraphicsView::mouseReleaseEvent(e);
}

void GPianoRollView::keyPressEvent(QKeyEvent *e)
{
    QGraphicsView::keyPressEvent(e);
}

void GPianoRollView::keyReleaseEvent(QKeyEvent *e)
{
    QGraphicsView::keyReleaseEvent(e);
}
