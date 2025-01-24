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
    const bool isCurrentlyDraft = mScene->getDraftNoteId() == mId;
    if(isCurrentlyDraft)
    {
        return;
    }

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
        if(!isCtrl)
            scene()->clearSelection();
        setSelected(true);
        update();

        const bool isCurrentlyDraft = mScene->getDraftNoteId() == mId;
        if(isCurrentlyDraft)
        {
            const bool isShift = e->modifiers() & Qt::ShiftModifier;
            setState(isShift ? GNoteState::RESIZING : GNoteState::MOVING);
        }
        else
        {
            setState(isMouseNearEdge(e->pos().x()) ? GNoteState::RESIZING : GNoteState::MOVING);
        }
    }

    QGraphicsObject::mousePressEvent(e);
}

void GNoteObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    mState = GNoteState::IDLE;

    if(mScene->getDraftNoteId() == mId)
    {
        mScene->clearDraftNote();
    }

}

void GNoteObject::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{    
    const GIndex currMouseIndex = mScene->cellIndexAt(e->scenePos());
    const GIndex prevMouseIndex = mScene->cellIndexAt(e->lastScenePos());
    const GIndex indexDelta = currMouseIndex - prevMouseIndex;

    if(indexDelta.row() == 0 && indexDelta.col() == 0)
        return;

    switch (mState) {
    case GNoteState::MOVING:
    {
        mScene->model()->setPosition(mId, getNoteData()->index() + indexDelta);
        break;
    }
    case GNoteState::RESIZING:
    {
        const auto note = getNoteData();
        const auto noteEndCol = note->index().col() + (note->duration() - 1);

        if(prevMouseIndex.col() == noteEndCol)
            mScene->model()->setDuration(mId, getNoteData()->duration() + indexDelta.col());
        break;
    }
    default:
        break;
    }
}

GNoteItem *GNoteObject::getNoteData()
{
    return mScene->model()->note(mId);
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

    painter->drawRoundedRect(1, 1, mNoteRect.width() - 2, mNoteRect.height() - 2, 5.0, 5.0);
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
