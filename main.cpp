#include <QtWidgets/QApplication>
#include <qrect.h>
#include "render.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // starting gif/animation
    QString gifPath = ":/KonataDancer/resource/konata/Konata.gif";

    // initialize window
    CharacterWindow window(gifPath);
    window.show();

    // calculate the center of the monitor
    QScreen* screen = QGuiApplication::primaryScreen();

    if (screen) {

        QRect screenGeometry = screen->geometry();

        int screenWidth = screenGeometry.width() / 2 - window.width() / 2;
        int screenHeight = screenGeometry.height() / 2 - window.height() / 2;

        window.move(screenWidth, screenHeight);
    }
    else
    {
        // set to 0, 0 if no primary monitor is found
        window.move(0, 0);

    }

    // set window size to GIF's size
    window.setFixedSize(window.size());

    return app.exec();
}
