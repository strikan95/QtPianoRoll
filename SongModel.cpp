#include "SongModel.h"

SongModel::SongModel(QObject *parent) :
    QObject{parent}
{}

MNoteItem *SongModel::note(NoteId id) const
{
    return mNotes.contains(id) ? mNotes[id] : Q_NULLPTR;
}

NoteId SongModel::loadNote(int pitch, int startTime, int duration)
{
    NoteId id = createNote(pitch, startTime, duration);
    emit noteLoaded(id);

    return id;
}

NoteId SongModel::addNote(int pitch, int startTime, int duration)
{
    NoteId id = createNote(pitch, startTime, duration);
    emit noteAdded(id);

    return id;
}

NoteId SongModel::createNote(int pitch, int startTime, int duration)
{
    NoteId id = mLastNoteId++;
    MNoteItem *note = new MNoteItem(id, pitch, startTime, duration);

    mNotes.insert(id, note);
    return id;
}

void SongModel::setPosition(NoteId id, QPoint pos)
{
    MNoteItem *note = mNotes[id];
    note->mStartTime = pos.x();
    note->mPitch = pos.y();

    emit notePositionChanged(id);
}

void SongModel::setDuration(NoteId id, int duration)
{
    if(duration <= 0)
        return;

    MNoteItem *note = mNotes[id];
    note->mDuration = duration;

    emit noteDurationChanged(id);
}

void SongModel::removeNote(NoteId id)
{
    if (!mNotes.contains(id))
        return;

    mNotes[id] = Q_NULLPTR;

    emit noteRemoved(id);
}
