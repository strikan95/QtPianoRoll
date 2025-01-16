#include "GNoteObject.h"

#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPen>

#include "GPianoRollScene.h"
#include "SongModel.h"

GNoteObject::GNoteObject(GPianoRollScene *scene, QRect noteRect)
    : mScene(scene)
    , mNoteRect(noteRect)
{
    scene->addItem(this);

    setAcceptHoverEvents(true);
    setFlag(ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

void GNoteObject::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    qDebug() << "Received press event";

    if (!(e->button() & Qt::LeftButton))
        return;

    if (!(e->modifiers() & Qt::ControlModifier))
        scene()->clearSelection();
    setSelected(true);

    update();

    QGraphicsObject::mousePressEvent(e);
}

void GNoteObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    //QGraphicsObject::mouseReleaseEvent(e);
}

void GNoteObject::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    qDebug() << "Received move event";

    if (isSelected()) {
        const QPointF mousePos = e->scenePos();
        const int cellXIndex = std::floor(mousePos.x() / NOTE_WIDTH);
        const int cellYIndex = std::floor(mousePos.y() / NOTE_HEIGHT);

        const NoteId id = mScene->getNoteId(this);
        mScene->model()->setPosition(id, QPoint(cellXIndex, cellYIndex));

        return;
    }

    //QGraphicsObject::mouseMoveEvent(e);
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
