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

class CharacterEntity : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    CharacterEntity(const QString& gifPath, const QRect& screenBounds)
        : gif(gifPath) {
        if (!gif.isValid()) {
            qDebug() << "Failed to load GIF: " << gifPath;
            return;
        }
        setPixmap(gif.currentPixmap());
        connect(&gif, &QMovie::frameChanged, this, &CharacterEntity::updateFrame);
        gif.start();

        int x = QRandomGenerator::global()->bounded(screenBounds.width() - pixmap().width());
        int y = QRandomGenerator::global()->bounded(screenBounds.height() - pixmap().height());
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
    CharacterWindow(const QString& gifPath) {
        scene = new QGraphicsScene(this);
        setScene(scene);
        setBackgroundBrush(Qt::transparent);
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
        setAttribute(Qt::WA_TranslucentBackground);
        setRenderHint(QPainter::SmoothPixmapTransform);
        setStyleSheet("background: transparent; border: none;");

        // get screen geometry (for full screen or multiple screens)
        QRect totalGeometry;
        const QList<QScreen*> screens = QGuiApplication::screens();
        for (QScreen* screen : screens) {
            totalGeometry = totalGeometry.united(screen->geometry());
        }

        // set window geometry to cover the entire screen(s)
        setGeometry(totalGeometry);

        // create CharacterEntity and add it to the scene
        CharacterEntity* character = new CharacterEntity(gifPath, totalGeometry);
        scene->addItem(character);

        // resize window to match the total screen area
        if (!character->pixmap().isNull()) {
            setFixedSize(totalGeometry.size());
        }
    }

private:
    QGraphicsScene* scene;
};
