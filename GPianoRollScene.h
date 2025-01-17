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

    NoteId getNoteId(GNoteObject *obj);

    QPointF cellRectAt(GIndex index);
    QPointF cellRectAt(QPointF pos);
    GIndex cellIndexAt(QPointF pos);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *e) override;

public Q_SLOTS:
    void onNoteLoaded(NoteId id);
    void onNoteAdded(NoteId id);
    void onNoteRemoved(NoteId id);

    void onNotePositionChanged(NoteId id);
    void onNoteDurationChanged(NoteId id);

private:
    GNoteObject *createNoteObject(NoteId id);

private:
    SongModel *mModel;
    QMap<NoteId, GNoteObject *> mNoteGraphicalObjects;
    GNoteObject *mDraftNote;
};

#endif // GPIANOROLLSCENE_H
