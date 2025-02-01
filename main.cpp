#include <QtWidgets/QApplication>
#include <QSystemTrayIcon>
#include <QTimer.h>
#include "render.h"

// global pointer to the main entity
CharacterWindow* konata = nullptr;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);


    // create base konata entity
    QString GifPath = ":/KonataDancer/resource/konata/Konata.gif";
    konata = new CharacterWindow(GifPath);
    konata->show();

    // calculate the center of the primary display
    QScreen* screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        int centerX = screenGeometry.width() / 2 - konata->width() / 2;
        int centerY = screenGeometry.height() / 2 - konata->height() / 2;
        konata->move(centerX, centerY);
    } else {
        konata->move(0, 0);
    }


    // create and initialize tray icon
    QSystemTrayIcon trayIcon;
    QIcon icon(":/KonataDancer/resource/ui/tray-icon.png");
    trayIcon.setIcon(icon);
    trayIcon.setVisible(true);


    // create the context menu
    QMenu* menu = new QMenu();

    QAction* openAboutLink = new QAction("About", &trayIcon);
    QObject::connect(openAboutLink, &QAction::triggered, []() {
        QDesktopServices::openUrl(QUrl("https://konata.nightvoid.com", QUrl::TolerantMode));
        });
    menu->addAction(openAboutLink);

    QAction* openSourceLink = new QAction("Source", &trayIcon);
    QObject::connect(openSourceLink, &QAction::triggered, []() {
        QDesktopServices::openUrl(QUrl("https://software.nightvoid.com/stable/#konata", QUrl::TolerantMode));
        });
    menu->addAction(openSourceLink);

    QAction* openGithubLink = new QAction("GitHub", &trayIcon);
    QObject::connect(openGithubLink, &QAction::triggered, []() {
        QDesktopServices::openUrl(QUrl("https://github.com/M1chaelJackson/Konata-Dancer-Remake", QUrl::TolerantMode));
        });
    menu->addAction(openGithubLink);

    QAction* quitAction = new QAction("Quit", &trayIcon);
    QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);
    menu->addAction(quitAction);
    trayIcon.setContextMenu(menu);


    // if tray icon is clicked, spawn an introduction entity
    QObject::connect(&trayIcon, &QSystemTrayIcon::activated, [&](QSystemTrayIcon::ActivationReason reason) {
        // bool activeIntroduction = false; add later - stop access to multiple introduction entity instances
        if (reason == QSystemTrayIcon::Trigger) {

            // get konata's position.
            QPointF scenePos = konata->getCharacterPosition();


            // convert the scene position to the view's coordinates.
            QPoint viewPos = konata->mapFromScene(scenePos);


            // Now convert that to global coordinates.
            QPoint globalPos = konata->mapToGlobal(viewPos);
            qDebug() << "Global character position:" << globalPos;


            // create introduction entity
            CharacterWindow* hello = new CharacterWindow(":/KonataDancer/resource/konata/hello.gif");
            hello->show();


            // move to original konata entity
            hello->move(globalPos.x() - 2850, globalPos.y() - 1010); // the current calculation/geometry system is unstable, subdivision should fix the coordinates
            qDebug() << "Introduction entity spawned at:" << hello->pos();


            QTimer::singleShot(4600, [hello]() { // make the entity temporary - lasts 4.6 seconds
                hello->close();
                hello->deleteLater(); // optimize memory consumption
            });

        }
        });



    return app.exec();
}
