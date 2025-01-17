#ifndef SONGMODEL_H
#define SONGMODEL_H

#include <QMap>
#include <QObject>
#include <QPoint>

#include "Defines.h"

struct MNoteItem
{
    MNoteItem(NoteId id, int pitch, int startTime, int duration)
        : mId(id)
        , mPitch(pitch)
        , mStartTime(startTime)
        , mDuration(duration)
    {}

    GIndex index() const { return GIndex(mPitch, mStartTime); }
    NoteId id() const { return mId; }
    int startTime() const { return mStartTime; }
    int duration() const { return mDuration; }
    int pitch() const { return mPitch; }

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
    NoteId loadNote(int pitch, int startTime, int duration);
    void removeNote(NoteId id);
    MNoteItem *note(NoteId id) const;

    void setPosition(NoteId id, QPoint pos);
    void setDuration(NoteId id, int duration);

Q_SIGNALS:
    void notePositionChanged(NoteId id);
    void noteDurationChanged(NoteId id);
    void noteAdded(NoteId id);
    void noteLoaded(NoteId id);
    void noteRemoved(NoteId id);

private:
    NoteId createNote(int pitch, int startTime, int duration);

private:
    NoteId mLastNoteId = 0;
    QMap<NoteId, MNoteItem *> mNotes;
};

#endif // SONGMODEL_H
