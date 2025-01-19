#include "SongModel.h"

SongModel::SongModel(QObject *parent) :
    QObject{parent}
{}

GNoteItem *SongModel::note(GNoteId id) const
{
    return mNotes.contains(id) ? mNotes[id] : Q_NULLPTR;
}

GNoteId SongModel::addNote(int pitch, int startTime, int duration)
{
    GNoteId id = createNote(pitch, startTime, duration);
    emit noteAdded(id);

    return id;
}

GNoteId SongModel::createNote(int pitch, int startTime, int duration)
{
    GNoteId id = mLastNoteId++;
    GNoteItem *note = new GNoteItem(id, pitch, startTime, duration);

    mNotes.insert(id, note);
    return id;
}

void SongModel::setPosition(GNoteId id, QPoint pos)
{
    GNoteItem *note = mNotes[id];
    note->mStartTime = pos.x();
    note->mPitch = pos.y();

    emit notePositionChanged(id);
}

void SongModel::setDuration(GNoteId id, int duration)
{
    if(duration <= 0)
        return;

    GNoteItem *note = mNotes[id];
    note->mDuration = duration;

    emit noteDurationChanged(id);
}

void SongModel::removeNote(GNoteId id)
{
    if (!mNotes.contains(id))
        return;

    mNotes[id] = Q_NULLPTR;

    emit noteRemoved(id);
}
