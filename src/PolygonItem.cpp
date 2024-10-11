#include <QGraphicsSceneMouseEvent>
#include <cmath>
#include <QGraphicsView>
#include <QDebug>

#include "PolygonItem.h"

PolygonItem::PolygonItem(const QPolygonF &polygon) : polygon(polygon), selectedVertexIndex(-1) {}

QRectF PolygonItem::boundingRect() const {
    return polygon.boundingRect();
}

void PolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    for (int i = 0; i < polygon.size() - 1; ++i) {
        drawBresenhamLine(painter, polygon[i], polygon[i + 1]);
    }
    drawBresenhamLine(painter, polygon.last(), polygon.first()); // Close the polygon
}

void PolygonItem::setPolygon(const QPolygonF &newPolygon) {
    polygon = newPolygon;
    update();
}

QPolygonF PolygonItem::getPolygon() const {
    return polygon;
}

void PolygonItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF clickPos = event->pos();
    selectedVertexIndex = findClosestVertex(clickPos);
    if (selectedVertexIndex != -1) {
        setFlag(QGraphicsItem::ItemIsMovable, true);
    } else {
        setFlag(QGraphicsItem::ItemIsMovable, false);
    }
}

void PolygonItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (selectedVertexIndex != -1) {
        polygon[selectedVertexIndex] = event->pos();
        setPolygon(polygon);
    }
}

void PolygonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *) {
    selectedVertexIndex = -1;
}

int PolygonItem::findClosestVertex(const QPointF &pos) {
    QList<QGraphicsView*> views = scene()->views();
    if (views.isEmpty()) return -1;
    QGraphicsView *view = views.first();
    int viewWidth = view->viewport()->width();
    int viewHeight = view->viewport()->height();

    qreal tolerance = 0.10 * qMin(viewWidth, viewHeight);

    tolerance = qMax(tolerance, 5.0);
    qDebug() << "Calculated tolerance:" << tolerance;

    for (int i = 0; i < polygon.size(); ++i) {
        if (QLineF(pos, polygon[i]).length() < tolerance) {
            return i;
        }
    }
    return -1; // No vertex selected
}

void PolygonItem::drawBresenhamLine(QPainter *painter, const QPointF &start, const QPointF &end) {
    int x1 = static_cast<int>(start.x());
    int y1 = static_cast<int>(start.y());
    int x2 = static_cast<int>(end.x());
    int y2 = static_cast<int>(end.y());

    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        painter->drawPoint(x1, y1);

        if (x1 == x2 && y1 == y2) break;

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
