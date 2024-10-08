#ifndef DRAWCANVAS_H
#define DRAWCANVAS_H

#include <QWidget>
#include <QPainter>

class DrawCanvas : public QWidget {
    Q_OBJECT

public:
    explicit DrawCanvas(QWidget *parent = nullptr) : QWidget(parent) {
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);

        // Set the pen color and width (for drawing pixels)
        painter.setPen(QPen(Qt::black, 1));

        painter.drawPoint(50, 50);
    }
};

#endif // DRAWCANVAS_H
