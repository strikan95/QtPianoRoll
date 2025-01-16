#ifndef GPIANOROLLVIEW_H
#define GPIANOROLLVIEW_H

#include <QGraphicsView>
#include <QObject>

class GPianoRollScene;

class GPianoRollView : public QGraphicsView
{
    Q_OBJECT
public:
    GPianoRollView(QWidget *parent = Q_NULLPTR);
    GPianoRollView(GPianoRollScene *scene, QWidget *parent = Q_NULLPTR);
};

#endif // GPIANOROLLVIEW_H
