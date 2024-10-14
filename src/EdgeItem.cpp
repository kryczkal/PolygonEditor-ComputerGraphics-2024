#include "EdgeItem.h"
#include "PolygonItem.h"
#include "VertexItem.h"
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <cassert>

static constexpr bool useBresenham = false;

EdgeItem::EdgeItem(VertexItem *start, VertexItem *end, QGraphicsItem *parent)
    : QGraphicsItem(parent), startVertex{start}, endVertex{end}
{
    assert(dynamic_cast<PolygonItem *>(parent) != nullptr); // Parent must be a PolygonItem
    setZValue(0);
}

QRectF EdgeItem::boundingRect() const
{
    QPointF startPoint = mapFromItem(startVertex, 0, 0);
    QPointF endPoint   = mapFromItem(endVertex, 0, 0);
    return QRectF(startPoint, endPoint).normalized().adjusted(-2, -2, 2, 2);
}

void EdgeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPointF startPoint = mapFromItem(startVertex, 0, 0);
    QPointF endPoint   = mapFromItem(endVertex, 0, 0);

    if constexpr (useBresenham)
    {
        painter->setRenderHint(QPainter::Antialiasing, false);
        painter->setPen(QPen(Qt::black, 1));
        bresenham(painter, startPoint, endPoint, width);
    }
    else
    {
        painter->setPen(QPen(Qt::black, width));
        painter->drawLine(startPoint, endPoint);
    }

    PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
    if (polygon->paintIndex)
    {
        QPointF center = (startPoint + endPoint) / 2;
        QString text   = QString::number(polygon->getEdgeIndex(this));

        QPainterPath path;
        path.addText(center, painter->font(), text);

        QPen pen(Qt::black, 3.0);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(path);

        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::white);
        painter->drawPath(path);
    }
}

void EdgeItem::bresenham(QPainter *painter, const QPointF &start, const QPointF &end, qreal width)
{
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
        for (int i = -halfWidth; i <= halfWidth; ++i)
        {
            if (dx > dy)
            {
                painter->drawPoint(x1, y1 + i);
            }
            else
            {
                painter->drawPoint(x1 + i, y1);
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
    out << "(" << edge.startVertex->pos() << ", " << edge.endVertex->pos() << ")";
    return out;
}

void EdgeItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction *subdivideAction = menu.addAction("Subdivide");
    QAction *selectedAction  = menu.exec(event->screenPos());

    PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
    polygon->subdivideEdge(this);
}
