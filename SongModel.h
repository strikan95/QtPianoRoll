#ifndef SONGMODEL_H
#define SONGMODEL_H

#include <QMap>
#include <QObject>
#include <QPoint>

#include "Defines.h"

struct GNoteItem
{
    GNoteItem(GNoteId id, int pitch, int startTime, int duration)
        : mId(id)
        , mPitch(pitch)
        , mStartTime(startTime)
        , mDuration(duration)
    {}

    GIndex index() const { return GIndex(mPitch, mStartTime); }
    GNoteId id() const { return mId; }
    int startTime() const { return mStartTime; }
    int duration() const { return mDuration; }
    int pitch() const { return mPitch; }

    GNoteId mId;
    int mPitch;
    int mStartTime;
    int mDuration;
};

class SongModel : public QObject
{
    Q_OBJECT
public:
    explicit SongModel(QObject *parent = nullptr);

    GNoteId addNote(int pitch, int startTime, int duration);
    void removeNote(GNoteId id);
    GNoteItem *note(GNoteId id) const;

    void setPosition(GNoteId id, GIndex pos);
    void setDuration(GNoteId id, int duration);

Q_SIGNALS:
    void notePositionChanged(GNoteId id);
    void noteDurationChanged(GNoteId id);
    void noteAdded(GNoteId id);
    void noteRemoved(GNoteId id);

private:
    GNoteId createNote(int pitch, int startTime, int duration);

private:
    GNoteId mLastNoteId = 0;
    QMap<GNoteId, GNoteItem *> mNotes;
};

#endif // SONGMODEL_H
