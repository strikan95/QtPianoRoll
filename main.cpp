#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>

#include "GPianoRollScene.h"
#include "GPianoRollView.h"
#include "SongModel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget mainWidget;
    QVBoxLayout *l = new QVBoxLayout(&mainWidget);

    auto scene = new GPianoRollScene();
    auto model = new SongModel();

    scene->setModel(model);

    auto view = new GPianoRollView(scene);
    l->addWidget(view);
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    //view->scale(10,10);

    mainWidget.setWindowTitle("[*]Piano Roll Component");
    mainWidget.resize(1600, 1100);
    int width = mainWidget.frameGeometry().width();
    int height = mainWidget.frameGeometry().height();

    QScreen *screen = a.primaryScreen();

    int screenWidth = screen->geometry().width();
    int screenHeight = screen->geometry().height();

    mainWidget.setGeometry((screenWidth / 2) - (width / 2),
                           (screenHeight / 2) - (height / 2),
                           width,
                           height);
    mainWidget.showNormal();

    model->loadNote(1, 2, 2);

    return a.exec();
}
