//
// Created by wookie on 10/11/24.
//
#include <QAction>
#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>

#include "EdgeItem.h"
#include "PolygonItem.h"

EdgeItem::EdgeItem(VertexItem *start, VertexItem *end, QGraphicsItem *parent)
    : QGraphicsItem(parent), startVertex{start}, endVertex{end}
{
}

QRectF EdgeItem::boundingRect() const
{
    return QRectF(startVertex->position, endVertex->position).normalized().adjusted(-2, -2, 2, 2);
}

void EdgeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if constexpr (useBresenham)
    {
        painter->setRenderHint(QPainter::Antialiasing, false);
        painter->setPen(QPen(Qt::black, 1));
        bresenham(painter, startVertex->position, endVertex->position, width);
    }
    else
    {
        painter->setPen(QPen(Qt::black, width));
        painter->drawLine(startVertex->position, endVertex->position);
    }
}
void EdgeItem::bresenham(QPainter *painter, const QPointF &start, const QPointF &end, qreal width)
{
    // Bresenham's line algorithm
    int x1 = static_cast<int>(start.x());
    int y1 = static_cast<int>(start.y());
    int x2 = static_cast<int>(end.x());
    int y2 = static_cast<int>(end.y());

    int dx  = std::abs(x2 - x1);
    int dy  = std::abs(y2 - y1);
    int sx  = (x1 < x2) ? 1 : -1;
    int sy  = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    int halfWidth = std::max(1, static_cast<int>(width / 2));

    while (true)
    {
        // Draw the thickness by drawing multiple points perpendicular to the line direction
        for (int i = -halfWidth; i <= halfWidth; ++i)
        {
            if (dx > dy)
            {
                painter->drawPoint(x1, y1 + i); // Line is more horizontal, vary y to add thickness
            }
            else
            {
                painter->drawPoint(x1 + i, y1); // Line is more vertical, vary x to add thickness
            }
        }

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

QDataStream &operator<<(QDataStream &out, const EdgeItem &edge)
{
    out << "(" << edge.startVertex->position << ", " << edge.endVertex->position << ")";
    return out;
}
void EdgeItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    //    QMenu menu;
    //    QAction *subdivideAction = menu.addAction("Subdivide");
    //    QAction *selectedAction  = menu.exec(event->screenPos());
    //
    //    PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
    //    polygon->subdivideEdge(this);
}
