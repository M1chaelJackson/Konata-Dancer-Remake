#ifndef RENDER_H
#define RENDER_H

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMovie>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include <QRandomGenerator>
#include <QScreen>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QAction>
#include <QFrame>
#include <QDesktopservices.h>

#include <QMenu>
#include <QAction>

class CharacterEntity : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    CharacterEntity(const QString& gifPath, const QRect& bounds)
        : gif(gifPath) {
        if (!gif.isValid()) {
            qDebug() << "Failed to load GIF:" << gifPath;
            return;
        }
        setPixmap(gif.currentPixmap());
        connect(&gif, &QMovie::frameChanged, this, &CharacterEntity::updateFrame);
        gif.start();

        // Place it randomly within the given bounds:
        int x = QRandomGenerator::global()->bounded(bounds.width() - pixmap().width());
        int y = QRandomGenerator::global()->bounded(bounds.height() - pixmap().height());
        setPos(x, y);
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        dragStartPos = event->pos();
        isDragging = true;
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        if (isDragging) {
            setPos(pos() + event->pos() - dragStartPos);
        }
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        Q_UNUSED(event);
        isDragging = false;
    }

    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override {
        QMenu menu;
        QAction* action1 = menu.addAction("About"); // temporarily useless
        QAction* action2 = menu.addAction("Source");
        QAction* action3 = menu.addAction("Github");
        QAction* action4 = menu.addAction("Exit");

        // execute the menu at the event's screen position.
        QAction* selectedAction = menu.exec(event->screenPos());
        if (selectedAction == action1) {
            QDesktopServices::openUrl(QUrl("https://konata.nightvoid.com", QUrl::TolerantMode));
        }
        else if (selectedAction == action2) {
            QDesktopServices::openUrl(QUrl("https://software.nightvoid.com/stable/#konata", QUrl::TolerantMode));
        }
        else if (selectedAction == action3) {
            QDesktopServices::openUrl(QUrl("https://github.com/M1chaelJackson/Konata-Dancer-Remake", QUrl::TolerantMode));
        }
        else if (selectedAction == action4) {
            QApplication::exit();

        }

        event->accept();
    }

private slots:
    void updateFrame() {
        setPixmap(gif.currentPixmap());
    }

private:
    QMovie gif;
    QPointF dragStartPos;
    bool isDragging = false;
};

class CharacterWindow : public QGraphicsView {
public:
    // use the primary screen geometry.
    CharacterWindow(const QString& gifPath) {
        scene = new QGraphicsScene(this);
        setScene(scene);
        setBackgroundBrush(Qt::transparent);
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
        setAttribute(Qt::WA_TranslucentBackground);
        setRenderHint(QPainter::SmoothPixmapTransform);
        setStyleSheet("background: transparent; border: none;");
        setFrameShape(QFrame::NoFrame);
        setViewportMargins(0, 0, 0, 0);


        QRect totalGeometry;
        const QList<QScreen*> screens = QGuiApplication::screens();
        for (QScreen* screen : screens) {
            totalGeometry = totalGeometry.united(screen->geometry());
        }

        // set window geometry to cover the entire screen(s)
        setGeometry(totalGeometry);

        // create the CharacterEntity using the chosen bounds.
        character = new CharacterEntity(gifPath, totalGeometry);
        scene->addItem(character);


    }

    // function to retrieve the character’s position.
    QPointF getCharacterPosition() const {
        return character ? character->pos() : QPointF(0, 0);
    }

private:
    QGraphicsScene* scene;
    CharacterEntity* character;
};


#endif // !RENDER_H
