#include "GNoteObject.h"

#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPen>
#include <QCursor>

#include "GPianoRollScene.h"
#include "SongModel.h"

GNoteObject::GNoteObject(GPianoRollScene *scene, GNoteId id, QRect noteRect, GNoteState state) :
    mScene(scene),
    mId(id),
    mNoteRect(noteRect),
    mState(state)
{
    scene->addItem(this);

    setAcceptHoverEvents(true);
    setFlag(ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

void GNoteObject::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
{
    if(mState != GNoteState::IDLE)
        return;

    const auto mouseX = e->pos().x();

    bool isResizing = isMouseNearEdge(mouseX);
    setCursor(isResizing ? Qt::SizeHorCursor : Qt::SizeAllCursor);
}

bool GNoteObject::isMouseNearEdge(int mouseX)
{
    qDebug() << "Mousex:" << mouseX << "Note rect width:" << mNoteRect.width() << mNoteRect.width() - 20;
    return mouseX >= (mNoteRect.width() - 20);
}

void GNoteObject::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    if(e->button() & Qt::LeftButton)
    {
        const bool isCtrl = e->modifiers() & Qt::ControlModifier;
        if(isCtrl)
            scene()->clearSelection();
        setSelected(true);
        update();

        if(mState == GNoteState::IDLE)
        {
            setState(isMouseNearEdge(e->pos().x()) ? GNoteState::RESIZING : GNoteState::MOVING);
        }
    }

    QGraphicsObject::mousePressEvent(e);
}

void GNoteObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    mState = GNoteState::IDLE;
}

void GNoteObject::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    switch (mState) {
    case GNoteState::MOVING:
    {
        const QPointF mousePos = e->scenePos();
        GIndex index = mScene->cellIndexAt(mousePos);

        mScene->model()->setPosition(mId, QPoint(index.col(), index.row()));
        break;
    }
    case GNoteState::RESIZING:
    {
        const QPointF mousePos = e->scenePos();
        const int mouseColIndex = mScene->cellIndexAt(mousePos).col();
        const int noteStartColIndex = mScene->cellIndexAt(pos()).col();

        if(mouseColIndex < noteStartColIndex)
            return;

        int newDur = mouseColIndex - noteStartColIndex + 1;
        mScene->model()->setDuration(mId, newDur);

        break;
    }
    default:
        break;
    }
}

void GNoteObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const auto highlightColor = Qt::yellow;
    const auto noteBrushColor = QColor::fromRgb(162, 210, 170);

    const auto highlightPen = QPen(highlightColor);
    const auto noteBrush = QBrush(noteBrushColor);

    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(noteBrush);

    if (isSelected()) {
        painter->setPen(highlightPen);
    }

    painter->drawRoundedRect(mNoteRect, 5.0, 5.0);
    //painter->drawRoundedRect(1, 1, (mSize.width() * mDuration) - 2, mSize.height() - 2, 5.0, 5.0);
    painter->restore();
}

void GNoteObject::setState(GNoteState state)
{
    mState = state;

    if(mState != GNoteState::IDLE)
        setCursor(mState == GNoteState::RESIZING ? Qt::SizeHorCursor : Qt::SizeAllCursor);
}

QRectF GNoteObject::boundingRect() const
{
    QMarginsF margins(10, 10, 10, 10); // 10 px margin on all sides
    return QRectF(mNoteRect).marginsAdded(margins);
}
