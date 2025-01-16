#ifndef GPIANOROLLSCENE_H
#define GPIANOROLLSCENE_H

#include <QGraphicsScene>
#include <QObject>
#include <QMap>

#include "Defines.h"

class SongModel;
class GNoteObject;

class GPianoRollScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GPianoRollScene(QObject *parent = nullptr);

    SongModel* model() const { return mModel; }
    void setModel(SongModel *model);

    NoteId getNoteId(GNoteObject *obj);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *e) override;

public Q_SLOTS:
    void onNoteAdded(NoteId id);
    void onNoteRemoved(NoteId id);

    void onNotePositionChanged(NoteId id);

private:
    SongModel *mModel;
    QMap<NoteId, GNoteObject *> mNoteGraphicalObjects;
    GNoteObject *mDraftNote;
};

#endif // GPIANOROLLSCENE_H
