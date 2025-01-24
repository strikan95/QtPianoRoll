#include "GPianoRollScene.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

#include "GNoteObject.h"
#include "SongModel.h"

GPianoRollScene::GPianoRollScene(QObject *parent) :
    QGraphicsScene(parent)
{
    setBackgroundBrush(Qt::gray);
}

void GPianoRollScene::setModel(SongModel *model)
{
    mModel = model;

    connect(model, &SongModel::noteAdded, this, &GPianoRollScene::onNoteAdded);
    connect(model, &SongModel::noteRemoved, this, &GPianoRollScene::onNoteRemoved);
    connect(model, &SongModel::notePositionChanged, this, &GPianoRollScene::onNotePositionChanged);
    connect(model, &SongModel::noteDurationChanged, this, &GPianoRollScene::onNoteDurationChanged);
}

GNoteId GPianoRollScene::getGNoteId(GNoteObject *obj)
{
    return mNoteGraphicalObjects.key(obj);
}

// for snapping to the grid
QPointF GPianoRollScene::cellRectAt(QPointF scenePos)
{
    GIndex index = cellIndexAt(scenePos);
    return QPointF(index.col() * NOTE_WIDTH, index.row() * NOTE_HEIGHT);
}

QPointF GPianoRollScene::cellRectAt(GIndex index)
{
    const double cellXPos = index.col() * NOTE_WIDTH;
    const double cellYPos = index.row() * NOTE_HEIGHT;

    return QPointF(cellXPos, cellYPos);
}

GIndex GPianoRollScene::cellIndexAt(QPointF pos)
{
    const int cellRowPos = std::floor(pos.y() / NOTE_HEIGHT);
    const int cellColPos = std::floor(pos.x() / NOTE_WIDTH);

    return GIndex(cellRowPos, cellColPos);
}

void GPianoRollScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    bool hasItemsUnderCursor = items(e->scenePos()).count() > 0;

    switch (e->button()) {
    case Qt::LeftButton: {
        // pass click if there are items underneath
        if (hasItemsUnderCursor) {
            QGraphicsScene::mousePressEvent(e);
            return;
        }

        // else well start adding new item
        QPointF scenePressPos = e->scenePos();
        auto cellIndex = cellIndexAt(scenePressPos);

        GNoteId id = model()->addNote(cellIndex.row(), cellIndex.col(), 1);
        mDraftNoteId = id;
        QGraphicsScene::mousePressEvent(e); // pass it onto new notes gobj
        break;
    }
    case Qt::RightButton: {
        if (hasItemsUnderCursor) {
            foreach (const auto item, items(e->scenePos())) {
                GNoteId id = mNoteGraphicalObjects.key(qgraphicsitem_cast<GNoteObject *>(item));
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

void GPianoRollScene::onNoteAdded(GNoteId id)
{
    GNoteItem *noteP = model()->note(id);
    if (noteP) {
        GNoteObject *ngobj
            = new GNoteObject(this, id, QRect(0, 0, NOTE_WIDTH * noteP->mDuration, NOTE_HEIGHT));
        mNoteGraphicalObjects[id] = ngobj;

        auto cellPos = cellRectAt(noteP->index());
        ngobj->setPos(cellPos.x(), cellPos.y());
    }
}

void GPianoRollScene::onNoteRemoved(GNoteId id)
{
    if (!mNoteGraphicalObjects.contains(id))
        return;

    mNoteGraphicalObjects[id]->deleteLater();
    mNoteGraphicalObjects[id] = Q_NULLPTR;
}

void GPianoRollScene::onNotePositionChanged(GNoteId id)
{
    if (!mNoteGraphicalObjects.contains(id))
        return;

    GNoteItem *noteP = model()->note(id);
    if (noteP) {
        auto cellPos = cellRectAt(noteP->index());

        mNoteGraphicalObjects[id]->setPos(cellPos.x(), cellPos.y());
    }
}

void GPianoRollScene::onNoteDurationChanged(GNoteId id)
{
    if (!mNoteGraphicalObjects.contains(id))
        return;

    GNoteItem *noteP = model()->note(id);
    if (noteP)
    {
        GNoteObject *ngobj = mNoteGraphicalObjects[id];
        ngobj->setNoteRect(QRect(0, 0, noteP->duration() * NOTE_WIDTH, NOTE_HEIGHT));
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
