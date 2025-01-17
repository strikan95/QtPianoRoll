#include "GNoteObject.h"

#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPen>
#include <QCursor>

#include "GPianoRollScene.h"
#include "SongModel.h"

GNoteObject::GNoteObject(GPianoRollScene *scene, NoteId id, QRect noteRect, GNoteState state) :
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
    const auto mouseX = e->pos().x();
    bool isResizing = (mState == GNoteState::PLACING && (e->modifiers() & Qt::ShiftModifier))
                      || (isMouseNearEdge(mouseX) && mState != GNoteState::PLACING);

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

        const bool isResizing = e->modifiers() & Qt::ShiftModifier;
        if (isResizing)
        {
            mState = GNoteState::RESIZING;
        }
        else if (mState != GNoteState::PLACING) // let state stay as PLACING if it already is
        {
            mState = GNoteState::MOVING;
        }
    }

    QGraphicsObject::mousePressEvent(e);
}

void GNoteObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    mState = GNoteState::PLACED;
}

void GNoteObject::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    switch (mState) {
    case GNoteState::PLACING:
    case GNoteState::MOVING:
    {
        qDebug() << "Moving note";
        const QPointF mousePos = e->scenePos();
        GIndex index = mScene->cellIndexAt(mousePos);

        mScene->model()->setPosition(mId, QPoint(index.col(), index.row()));
        break;
    }
    case GNoteState::RESIZING:
    {
        qDebug() << "Resizing note";

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

QRectF GNoteObject::boundingRect() const
{
    QMarginsF margins(10, 10, 10, 10); // 10 px margin on all sides
    return QRectF(mNoteRect).marginsAdded(margins);
}
