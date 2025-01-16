#ifndef GNOTEOBJECT_H
#define GNOTEOBJECT_H

#include <QGraphicsObject>
#include <QObject>

class GPianoRollScene;
class GNoteObject;

class GNoteObject : public QGraphicsObject
{
    Q_OBJECT
public:
    GNoteObject(GPianoRollScene *scene, QRect noteRect);
    QRectF boundingRect() const override;

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *e) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e) override;

private:
    GPianoRollScene *mScene;
    QRect mNoteRect;
};

#endif // GNOTEOBJECT_H
