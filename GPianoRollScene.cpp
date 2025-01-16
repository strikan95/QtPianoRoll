#include "GPianoRollScene.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

#include "GNoteObject.h"
#include "SongModel.h"

GPianoRollScene::GPianoRollScene(QObject *parent)
    : QGraphicsScene(parent)
{
    setBackgroundBrush(Qt::gray);
}

void GPianoRollScene::setModel(SongModel *model)
{
    mModel = model;

    connect(model, &SongModel::noteAdded, this, &GPianoRollScene::onNoteAdded);
    connect(model, &SongModel::noteRemoved, this, &GPianoRollScene::onNoteRemoved);
    connect(model, &SongModel::notePositionChanged, this, &GPianoRollScene::onNotePositionChanged);
}

NoteId GPianoRollScene::getNoteId(GNoteObject *obj)
{
    return mNoteGraphicalObjects.key(obj);
}

void GPianoRollScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    switch (e->button()) {
    case Qt::LeftButton: {
        // pass click if there are items underneath
        if (items(e->scenePos()).count()) {
            QGraphicsScene::mousePressEvent(e);
            return;
        }

        // else well start adding new item
        QPointF scenePressPos = e->scenePos();

        int cellRowIndex = std::floor(scenePressPos.y() / NOTE_HEIGHT);
        int cellColIndex = std::floor(scenePressPos.x() / NOTE_WIDTH);

        model()->addNote(cellRowIndex, cellColIndex, 1);
        QGraphicsScene::mousePressEvent(e); // pass it onto new notes gobj
        break;
    }
    case Qt::RightButton: {
        if (items(e->scenePos()).count()) {
            foreach (const auto item, items(e->scenePos())) {
                NoteId id = mNoteGraphicalObjects.key(qgraphicsitem_cast<GNoteObject *>(item));
                mModel->removeNote(id);
            }
        }
        break;
    }
    default:
        QGraphicsScene::mousePressEvent(e);
        break;
    }
}

void GPianoRollScene::onNoteAdded(NoteId id)
{
    MNoteItem *noteP = model()->note(id);
    if (noteP) {
        GNoteObject *ngobj
            = new GNoteObject(this, QRect(0, 0, NOTE_WIDTH * noteP->mDuration, NOTE_HEIGHT));
        mNoteGraphicalObjects[id] = ngobj;

        QPoint noteIndex = QPoint(noteP->mStartTime, noteP->mPitch);
        const double cellXPos = noteIndex.x() * NOTE_WIDTH;
        const double cellYPos = noteIndex.y() * NOTE_HEIGHT;

        ngobj->setPos(cellXPos, cellYPos);
    }
}

void GPianoRollScene::onNoteRemoved(NoteId id)
{
    if (!mNoteGraphicalObjects.contains(id))
        return;

    mNoteGraphicalObjects[id]->deleteLater();
    mNoteGraphicalObjects[id] = Q_NULLPTR;
}

void GPianoRollScene::onNotePositionChanged(NoteId id)
{
    if (!mNoteGraphicalObjects.contains(id))
        return;

    MNoteItem *noteP = model()->note(id);
    if (noteP) {
        const int cellXPos = noteP->mStartTime * NOTE_WIDTH;
        const int cellYPos = noteP->mPitch * NOTE_HEIGHT;

        mNoteGraphicalObjects[id]->setPos(cellXPos, cellYPos);
    }
}

void GPianoRollScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawBackground(painter, rect);

    QRectF windowRect = rect;
    QPointF tl = windowRect.topLeft();
    QPointF br = windowRect.bottomRight();

    int gridWidth = NOTE_WIDTH;
    int gridHeight = NOTE_HEIGHT;

    double left = std::floor(tl.x() / gridWidth - 0.5);
    double right = std::floor(br.x() / gridWidth + 1.0);
    double bottom = std::floor(tl.y() / gridHeight - 0.5);
    double top = std::floor(br.y() / gridHeight + 1.0);

    QPen linePen = QPen(QColor::fromRgb(36, 52, 62));
    linePen.setWidth(1);

    const auto bgColor1 = QColor::fromRgb(46, 62, 72);
    const auto bgBrush1 = QBrush(bgColor1);

    const auto bgColor2 = QColor::fromRgb(52, 68, 78);
    const auto bgBrush2 = QBrush(bgColor2);

    const int pianoBgPattern[] = {0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0};

    //draw alternating bg
    for (int yi = int(bottom); yi <= int(top); ++yi) {
        const auto bgBrush = pianoBgPattern[yi % 12] ? bgBrush1 : bgBrush2;
        painter->setBrush(bgBrush);
        painter->setPen(Qt::NoPen);

        QRectF rect(QPointF(0, yi * gridHeight),
                    QPointF((right + 1) * gridWidth, (yi + 1) * gridHeight));
        painter->drawRect(rect);
    }

    painter->setPen(linePen);

    // vertical lines
    for (int xi = int(left); xi <= int(right); ++xi) {
        QLineF line(xi * gridWidth, bottom * gridHeight, xi * gridWidth, top * gridHeight);
        painter->drawLine(line);
    }

    // horizontal lines
    for (int yi = int(bottom); yi <= int(top); ++yi) {
        QLineF line(left * gridWidth, yi * gridHeight, right * gridWidth, yi * gridHeight);
        painter->drawLine(line);
    }
}
