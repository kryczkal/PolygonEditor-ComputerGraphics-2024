//
// Created by wookie on 10/11/24.
//

#include "VertexItem.h"
#include "PolygonItem.h"
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>

VertexItem::VertexItem(const QPointF &position, QGraphicsItem *parent) : QGraphicsItem(parent), position{position} {}

QRectF VertexItem::boundingRect() const
{
    return QRectF(position.x() - radius, position.y() - radius, 2 * radius, 2 * radius);
}

void VertexItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(Qt::black);
    painter->drawEllipse(position, radius, radius);
}

void VertexItem::setPosition(const QPointF &newPosition)
{
    position = newPosition;
    update();
}

QPointF VertexItem::getPosition() const { return position; }
void VertexItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction *deleteAction   = menu.addAction("Delete");
    QAction *selectedAction = menu.exec(event->screenPos());

    PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
    if (selectedAction == deleteAction)
    {
        polygon->deleteVertex(this);
    }
}

QDataStream &operator<<(QDataStream &out, const VertexItem &vertex)
{
    out << vertex.position;
    return out;
}
