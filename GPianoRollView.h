#ifndef GPIANOROLLVIEW_H
#define GPIANOROLLVIEW_H

#include <QGraphicsView>
#include <QObject>
#include <QMouseEvent>
#include <QRubberBand>

class GPianoRollScene;

enum GControlState
{
    Default,
    Pan,
    RubberBandSelect,
    VerticalZoom
};

class GPianoRollView : public QGraphicsView
{
    Q_OBJECT
public:
    GPianoRollView(QWidget *parent = Q_NULLPTR);
    GPianoRollView(GPianoRollScene *scene, QWidget *parent = Q_NULLPTR);

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;

private:
    void updateControlState(GControlState state) { mCS = state; }
    GControlState controlState() const { return mCS; }

private:
    GControlState mCS;
    std::unique_ptr<QRubberBand> mRubberBand = std::make_unique<QRubberBand>(QRubberBand::Rectangle, this);
    QPoint mLastMousePosition;
};

#endif // GPIANOROLLVIEW_H
