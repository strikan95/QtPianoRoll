#ifndef DEFINES_H
#define DEFINES_H

using NoteId = unsigned int;

struct GIndex
{
public:
    GIndex(int row, int col) : mRow(row), mCol(col) {}
    int row() const { return mRow; }
    int col() const { return mCol; }

private:
    int mRow;
    int mCol;
};

const double NOTE_WIDTH = 100.0;
const double NOTE_HEIGHT = 50.0;

#endif // DEFINES_H
