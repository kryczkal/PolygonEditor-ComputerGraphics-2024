//
// Created by wookie on 10/11/24.
//

#include <QAction>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>

#include "PolygonItem.h"
#include "VertexItem.h"

VertexItem::VertexItem(const QPointF &position, QGraphicsItem *parent) : QGraphicsItem(parent), position{position}
{
    assert(dynamic_cast<PolygonItem *>(parent) != nullptr); // Parent must be a PolygonItem
    QGraphicsItem::setZValue(1);
}

QRectF VertexItem::boundingRect() const
{
    return QRectF(position.x() - radius, position.y() - radius, 2 * radius, 2 * radius);
}

void VertexItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(Qt::black);
    painter->drawEllipse(position, radius, radius);

    // Paint the number of the vertex
    PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
    if (polygon->paintIndex) {
        // Get the text to display
        QString text = QString::number(static_cast<int>(polygon->getVertexIndex(this)));

        // Create the text position
        QPointF textPosition = position + QPointF(radius, -radius);

        // Create a QPainterPath to outline the text (for the black stroke)
        QPainterPath path;
        path.addText(textPosition, painter->font(), text);

        // Draw the black stroke
        QPen pen(Qt::black, 3.0);
        painter->setPen(pen);
        painter->drawPath(path);

        // Fill the text with white color
        painter->setPen(Qt::NoPen);  // Disable the pen for filling
        painter->setBrush(Qt::white);
        painter->drawPath(path);
    }
}

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
bool VertexItem::hasBothEdges() const { return edgeFrom != nullptr && edgeTo != nullptr; }

bool VertexItem::hasOneEdge() const { return (!hasBothEdges() && (edgeFrom != nullptr || edgeTo != nullptr)); }
