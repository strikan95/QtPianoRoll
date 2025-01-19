#ifndef GPIANOROLLSCENE_H
#define GPIANOROLLSCENE_H

#include <QGraphicsScene>
#include <QMap>
#include <QObject>

#include "Defines.h"

class SongModel;
class GNoteObject;

class GPianoRollScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GPianoRollScene(QObject *parent = nullptr);

    SongModel *model() const { return mModel; }
    void setModel(SongModel *model);

    GNoteId getGNoteId(GNoteObject *obj);

    QPointF cellRectAt(GIndex index);
    QPointF cellRectAt(QPointF pos);
    GIndex cellIndexAt(QPointF pos);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *e) override;

public Q_SLOTS:
    void onNoteAdded(GNoteId id);
    void onNoteRemoved(GNoteId id);

    void onNotePositionChanged(GNoteId id);
    void onNoteDurationChanged(GNoteId id);

private:
    GNoteObject *createNoteObject(GNoteId id);

private:
    SongModel *mModel;
    QMap<GNoteId, GNoteObject *> mNoteGraphicalObjects;
    GNoteObject *mDraftNote;
};

#endif // GPIANOROLLSCENE_H
