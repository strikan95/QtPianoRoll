#ifndef DEFINES_H
#define DEFINES_H

using GNoteId = unsigned int;

struct GIndex
{
public:
    GIndex(int row, int col) : mRow(row), mCol(col) {}
    int row() const { return mRow; }
    int col() const { return mCol; }

    bool isNull() const { return mRow != 0 || mCol != 0; }

    GIndex operator+(const GIndex& other) const {
        return {mRow + other.row(), mCol + other.col()};
    }

    GIndex operator-(const GIndex& other) const {
        return {mRow - other.row(), mCol - other.col()};
    }

private:
    int mRow;
    int mCol;
};

const double NOTE_WIDTH = 100.0;
const double NOTE_HEIGHT = 50.0;

#endif // DEFINES_H
