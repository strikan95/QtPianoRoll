#include "GPianoRollView.h"
#include "GPianoRollScene.h"

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
