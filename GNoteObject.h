#ifndef GNOTEOBJECT_H
#define GNOTEOBJECT_H

#include <QGraphicsObject>
#include <QObject>

#include "Defines.h"

class GPianoRollScene;
class GNoteObject;
class GNoteItem;

enum GNoteState {
    MOVING,
    RESIZING,
    IDLE
};

class GNoteObject : public QGraphicsObject
{
    Q_OBJECT
public:
    GNoteObject(GPianoRollScene *scene, GNoteId id, QRect noteRect, GNoteState state = GNoteState::IDLE);
    QRectF boundingRect() const override;

    void setNoteRect(QRect noteRect) { prepareGeometryChange(); mNoteRect = noteRect; }
    void setState(GNoteState state);

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *e) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e) override;

    void hoverMoveEvent(QGraphicsSceneHoverEvent *e) override;

private:
    bool isMouseNearEdge(int mouseX);
    GNoteItem *getNoteData();

private:
    GPianoRollScene *mScene;
    const GNoteId mId;
    QRect mNoteRect;

    GNoteState mState;
};

#endif // GNOTEOBJECT_H
