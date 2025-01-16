#ifndef SONGMODEL_H
#define SONGMODEL_H

#include <QObject>
#include <QMap>
#include <QPoint>

#include "Defines.h"

struct MNoteItem
{
    MNoteItem(NoteId id, int pitch, int startTime, int duration):
        mId(id),
        mPitch(pitch),
        mStartTime(startTime),
        mDuration(duration)
    {}

    NoteId mId;
    int mPitch;
    int mStartTime;
    int mDuration;
};

class SongModel : public QObject
{
    Q_OBJECT
public:
    explicit SongModel(QObject *parent = nullptr);

    NoteId addNote(int pitch, int startTime, int duration);
    void removeNote(NoteId id);
    MNoteItem* note(NoteId id) const;

    void setPosition(NoteId id, QPoint pos);

Q_SIGNALS:
    void notePositionChanged(NoteId id);
    void noteDurationChanged(NoteId id);
    void noteAdded(NoteId id);
    void noteRemoved(NoteId id);

private:
    NoteId mLastNoteId = 0;
    QMap<NoteId, MNoteItem *> mNotes;
};

#endif // SONGMODEL_H
