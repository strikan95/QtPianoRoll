#ifndef GNOTEOBJECT_H
#define GNOTEOBJECT_H

#include <QGraphicsObject>
#include <QObject>

#include "Defines.h"

class GPianoRollScene;
class GNoteObject;

enum GNoteState {
    INITIALIZING,
    MOVING,
    RESIZING,
    PLACING,
    PLACED
};

class GNoteObject : public QGraphicsObject
{
    Q_OBJECT
public:
    GNoteObject(GPianoRollScene *scene, NoteId id, QRect noteRect, GNoteState state = GNoteState::INITIALIZING);
    QRectF boundingRect() const override;

    void setNoteRect(QRect noteRect) { prepareGeometryChange(); mNoteRect = noteRect; }
    void setInitialState(GNoteState state)
    {
        if(mState != GNoteState::INITIALIZING)
            return;
        mState = state;
    }

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *e) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e) override;

    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    bool isMouseNearEdge(int mouseX);

private:
    GPianoRollScene *mScene;
    const NoteId mId;
    QRect mNoteRect;

    GNoteState mState;
};

#endif // GNOTEOBJECT_H
