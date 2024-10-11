#ifndef DRAWCANVAS_H
#define DRAWCANVAS_H

#include <QWidget>
#include <QPainter>

class DrawCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit DrawCanvas(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);

        painter.setPen(Qt::black);
        painter.setBrush(
            QBrush(Qt::black, Qt::SolidPattern)
            );

        drawLine(painter, 0, 0, 1000, 1000);
    }

    void drawLine(QPainter& painter, int x1, int y1, int x2, int y2) {
        // Bresenham’s Line Algorithm
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;

        while (true) {
            // Draw a pixel at the current position
            painter.drawPoint(x1, y1);

            if (x1 == x2 && y1 == y2)
                break;

            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }

signals:
};

#endif // DRAWCANVAS_H
