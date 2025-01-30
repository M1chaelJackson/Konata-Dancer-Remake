#include "KonataDancer.h"
#include <QtWidgets/QApplication>
#include <qrect.h>
#include "render.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QString gifPath = ":/KonataDancer/resource/konata/Konata.gif";
    CharacterWindow window(gifPath);
    window.show();

    window.move(0, 0);
    window.setFixedSize(2560, 1440);
    return app.exec();
}
